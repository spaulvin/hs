#include <Wire.h>
#include <ESP8266WiFi.h>

//For OTA
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

#include "SSD1306.h"
// Display Settings
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDA_PIN = 1;
const int SDC_PIN = 3;

SSD1306  display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);

const char* ssid = "dir300";
const char* password = "o2bukabaka";

#define MAX_SRV_CLIENTS 1
WiFiServer server(23);
WiFiClient serverClients[MAX_SRV_CLIENTS];

ESP8266WebServer web_server(80);

const int close_dist = 40;

//Правый двиг
int IN4 = 2;
int IN5 = 0;

//Левый двиг
int IN6 = 4;
int IN7 = 5;

//Левый сенсор
const int echoPin = 13;
const int trigPin = 12;

//Правый сенсор
const int rightEchoPin = 14;
const int rightTrigPin = 16;


const int near = 40;
const int far = 100;

char hellostr[] =   "<html>"
                    "<body>"
                    "<script>"
                    "    xhr = new XMLHttpRequest();"
                    "    document.onkeydown = function(e) {"
                    "        xhr.open('GET', e.keyCode, true);"
                    "        xhr.send();"
                    "    }"
                    "</script>"
                    "Для управления:<br>"
                    "left - влево<br>"
                    "right - вправо<br>"
                    "up - вперед<br>"
                    "down - назад<br>"
                    "space - старт/стоп<br>"
                    "a - автопилот<br>"
                    "</body>"
                    "</html>";

String state = "stopped";

void setup()
{
  Serial.begin(115200);

  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    echo("WiFi");
  }

  ArduinoOTA.begin();
  ArduinoOTA.onStart([]() {

  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {

  });

  ArduinoOTA.onEnd([]() {

  });

  server.begin();
  server.setNoDelay(true);

  pinMode (IN4, OUTPUT);
  pinMode (IN5, OUTPUT);
  pinMode (IN6, OUTPUT);
  pinMode (IN7, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(rightTrigPin, OUTPUT);
  pinMode(rightEchoPin, INPUT);

  web_server.on("/", handleRoot);

  web_server.on("/32", space);

  web_server.on("/37", left);
  web_server.on("/38", up);
  web_server.on("/39", right);
  web_server.on("/40", down);

  web_server.on("/65", a);

  web_server.begin();
}

void echo(String text) {
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display.drawString(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 - 10, text);
  display.display();
}

void a() {
  state = "auto";
}

void left() {
  state = "manual";
  turn_left();
  web_server.send(200, "text/html", "ok");
}

void right() {
  state = "manual";
  turn_right();
  web_server.send(200, "text/html", "ok");
}

void up() {
  state = "manual";
  fwd();
  web_server.send(200, "text/html", "ok");
}

void down() {
  state = "manual";
  backwards();
  web_server.send(200, "text/html", "ok");
}

void space() {
  if (state == "stopped") {
    state = "manual";
    fwd();
  } else {
    st();
    state = "stopped";
  }
  web_server.send(200, "text/html", "ok");
}

void loop()
{
  ArduinoOTA.handle();

  web_server.handleClient();

  handleTelnet();

  if (state == "auto") {
    if (dist_left() < close_dist && dist_right() < close_dist) {
      backwards();
    } else {
      if (dist_left() < close_dist) {
        right_stop();
      } else {
        right_fwd();
      }

      if (dist_right() < close_dist) {
        left_stop();
      } else {
        left_fwd();
      }
    }

  }

}

void handleRoot() {
  web_server.send(200, "text/html", hellostr);
}

long dist_left() {
  long duration;
  int dist;


  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  dist = duration / 58.138;

  return dist;
}

long dist_right() {
  long duration;
  int dist;


  digitalWrite(rightTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(rightTrigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(rightTrigPin, LOW);

  duration = pulseIn(rightEchoPin, HIGH);
  dist = duration / 58.138;

  return dist;
}

void fwd() {
  left_fwd();
  right_fwd();
}
void st() {
  left_stop();
  right_stop();
}

void turn_left() {
  left_stop();
  right_fwd();
}

void turn_right() {
  right_stop();
  left_fwd();
}

void backwards() {
  right_back();
  left_back();
}

void turnaround_right() {
  right_fwd();
  left_back();
}

void turnaround_left() {
  right_back();
  left_fwd();
}



void right_fwd() {
  digitalWrite (IN4, HIGH);
  digitalWrite (IN5, LOW);
}

void right_stop() {
  digitalWrite (IN4, LOW);
  digitalWrite (IN5, LOW);
}

void right_back() {
  digitalWrite (IN4, LOW);
  digitalWrite (IN5, HIGH);
}

void left_fwd() {
  digitalWrite (IN6, HIGH);
  digitalWrite (IN7, LOW);
}

void left_stop() {
  digitalWrite (IN6, LOW);
  digitalWrite (IN7, LOW);
}

void left_back() {
  digitalWrite (IN6, LOW);
  digitalWrite (IN7, HIGH);
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
        serverClients[i].println("ping");
        while (serverClients[i].available()) {
          String command = serverClients[i].readStringUntil('\n');

          if (command == "fwd") {
            fwd();
          } else  if (command == "back") {
            backwards();
          } else  if (command == "stop") {
            st();
          }

          String response = "ok";

          serverClients[i].println(response);
        }
      }
    }
  }

}
