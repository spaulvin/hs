/*
   ESP8266 12-f
   Прошивка

   EN - 3.3 (всегда)
   VCC - 3.3
   GPIO0 - GND
   GPIO15 - GND (всегда)
   TX - RX
   RX - TX




*/


#include <Wire.h>
#include "SSD1306.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

#include "images.h"

const char* ssid = "dir300";
const char* password = "o2bukabaka";

// Display Settings
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = 2;
const int SDC_PIN = 4;

const int RELAY = 16;

SSD1306  display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);

  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
    display.display();
  }

  server.on("/on", []() {
    digitalWrite(RELAY, LOW);
    server.send(200, "text/html", "<a href='/off'>OFF</a>");

    display.clear();
    display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
    display.display();
  });

  server.on("/off", []() {
    digitalWrite(RELAY, HIGH);
    server.send(200, "text/html", "<a href='/on'>ON</a>");

    display.clear();
    display.drawXbm(34, 14, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
    display.display();
  });

  server.begin();
}

void loop() {

  server.handleClient();

  // clear the display
  display.clear();

  HTTPClient http;

  // Make a HTTP request:
  http.begin("http://192.168.88.246:81/temp.php");

  int httpCode = http.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.setFont(ArialMT_Plain_16);
      display.drawString(0, 0, payload);

      display.display();
    }
  } else {
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, http.errorToString(httpCode).c_str());

    display.display();
  }

  http.end();

  delay(1000);
}
