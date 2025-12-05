#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" WiFi Scanner ");
  delay(1500);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
}

void displayScrollingMAC(String mac) {
  // Scroll long MAC addresses
  if(mac.length() <= 16) {
    lcd.setCursor(0,1);
    lcd.print(mac);
    delay(1500);
  } else {
    for(int pos = 0; pos <= mac.length() - 16; pos++) {
      lcd.setCursor(0,1);
      lcd.print(mac.substring(pos, pos+16));
      delay(300);
    }
  }
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scanning...");
  delay(800);

  int n = WiFi.scanNetworks();
  lcd.clear();

  if (n == 0) {
    lcd.setCursor(0, 0);
    lcd.print("No networks");
    delay(2000);
  } else {
    for (int i = 0; i < n; i++) {
      lcd.clear();

      String ssid = WiFi.SSID(i);
      if(ssid.length() > 10) ssid = ssid.substring(0, 10);

      lcd.setCursor(0,0);
      lcd.print(ssid);
      lcd.print(" ");
      lcd.print(WiFi.RSSI(i));

      String mac = WiFi.BSSIDstr(i);
      displayScrollingMAC(mac);

      delay(1200);
    }
  }

  delay(1500); // wait before scanning again
}
