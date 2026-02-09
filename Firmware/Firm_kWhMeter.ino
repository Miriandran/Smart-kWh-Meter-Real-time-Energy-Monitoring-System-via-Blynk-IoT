#define BLYNK_TEMPLATE_ID "TMPL6JjZI-7wS"
#define BLYNK_TEMPLATE_NAME "Smart kWh Meter"
#define BLYNK_AUTH_TOKEN "8e2mABeeEUQigZSzOvvFsm-my7Djd5v-"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "EmonLib.h"

// OLED
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long lastDisplaySwitch = 0;
int displayPage = 0;
const unsigned long displayInterval = 2000;

char ssid[] = "Ubi cilembu";
char pass[] = "lalalala";

// EnergyMonitor
EnergyMonitor emon;
const float vCalibration = 140;
const float currCalibration = 1.097;

float kWh = 0.0;
unsigned long lastMillis = 0;

// Blynk Virtual Pins
#define VPIN_VOLTAGE V0
#define VPIN_CURRENT V1
#define VPIN_POWER   V2
#define VPIN_KWH     V3

void printCentered(const String& text, int y) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  int x = (SCREEN_WIDTH - w) / 2;
  display.setCursor(x, y);
  display.println(text);
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Gagal memulai OLED"));
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println("Menghubungkan...");
  display.display();

  WiFi.begin(ssid, pass);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retries++;
    if (retries > 40) {
      Serial.println("Gagal koneksi ke WiFi!");
      display.clearDisplay();
      display.setCursor(0, 20);
      display.println("WiFi Gagal");
      display.display();
      while (true);  // Hentikan program
    }
  }

  Serial.println("\nWiFi Terhubung!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.println("WiFi Terhubung!");
  display.print("IP: ");
  display.println(WiFi.localIP());
  display.display();
  delay(2000);  // tampilkan dulu 2 detik

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  emon.voltage(35, vCalibration, 1.7);
  emon.current(34, currCalibration);

  lastMillis = millis();
}

void loop() {
  Blynk.run();

  emon.calcVI(20, 2000);
  float voltage = emon.Vrms;
  float current = emon.Irms;
  if (voltage <10){
    voltage = 0;
  }
  if (current < 0.07){
    current = 0;
  }
  float power = voltage * current;

  unsigned long currentMillis = millis();
  float elapsedHours = (currentMillis - lastMillis) / 3600000.0;
  lastMillis = currentMillis;
  kWh += power * elapsedHours / 1000.0;

  if (millis() - lastDisplaySwitch > displayInterval) {
    displayPage = (displayPage + 1) % 4;
    lastDisplaySwitch = millis();
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  switch (displayPage) {
    case 0:
      printCentered("Tegangan", 0);
      printCentered(String(voltage, 1) + " V", 30);
      break;
    case 1:
      printCentered("Arus", 0);
      printCentered(String(current, 2) + " A", 30);
      break;
    case 2:
      printCentered("Daya", 0);
      printCentered(String(power, 1) + " W", 30);
      break;
    case 3:
      printCentered("Energi", 0);
      printCentered(String(kWh, 3) + " kWh", 30);
      break;
  }
  display.display();

  // Kirim ke Blynk
  Blynk.virtualWrite(VPIN_VOLTAGE, voltage);
  Blynk.virtualWrite(VPIN_CURRENT, current);
  Blynk.virtualWrite(VPIN_POWER, power);
  Blynk.virtualWrite(VPIN_KWH, kWh);

  delay(1000);
}
