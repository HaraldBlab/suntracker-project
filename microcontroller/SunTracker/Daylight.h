#ifndef DAYLIGHT_H
#define DAYLIGHT_H

#include <Arduino.h>

namespace Daylight {
  // Modi to set with setLightSensorMode
  typedef enum {
    Outdoors = 0,
    Indoors = 1
  } Mode;

  int dayNightLimit = 500; // indoor: higher values are night, lower values are day
  int raw = 0; // the last raw value read
  int analogPin = A0;
  int trace = 0;   // if 1 the raw vaue reaing is traced to Serial
  int ready = false;
  void set_mode(Mode mode) {
    // indoor values are 10 times higher than outdoor values
    if (mode == Indoors)
      dayNightLimit = 500;
    else
      dayNightLimit = 55;
  }
  void setup(Mode mode=Outdoors) {
    set_mode(mode);
    pinMode(analogPin, INPUT);
    ready = true;
  }
  int read() {
    raw = analogRead(analogPin);
    if (trace) {
      Serial.print("Day-Night: "); Serial.println(raw);
    }
    return raw;
  }
  int night() {
    return read() > dayNightLimit;
  }
  int day() {
    return read() <= dayNightLimit;
  }
}
#endif
