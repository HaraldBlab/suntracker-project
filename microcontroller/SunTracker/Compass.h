#ifndef Compass_h
#define Compass_h

#include <Arduino.h>
#include <QMC5883L.h>
namespace Compass
{
  QMC5883L _compass;
  int ready = false;
  void setup() {
    _compass.begin();
    ready = true;
  }
  int angle() {
    _compass.readData(); //read data from sensor
    int heading = _compass.heading(); // calculate the heading to north
    Serial.print(F("Heading: "));
    Serial.print(heading);
    Serial.println(F("°"));
    // TODO: value seems to be wrong.
    return heading + 90; // add 90 to get the value in east west
  }
}

#endif
