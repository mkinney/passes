#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27, if new version please use 0x3F instead.

const int PIN_K1 = 3;
const int PIN_K2 = 4;
const int PIN_K3 = 5;
const int PIN_K4 = 6;

void setup()
{
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight

  // buttons
  Serial.begin(115200);

  pinMode(PIN_K1, INPUT_PULLUP);
  pinMode(PIN_K2, INPUT_PULLUP);
  pinMode(PIN_K3, INPUT_PULLUP);
  pinMode(PIN_K4, INPUT_PULLUP);
}

void loop()
{
  lcd.setCursor(3, 0); // set the cursor to column 3, line 0
  lcd.print("Hello GeeekPi");  // Print a message to the LCD

  //lcd.setCursor(2, 1); // set the cursor to column 2, line 1
  //lcd.print("hello world");  // Print a message to the LCD.

  if (!digitalRead(PIN_K1)) {
    lcd.setCursor(2, 1);
    lcd.print("K1 pressed");
  }

  if (!digitalRead(PIN_K2)) {
    lcd.setCursor(2, 1);
    lcd.print("K2 pressed");
  }

  if (!digitalRead(PIN_K3)) {
    lcd.setCursor(2, 1);
    lcd.print("K3 pressed");
  }
  if (!digitalRead(PIN_K4)) {
    lcd.setCursor(2, 1);
    lcd.print("K4 pressed");
  }

}
