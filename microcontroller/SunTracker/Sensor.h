#ifndef Sensor_h
#define Sensor_h

#include <Arduino.h>
#include "LightSensor.h"

namespace Sensor {
  LightSensor leftSensor;   // the east (left) sensor
  LightSensor rightSensor;  // the west (right) sensor
  LightSensor topSensor;    // the north (top) sensor
  // TODO: pins from from right to left 
  int sensorLeftPin = A3;
  int sensorRightPin = A2;
  int sensorTopPin = A1;
  int trace = 0;
  bool ready = true;
  /* initilizes the (analog) sensor to use pins A2, A3 and A4 */
  void setup() {
    setLightSensorMode(LIGHTSENSOR_MODE_INDOORS);
    leftSensor.setup(sensorLeftPin);
    rightSensor.setup(sensorRightPin);      
    topSensor.setup(sensorTopPin);      
  }
  /* Read new sensor values */
  void read() {
    leftSensor.read();
    if (trace) {
      Serial.print(F("> Left: ")); Serial.print(leftSensor.raw);
      Serial.print(F(" = ")); Serial.print(leftSensor.state);
    }
    // right sensor light/dark
    rightSensor.read(); 
    if (trace) {
      Serial.print(F(" Right: ")); Serial.print(rightSensor.raw);
      Serial.print(F(" = ")); Serial.print(rightSensor.state);
    }
    // right sensor light/dark
    topSensor.read(); 
    if (trace) {
      Serial.print(F(" Top: ")); Serial.print(topSensor.raw);
      Serial.print(F(" = ")); Serial.print(topSensor.state);
      Serial.println();
    }
  }
  /* Checks if only the west sensor is lit by the sun */
  int only_east_in_dark() {
    return (rightSensor.state == Dark && leftSensor.state > Dark);
  }
  /* Checks if only the east sensor is lit by the sun. */
  int only_west_in_dark() {
    return (leftSensor.state == Dark && rightSensor.state > Dark); 
  }
  /* Checks if all sensors are lit by the sun */
  int sunny() {
    return (leftSensor.state == Sunny && rightSensor.state == Sunny);
  }
  int north_sunny() {
    return (leftSensor.state == Sunny && rightSensor.state == Sunny);
  }
  int only_north_in_dark() {
    return (topSensor.state == Dark); 
  }
}
#endif
