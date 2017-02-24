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

#include "SSD1306.h"
// Display Settings
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = 1;
const int SDC_PIN = 3;

const int sonar_front_trig = 13;
const int sonar_front_echo = 12;

//Компас
#include "HMC5883L.h"

//Двигатели
#include "L298N.h"

#include "web.h"

SSD1306  display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);

const char* ssid = "dir300";
const char* ap_ssid = "NaNa";
const char* password = "o2bukabaka";

//Ик датчкики препятствия
const int ir_left = 14;
const int ir_right = 16;


//Для позиционирования
unsigned long t = 0;

void setup()
{
  Serial.begin(115200);

  SPIFFS.begin();

  Wire.begin(1, 3);
  Wire.setClock(400000);

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
  //Для консоли через телнет
  //  ArduinoOTA.setPassword((const char *)"123");/

  pinMode (IN4, OUTPUT);
  pinMode (IN5, OUTPUT);
  pinMode (IN6, OUTPUT);
  pinMode (IN7, OUTPUT);

  pinMode(ir_left, INPUT);
  pinMode(ir_right, INPUT);

  web_setup();

  mag.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);


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

void echo(String text) {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.drawString(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 - 10, text);
  display.display();
}

boolean obstacle_left() {
  return digitalRead(ir_left) == HIGH;
}

boolean obstacle_right() {
  return digitalRead(ir_right) == HIGH;
}

void wsDebug() {
  int speed = getSpeed();
  float w = getHeading();

  sensors_event_t event;
  mag.getEvent(&event);

  String debug = "{\n";
  debug += "\"t\": " + String(millis()) + ",\n";
  debug += "\"w\": " + String(w) + ",\n";
  debug += "\"mag_x\": " + String(event.magnetic.x) + ",\n";
  debug += "\"mag_y\": " + String(event.magnetic.y) + ",\n";
  debug += "\"speed\": " + String(speed) + ",\n";
  debug += "\"dist_front\": " + String(dist_front()) + "\n";
  debug += "}";


  webSocket.broadcastTXT(debug);
}

void loop()
{
  ArduinoOTA.handle();

  web_server.handleClient();

  webSocket.loop();

  wsDebug();

  if (state == "auto") {
//    if (!(obstacle_left() || obstacle_right() || dist_front() < 30)) {
//      fwd();
//    } else {
//      select_course();
//    }

    if (!(obstacle_left() || obstacle_right() || dist_front() < 30 )) {
      fwd();
    } else {
      backwards();
      delay(100);
      select_course();
    }
  }

}
