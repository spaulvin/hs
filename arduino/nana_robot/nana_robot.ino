//TX0                       RST
//RX0                       ADC
//GPIO5 (i2c sck)            EN
//GPIO4 (i2c sda)           GPIO16     right bumber
//GPIO0                     GPIO14     left bumber
//GPIO2                     GPIO12     sensor button
//GPI15  |                  GPIO13
//GND    |                  VCC


//For esp8266 12f set flash size 4m and 1m spiffs/

#include <Wire.h>
#include <ESP8266WiFi.h>

#include "FS.h"

//For OTA
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

#include <WebSocketsServer.h>
#include <Hash.h>

WebSocketsServer webSocket = WebSocketsServer(81);

#include "Adafruit_PWMServoDriver.h"
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#include "SSD1306.h"
// Display Settings
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = 4;
const int SDC_PIN = 5;

SSD1306  display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);

void echo(String text) {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.drawString(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 - 10, text);
  display.display();
}

//Бампер
const int bumper_pin = 16;

#include "NANA.h"

NANA nana;

const int sonar_front_trig = 13;
const int sonar_front_echo = 12;

#include "web.h"

#include "MPU6050.h"

const char* ssid = "dir300";
const char* ap_ssid = "NaNa";
const char* password = "o2bukabaka";

void setup()
{
  Serial.begin(115200);

  SPIFFS.begin();

  Wire.begin(SDA_PIN, SDC_PIN);
  Wire.setClock(400000);

  pwm.begin();
  pwm.setPWMFreq(1600);

  nana.setup();

  pinMode(sonar_front_trig, OUTPUT);
  pinMode(sonar_front_echo, INPUT);

  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  WiFi.mode(WIFI_AP_STA);

  WiFi.softAP(ap_ssid, password);

  IPAddress myIP = WiFi.softAPIP();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    echo("WiFi");
  }

  ArduinoOTA.onStart([]() {

  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {

  });

  ArduinoOTA.onEnd([]() {

  });

  ArduinoOTA.begin();

  web_setup();

  mag.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  echo("NaNa v1");

  pinMode(A0, INPUT);

  nana.setDriveMode(1);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      //            USE_SERIAL.printf("[%u] Disconnected!\n", num);/
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        //                USE_SERIAL.printf("[%u] Connected from %d.%d.%/d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      //            USE_SERIAL.printf("[%u] get Text: %s\n", num, payload/);

      // send message to client
      // webSocket.sendTXT(num, "message here");

      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      //            USE_SERIAL.printf("[%u] get binary lenght: %u\n", num, l/enght);
      hexdump(payload, lenght);

      // send message to client
      // webSocket.sendBIN(num, payload, lenght);
      break;
  }

}

void loop()
{
  ArduinoOTA.handle();

  web_server.handleClient();

  webSocket.loop();

  nana.loop();

}
