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

long x = 0;

bool spinning = false;

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

  show();

  // get to "zero" position
  delay(key_delay);
  move();

} // setup

void show() {
  lcd.setCursor(0, 0);
  lcd.print("Pass:");  // 1st line on lcd display

  // "clear" prior number
  lcd.setCursor(6, 0);
  lcd.print("   ");

  lcd.setCursor(7, 0);
  lcd.print(pass);

  lcd.setCursor(0, 1); // 2nd line on lcd display
  lcd.print("Total:");

  // "clear" prior number
  lcd.setCursor(6, 1);
  lcd.print("   ");

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
    pass++;
    show();
    delay(key_delay);
    move();
  }

  /* if ((!spinning) && (digitalRead(PIN_FROM_TORMACH))) {
      pass++;
      show();
      move();
  } */

  stepper1.run();
  /*
  if (stepper1.run()) {
    // .run() returns true if it is still turning
  } else {
    // once the stepper is done turning, turn off our boolean
    if (spinning) {
      spinning = false;
    }
  } */

} // loop
