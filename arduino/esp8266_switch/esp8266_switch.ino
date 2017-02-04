/*
   ESP8266 12-f
   Прошивка

   EN - 3.3 (всегда)
   VCC - 3.3
   GPIO0 - GND
   GPIO15 - GND (всегда)
   TX - RX
   RX - TX

   Последующие прошивки можно выполнять через OTA

*/


#include <Wire.h>
#include "SSD1306.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

//For OTA
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "images.h"

const char* ssid = "dir300";
const char* password = "o2bukabaka";

// Display Settings
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = 2;
const int SDC_PIN = 4;

const int RELAY = 16;

SSD1306  display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);

#define MAX_SRV_CLIENTS 1
WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

void setup() {
  Serial.begin(115200);

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

  ArduinoOTA.begin();
  ArduinoOTA.onStart([]() {
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display.drawString(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 - 10, "OTA Update");
    display.display();
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    display.drawProgressBar(4, 32, 120, 8, progress / (total / 100) );
    display.display();
  });

  ArduinoOTA.onEnd([]() {
    echo("Restart");
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
    display.drawString(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, "Restart");
    display.display();
  });

  server.begin();
  server.setNoDelay(true);

}

void loop() {
  ArduinoOTA.handle();

  handleTelnet();
}

void handleTelnet() {

  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()) {
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }

  //check clients for data
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      if (serverClients[i].available()) {
        while (serverClients[i].available()) {
          String command = serverClients[i].readStringUntil('\n');
          String response = handleCommand(command);

          serverClients[i].println(response);
        }


        delay(2000);
      }
    }
  }

}

String handleCommand(String text) {
  String response = "unknown command: " + text;

  if ( text.indexOf("help") == 0) {
    response = "help";
  } else if (text.indexOf("html24") == 0) {
    text.replace("html24","");
    echo24(text);
    response = "ok";
  } else if (text.indexOf("html16") == 0) {
    text.replace("html16","");
    echo16(text);
    response = "ok";
  }  else if (text.indexOf("html") == 0) {
    text.replace("html","");
    echo(text);
    response = "ok";
  }
  return response;
}

void echo(String text) {
  text.replace("<br>", "\n");
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.drawString(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, text);
  display.display();
}

void echo16(String text) {
  text.replace("<br>", "\n");
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.drawString(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, text);
  display.display();
}

void echo24(String text) {
  text.replace("<br>", "\n");
  display.clear();
  display.setFont(ArialMT_Plain_24);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.drawString(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, text);
  display.display();
}
