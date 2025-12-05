#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Simple MAC vendor lookup (limited)
String getVendor(String mac) {
  mac.toUpperCase();
  if (mac.startsWith("FC:EC:DA")) return "Huawei";
  if (mac.startsWith("60:38:E0")) return "Xiaomi";
  if (mac.startsWith("A4:50:46")) return "TP-Link";
  if (mac.startsWith("78:44:FD")) return "JioFiber";
  if (mac.startsWith("B8:27:EB")) return "RaspberryPi";
  if (mac.startsWith("DC:A6:32")) return "Cisco";
  if (mac.startsWith("F4:0F:1B")) return "Netgear";
  return "Unknown";
}

String securityType(int type) {
  switch (type) {
    case WIFI_AUTH_OPEN: return "OPEN";
    case WIFI_AUTH_WEP: return "WEP";
    case WIFI_AUTH_WPA_PSK: return "WPA";
    case WIFI_AUTH_WPA2_PSK: return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA";
    case WIFI_AUTH_WPA3_PSK: return "WPA3";
    default: return "OTHER";
  }
}

String securityRank(String sec) {
  if (sec == "OPEN") return "Dangerous!";
  if (sec == "WEP") return "Broken!!";
  if (sec == "WPA") return "Old Prot";
  if (sec == "WPA2") return "Secure ✔";
  if (sec == "WPA3") return "Strong ✔✔";
  return "UnknownSec";
}

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" WiFi Security");
  delay(1500);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
}

void loop() {
  lcd.clear();
  lcd.print("Scanning...");
  delay(1000);

  int n = WiFi.scanNetworks();
  if (n == 0) {
    lcd.clear();
    lcd.print("No WiFi Found");
    delay(1500);
  } else {
    for (int i = 0; i < n; i++) {

      String ssid = WiFi.SSID(i);
      String mac = WiFi.BSSIDstr(i);
      String sec = securityType(WiFi.encryptionType(i));
      String rank = securityRank(sec);
      String vendor = getVendor(mac);

      // Detect duplicate SSID
      bool duplicate = false;
      for (int j = 0; j < n; j++) {
        if (i != j && WiFi.SSID(i) == WiFi.SSID(j)) {
          duplicate = true;
        }
      }
      
      if(ssid.length() > 10) ssid = ssid.substring(0, 10);

      // Display header with SSID and security
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(ssid);
      lcd.print(" ");
      lcd.print(sec);

      // Bottom Line Logic
      lcd.setCursor(0,1);
      if (sec == "OPEN") lcd.print("❌ OPEN RISK");
      else if (duplicate) lcd.print("⚠ DUPLICATE!");
      else lcd.print(vendor.substring(0,16));

      delay(2000);

      lcd.setCursor(0,1);
      lcd.print(rank);
      delay(1200);
    }
  }

  delay(2000);
}
