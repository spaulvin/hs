#include "OneWire.h"
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include "user_interface.h"

os_timer_t myTimer;

IPAddress zabbix(192, 168, 88, 249);

const char* ssid     = "dir300";
const char* password = "o2bukabaka";

OneWire  ds(2);  // on pin 10 (a 4.7K resistor is necessary)

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

//  os_timer_setfn(&myTimer, timerCallback, NULL);
//
//  os_timer_arm(&myTimer, 30 * 1000, true);
}

//void timerCallback(void *pArg) {
void timerCallback() {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  float celsius;

  int sensor_num = 0;

  ds.reset_search();
  while (ds.search(addr)) {
    sensor_num++;

    String mac = mac2String(addr);

    Serial.print("mac: ");
    Serial.println(mac);

    Serial.print("num: ");
    Serial.println(sensor_num);

    // the first ROM byte indicates which chip
    if (addr[0] != 0x28) {
      Serial.print("mac: ");
      Serial.println("Temp sensor is not DS18B20");
      return;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end

    delay(1000);     // maybe 750ms is enough, maybe not
    // we might do a ds.depower() here, but the reset will take care of it.

    present = ds.reset();
    ds.select(addr);
    ds.write(0xBE);         // Read Scratchpad

    for ( i = 0; i < 9; i++) {           // we need 9 bytes
      data[i] = ds.read();
    }

    // Convert the data to actual temperature
    // because the result is a 16 bit signed integer, it should
    // be stored to an "int16_t" type, which is always 16 bits
    // even when compiled on a 32 bit processor.
    int16_t raw = (data[1] << 8) | data[0];

    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time

    celsius = (float)raw / 16.0;

    Serial.print("t = ");
    Serial.println(celsius);
    sendzabbix(celsius, sensor_num);
  }
}

void loop(void) {
  ArduinoOTA.handle();
  timerCallback();
  delay(30*1000);
}

String mac2String(byte ar[])
{
  char macAddr[12];
  sprintf(macAddr, "%2X%2X%2X%2X%2X%2X", ar[0], ar[1], ar[2], ar[3], ar[4], ar[5]);
  return String(macAddr);
}

void sendzabbix(float temp, int sensor_num)
{
  WiFiClient zbx_client;
  if (zbx_client.connect(zabbix, 10051))
  {
    String msg = "{\"request\":\"agent data\",\"data\":[{\"host\":\"boiler\",\"key\":\"sensor";
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
