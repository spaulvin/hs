#include "OneWire.h"
OneWire  ds(2);  // on pin 10 (a 4.7K resistor is necessary)



void setup(void) {
  Serial.begin(9600);

  pinMode(A0, INPUT);
}

void loop(void) {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;

  ds.search(addr);

  // the first ROM byte indicates which chip
  if (addr[0] != 0x28) {
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

  int raw_v = analogRead(A0);
  Serial.println(raw_v);

  int Vin = 5;
  float K=2.88;
  float Vdrop = (Vin * raw_v) / 1024.0;
  int R1 = 1000;
  int Ra = 25;
  float TemperatureCoef = 0.019;
  //USA
  float PPMconversion = 0.7;
  float Rc = (Vdrop * R1) / (Vin - Vdrop);
  Rc = Rc - Ra; //acounting for Digital Pin Resitance
  float EC = 1000 / (Rc * K);


  //*************Compensating For Temperaure********************//
  float EC25  =  EC / (1 + TemperatureCoef * (celsius - 25.0));
  float ppm = (EC25) * (PPMconversion * 1000);
  Serial.println(ppm);
}
