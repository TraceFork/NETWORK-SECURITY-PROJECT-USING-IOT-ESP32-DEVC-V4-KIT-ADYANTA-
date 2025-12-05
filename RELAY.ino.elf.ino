#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LED1 33
#define LED2 32
#define RELAY_PIN 0

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Speeds
int normalSpeed = 300;  // First & Final permanent
int fastSpeed   = 50;   // Second speed
int slowSpeed   = 500;  // Third speed

unsigned long stateStartTime = 0;
int mode = 0;  
// 0 = first 300ms
// 1 = 50ms
// 2 = 500ms
// 3 = final permanent 300ms

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("L1  L2  RELAY");

  stateStartTime = millis();
}

void loop() {
  unsigned long now = millis();

  // Speed transition logic
  if(mode == 0 && now - stateStartTime >= 5000) { // after 5 sec → 50ms
    mode = 1;
    stateStartTime = now;
  }
  else if(mode == 1 && now - stateStartTime >= 2000) { // after 2 sec → 500ms
    mode = 2;
    stateStartTime = now;
  }
  else if(mode == 2 && now - stateStartTime >= 2000) { // after 2 sec → final
    mode = 3;
  }

  // Speed selection
  int delayTime;
  if(mode == 0) delayTime = normalSpeed;    // 300
  else if(mode == 1) delayTime = fastSpeed; // 50
  else if(mode == 2) delayTime = slowSpeed; // 500
  else delayTime = normalSpeed;            // FINAL permanent 300

  // ---- STATE 1 ----
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(RELAY_PIN, LOW);
  lcd.setCursor(0, 1);
  lcd.print("ON  OFF OFF ");
  delay(delayTime);

  // ---- STATE 2 ----
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  digitalWrite(RELAY_PIN, HIGH);
  lcd.setCursor(0, 1);
  lcd.print("OFF ON  ON  ");
  delay(delayTime);
}
