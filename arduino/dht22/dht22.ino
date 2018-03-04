#include "OneWire.h"
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include "user_interface.h"

#include "DHT.h"

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// DHT Sensor
const int DHTPin = 2;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];

os_timer_t myTimer;

bool timerTick = true;

IPAddress zabbix(192, 168, 88, 249);

const char* ssid     = "dir300";
const char* password = "o2bukabaka";

void setup(void) {
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  ArduinoOTA.onStart([]() {});
  ArduinoOTA.onEnd([]() { });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {  });
  ArduinoOTA.onError([](ota_error_t error) {});
  ArduinoOTA.begin();

  os_timer_setfn(&myTimer, timerCallback, NULL);

  os_timer_arm(&myTimer, 30 * 1000, true);
}

void timerCallback(void *pArg) {
  timerTick = true;
}

void send() {
  if (timerTick) {
    float celsius;

    int sensor_num = 1;

    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
    }

    //Температура восприятия, зависит от влажности
    //  float hic = dht.computeHeatIndex(t, h, false);/

    sendzabbix(t, sensor_num);
    sensor_num++;

    sendzabbix(h, sensor_num);

    timerTick = false;
  }
}

void loop(void) {
  ArduinoOTA.handle();
  send();
}

void sendzabbix(float temp, int sensor_num)
{
  WiFiClient zbx_client;
  if (zbx_client.connect(zabbix, 10051))
  {
    String msg = "{\"request\":\"agent data\",\"data\":[{\"host\":\"meteo\",\"key\":\"sensor";
    msg += sensor_num;
    msg += "\",\"value\":\"";
    msg += temp;
    msg += "\"}]}\r\n";
    Serial.println(msg);
    zbx_client.write(msg.c_str());
    delay(1);
    zbx_client.stop();
  } else {
    Serial.println("cant connect zabbix");
  }
}
