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
    // Serial.print(F("Heading: ")); Serial.print(heading); Serial.println(F("°"));
   // move get the value in east west
    return heading < 180 ? heading : heading - 360;
  }
}

#endif
