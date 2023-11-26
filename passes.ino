#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27, if new version please use 0x3F instead.

// digital IO pins for the buttons
const int PIN_K1 = 3;
const int PIN_K2 = 4;
const int PIN_K3 = 5;
const int PIN_K4 = 6;

// the key_delay is so we only register the button once
const int key_delay = 200;

int pass = 0;
int total = 10;

long x = 0;

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

  // Note: It cannot really go this fast
  stepper1.setMaxSpeed(400.0);
  stepper1.setAcceleration(400.0);

  show();

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
  x = (float)pass / (float)total * 360.0 * 50.0;    
  delay(1000);
	stepper1.moveTo(x);
} // move 

void loop() {
  
  if (!digitalRead(PIN_K1)) {
    total--;
    show();
    delay(key_delay);
  }

  if (!digitalRead(PIN_K2)) {
    total++;
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
    if (pass >= total) {
      pass = 0;
    }
    show();
    delay(key_delay);
    move();
  }

  stepper1.run();

} // loop
