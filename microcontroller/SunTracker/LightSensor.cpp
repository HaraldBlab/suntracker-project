#include "LightSensor.h"


// raw values measured at analog pin (0..1023)
// valus measured on a sunny day
// "in doora" values are ten times higher 50, 150, 550
int lightSunnyRaw = 5;
int lightShadowRaw = 15;
int lightDarkRaw = 55;

void setLightSensorMode(int mode)
{
  if (mode == LIGHTSENSOR_MODE_INDOORS)
  {
    /* TODO: home table values */
    lightSunnyRaw = 400;
    lightShadowRaw = 600;
    lightDarkRaw = 800;

  } else {
    lightSunnyRaw = 5;
    lightShadowRaw = 15;
    lightDarkRaw = 55;    
  }
}

void LightSensor::setup(int analogPin) {
  pin = analogPin;
}
int LightSensor::read() {
 raw = analogRead(pin);
 delay(25);
 getState();
 return raw;
}
void LightSensor::getState() {
  if (raw < lightSunnyRaw)
    state = Sunny;
  else if (raw < lightShadowRaw)
    state = Shadow;
  else
    state = Dark;
         
}
