#ifndef Settings_h
#define Settings_h

namespace Settings
{
  int serviceModePin = 3;
  int lightingModePin = 4;
  
  void setup(){
    pinMode(serviceModePin, INPUT);  
    pinMode(lightingModePin, INPUT);  
  }
  int serviceMode() {
    return digitalRead(serviceModePin) == LOW;
  }
  int lightingMode() {
    return digitalRead(lightingModePin) == LOW;
  }
}

#endif
