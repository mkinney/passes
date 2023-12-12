// Copyright 2023 Mike Kinney
// license is the same as the Arduino license

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27, if new version please use 0x3F instead.

// digital IO pins for the buttons
const int PIN_K1 = 3;
const int PIN_K2 = 4;
const int PIN_K3 = 5;
const int PIN_K4 = 6;

// digital IO pins for interaction with Tormach USB IO Part#23616
// Pin 12 on Arduino is connected to RELAY_0_NC on USB IO board
// Pin Gnd on Arduino is connected to COMMON_0 on USB IO board
const int PIN_FROM_TORMACH = 12;

// the key_delay is so we only register the button once
const int key_delay = 200;

int pass = 0;
int total = 12;

int value_read = 0;

long x = 0;

bool spinning = false;

bool integrations = false;
bool integrations_set = false;

// stepper
// digital pin8 is PUL+
// digital pin9 is DIR+
// DIR- and PUL- are connected to ground
AccelStepper stepper1(AccelStepper::DRIVER, 8, 9);

void setup() {

  lcd.init();  // initialize the lcd screen
  lcd.backlight();  // open the backlight

  Serial.begin(115200);

  // buttons
  pinMode(PIN_K1, INPUT_PULLUP);
  pinMode(PIN_K2, INPUT_PULLUP);
  pinMode(PIN_K3, INPUT_PULLUP);
  pinMode(PIN_K4, INPUT_PULLUP);

  // Tormach IO
  pinMode(PIN_FROM_TORMACH, INPUT_PULLUP);

  // Note: It cannot really go this fast
  stepper1.setMaxSpeed(400.0);
  stepper1.setAcceleration(400.0);

  //show();

  // get to "zero" position
  delay(key_delay);
  move();

  integrations = integrations_prompt();

} // setup

void clear_lcd() {
  String blanks = "                    ";
  lcd.setCursor(0, 0);
  lcd.print(blanks);
  lcd.setCursor(0, 1);
  lcd.print(blanks);
}

bool integrations_prompt() {
  lcd.setCursor(0, 0);
  lcd.print("Integrations?    "); // be sure to clear the last few characters
  lcd.setCursor(0, 1);
  lcd.print("top=Y bottom=N");
}

void show() {
  clear_lcd();
  lcd.setCursor(0, 0);
  lcd.print("Pass:");  // 1st line on lcd display

  lcd.setCursor(7, 0);
  lcd.print(pass);

  // indicator for integrations
  lcd.setCursor(13, 0);
  if (integrations) {
    lcd.print("I=Y"); // yes, integrations is enabled
  } else {
    lcd.print("I=N"); // no
  }

  lcd.setCursor(0, 1); // 2nd line on lcd display
  lcd.print("Total:");

  lcd.setCursor(7, 1);
  lcd.print(total);
} // show

void move() {
  // stepper dips
  // 1=ON 2=ON 3=OFF 4=ON
  // the motor can take 3.0A, so configured for 2.8A and full current
  // 5=OFF 6=ON 7=ON 8=ON 400 pulses/rev
  // not sure why the magical number below works (derived from trial and error)
  x = (float)pass / (float)total * 360.0 * 55.55;
  delay(1000);
  spinning = true;
	stepper1.moveTo(x);
} // move 

void loop() {
  
  if (!digitalRead(PIN_K1)) {
    if (!integrations_set) {
      integrations = true;
      integrations_set = true;
      total++;
    }
    total--;
    pass = 0;
    show();
    delay(key_delay);
  }

  if (!digitalRead(PIN_K2)) {
    total++;
    pass = 0;
    show();
    delay(key_delay);
  }

  if (!digitalRead(PIN_K3)) {
    pass--;
    show();
    delay(key_delay);
    move();
  }

  if (!digitalRead(PIN_K4)) {
    if (!integrations_set) {
      integrations = false;
      integrations_set = true;
      pass--;
    }
    pass++;
    show();
    delay(key_delay);
    move();
  }

  if (integrations) {
    // from docs: https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/
    // "If the pin isn’t connected to anything, digitalRead() can return either HIGH or LOW (and this can change randomly)."
    value_read = digitalRead(PIN_FROM_TORMACH);
    if ((!spinning) && (value_read == HIGH)) {
        pass++;
        show();
        move();
    }
    if (stepper1.run()) {
      // .run() returns true if it is still turning
    } else {
      // once the stepper is done turning, turn off our boolean
      if (spinning) {
        spinning = false;
      }
    }
  } else { // no integration
    stepper1.run();
  }

} // loop
