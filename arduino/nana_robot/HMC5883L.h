#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

float
getHeading() {
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);

  float heading = atan2(event.magnetic.y + 53.5, event.magnetic.x + 1.725);

  if (heading < 0)
    heading += 2 * PI;

  if (heading > 2 * PI)
    heading -= 2 * PI;

  float headingDegrees = heading * 180 / M_PI;

  Serial.println(headingDegrees);

  return headingDegrees;
}
