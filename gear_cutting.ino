#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27, if new version please use 0x3F instead.

// digital IO
const int PIN_K1 = 3;
const int PIN_K2 = 4;
const int PIN_K3 = 5;
const int PIN_K4 = 6;

// the key_delay is so we only register the button once
const int key_delay = 200;

int tooth_num = 1;
int total_teeth = 10;

void setup() {
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight

  // buttons
  Serial.begin(115200);
  pinMode(PIN_K1, INPUT_PULLUP);
  pinMode(PIN_K2, INPUT_PULLUP);
  pinMode(PIN_K3, INPUT_PULLUP);
  pinMode(PIN_K4, INPUT_PULLUP);

  // steppers
  pinMode(9,OUTPUT); // set Pin9 as PUL
  pinMode(8,OUTPUT); // set Pin8 as DIR
  //pinMode(13,OUTPUT); // set Pin13 as ENA+ (not 100% sure on this one)

  show();

} // setup

void show() {
  lcd.setCursor(0, 0);
  lcd.print("Tooth num:");  // 1st line on lcd display

  // "clear" prior number
  lcd.setCursor(12, 0);
  lcd.print("   ");

  lcd.setCursor(12, 0);
  lcd.print(tooth_num);

  lcd.setCursor(0, 1); // 2nd line on lcd display
  lcd.print("Total teeth:");

  // "clear" prior number
  lcd.setCursor(13, 1);
  lcd.print("   ");

  lcd.setCursor(13, 1);
  lcd.print(total_teeth);
} // show

void loop() {
  
  if (!digitalRead(PIN_K1)) {
    total_teeth--;
    show();
    delay(key_delay);
  }

  if (!digitalRead(PIN_K2)) {
    total_teeth++;
    show();
    delay(key_delay);
  }

  if (!digitalRead(PIN_K3)) {
    tooth_num--;
    show();
    delay(key_delay);
  }

  if (!digitalRead(PIN_K4)) {
    tooth_num++;
    show();
    delay(key_delay);

    int x;
    //digitalWrite(13,HIGH);
    digitalWrite(8,HIGH); // set high level direction
    for (x = 0; x < 400; x++) {
      digitalWrite(9,HIGH); // Output high
      delayMicroseconds(500); // set rotate speed
      digitalWrite(9,LOW); // Output low
      delayMicroseconds(500); // set rotate speed
    }
    delay(1000); //pause 1 second
    digitalWrite(8,LOW); // set high level direction
    for (x = 0; x < 400; x++) {
      digitalWrite(9,HIGH);
      delayMicroseconds(500);
      digitalWrite(9,LOW);
      delayMicroseconds(500);
    }
    delay(1000);
  }

} // loop
