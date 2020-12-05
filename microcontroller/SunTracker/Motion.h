#ifndef Motion_h
#define Motion_h

#include <Arduino.h>
#include <Servo.h>

namespace Motion
{

class MotionServo : public Servo
{
public:
  int pos = 0;        // variable to store the servo position
  int posstep = 5;    // single motor step
  int posstart = 0;   // motor from position
  int posend = 180;   // motor o  position
  int poshome = 180;  // motor home position
  int tracepos = 0;   // if 1 the position is traced to Serial
  MotionServo(int start,int end, int step,int home) {
    posstart = start;
    posend = end;
    posstep = step;
    poshome = home;
  }
  void home() {
    pos = poshome;
    write(pos); // tell servo to go to position in variable "pos"
    delay(25); // waits 15ms for the servo to reach the position
    if (tracepos) {
      Serial.print(F("* Home: ")); Serial.println(pos);
    }
  }
  void inc() {
    if (pos <= posend-posstep) {
      pos += posstep;
      if (tracepos) {
        Serial.print(F("* Move: ")); Serial.println(pos);
      }
      write(pos);
      delay(15);
    }
  }
  void dec() {
    if (pos >= posstart + posstep)
    {
      pos -= posstep;
      if (tracepos) {
        Serial.print(F("* Move: ")); Serial.println(pos);
      }
      write(pos);
      delay(15);
     }
  }
};

int servoPanPin = 3; // define the pin of servo signal line
int servoTiltPin = 9; // define the pin of servo signal line

// azimuth angle in xy plane
// from sun set to do sun down we have 180 deg
// from 6:00 to 18:00 we have 180 deg
// from 6:00 to 7:00 we have 15 deg
// from 6:00 to 6:20 we have 5 deg
// from 6:00 to 6:04 we have 1 deg
// for 4m we have deg
// home() is at 180 1 deg
// 4m is our best resolution
// 
// dec() moves from east to west
// inc() moves from west to east
#define EAST 180
#define WEST  0
#define WEST_EAST_STEP 5
#define WEST_EAST_HOME EAST
MotionServo azimuthServo(WEST, EAST, WEST_EAST_STEP, WEST_EAST_HOME);

// altidute angle to z direction
// dec() moves from north to south
// inc() moves from south west to north
#define NORTH 90
#define ECLIPSE  0
#define ECLIPSE_NORTH_STEP 2  // TODO: verify
#define ECLIPSE_NORTH_HOME ECLIPSE
MotionServo altitudeServo(ECLIPSE, NORTH, ECLIPSE_NORTH_STEP, ECLIPSE_NORTH_HOME);

int servoDelay = 500;

  void setup() {
    azimuthServo.tracepos = 1;
    azimuthServo.attach(servoPanPin);
    altitudeServo.tracepos = 1;
    altitudeServo.attach(servoTiltPin);
  }
  // check if we already at west
  int atWest() { return azimuthServo.pos == 0; }
  // move to west posstep degree
  void toWest() { azimuthServo.dec(); }
  // move to east posstep degree
  void toEast() { azimuthServo.inc(); }
  // check if we already at north
  int atNorth() { return altitudeServo.pos == NORTH; }
  // move to north posstep degree
  void toNorth() { altitudeServo.inc(); }
  // move to home position
  void home() {
    azimuthServo.home();
    altitudeServo.write(NORTH);
    delay(1000);
    altitudeServo.write(ECLIPSE);
    delay(1000);
  }
  void waitCompleted() { delay(servoDelay); }
}

#endif
