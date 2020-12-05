#ifndef LightSensor_h
#define LightSensor_h

#include <Arduino.h>

// Modi to set with setLightSensorMode
#define LIGHTSENSOR_MODE_OUTDOORS  0
#define LIGHTSENSOR_MODE_INDOORS   1
extern void setLightSensorMode(int mode);

typedef enum 
{
  Dark = 0,
  Shadow = 1,
  Sunny = 2
} LightSensorState;

class LightSensor {
public:
  int pin;  // the analog pin to read raw values
  int raw;  // the last raw value read
  LightSensorState state; // The light state to the ow value
  // Sets the analog pin to read rwa values
  void setup(int analogPin);
  // reads the next raw value from the pin and sets the sensor state
  int read();
  // gets the state for the last raw value read
  void getState();
};

#endif
