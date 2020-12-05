//
//
// Suntracker: Track the sun position using servo motors that follow a sun light sensor.
// A sun tracker (and more) build form (mostly) I2C components.
//
//
// build from components
// * Motion: provides a pan tilt movement of the platform
// * Sensor: provides information if the sun shines (directly) on it.
// * Compass: provides information of the horizontal rotation of the platform
// * Clock: provides information of the local date and time.
// * Display: shows information of the components and the processing.
// * Daylight: provides information if it is day or night
//
// additional components to add
// * TODO: Location: adds geographic location (length, width)
// *                 Wels: 14.1614, 48.0166
// * TODO: Tracking: calculates the position at time and location 
// * TODO: Follower reads data (attitude, azimute, time) from SD card
//

// servo motion part to move the platform
#include "Motion.h"

// sensor to get the position (direction) of the sun
#include "Sensor.h"

// sensor to check if it is day - we can run - or night - we wait for sun rise.
#include "Daylight.h"

// TODO: add LedButton(buttonPin, ledPin)
// TODO: add OnOffButton : LedButton
// TODO: add HomeButton : LedButton

// TODO: Add an IRremote control

// A clock to calculate the position of the sun at date and time
#include "Clock.h"

// Add I2C display to show date / location / sun track curve
#include "Display.h"

// Compass sensor to adjust tracker body in east west direction 
#include "Compass.h"

#define STATE_INIT 0      // loop initializations
#define STATE_RUN 1       // track the sun position
#define STATE_SCAN 2      // scan for the sun
#define STATE_DELAY 3     // delay
#define STATE_READY 4     // show ready state of hardware
#define STATE_CLOCK 5     // show date and time
#define STATE_COMPASS 6   // show compass heading
#define STATE_DAYLIGHT 7  // show the status of the day or night sensor
int state = STATE_INIT;

#define VERSION "1.0 Azimuth/Altitude"

void setup() {
  Serial.begin(115200);
  // initialize the clock
  Clock::setup();
  // intialize the display
  Display::setup();  
  // initialize the servos to handle motion
  Motion::setup();
  // initialize the (analog) light sensor
  Sensor::trace = 1;
  Sensor::setup();
  // initialize the compass
  Compass::setup();
  // initialize the day night sensor
  Daylight::setup(Daylight::Indoors);
  // Just to know which program is running on my Arduino
  Serial.println(F("START " __FILE__ "\r\nVersion " VERSION " from " __DATE__ " at " __TIME__));
  // start the state machine
  state = STATE_INIT;
}

typedef enum {
  Moving = 0,
  OnTarget = 1,
  Failed = 2
} TrackerState;

// scans for the first sun position
// returns OnTarget if any light sensor is sunny.
// returns Moving if it was moved to a new position 
// returns Failed no position no was found
int actionScanning() {
  Serial.println(F("Scanning ..."));

  if (Motion::atWest()){
    return Failed;
  }

  // move from east to west
  if (! Sensor::sunny()) {
    Motion::toWest();
    Motion::waitCompleted(); // waits for the servo to reach the position
  } else {
    if (Motion::atNorth()){
      return Failed;
    }
    Motion::toNorth();
    Motion::waitCompleted(); // waits for the servo to reach the position
  }
  // left sensor light/dark
  Sensor::read();

  // evaluate
  if (Sensor::sunny() && Sensor::north_sunny()) {
   return OnTarget;
  }  else {
   return Moving;
  }
}

// Tracks the sun direction
// returns OnTarget if all light sensor are sunny.
// returns Moving if it was moved to a new position 
// returns Failed if no position was found
int actionTracking() {
  Serial.println(F("Tracking ..."));
  // get sensor light/dark
  Sensor::read();
  // move to the sun
  if (Sensor::only_east_in_dark()) {
    Motion::toEast();
    Motion::waitCompleted(); // waits for the servo to reach the position
    return Moving;
  } else if (Sensor::only_west_in_dark()) {
    Motion::toWest();
    Motion::waitCompleted(); // waits for the servo to reach the position
    return Moving;
  } else if (Sensor::only_north_in_dark()) {
    Motion::toNorth();
    Motion::waitCompleted(); // waits for the servo to reach the position
    return Moving;    
  } else if (Sensor::sunny() && Sensor::north_sunny()) {
    delay(500); // waits for the sun to move ...
    return OnTarget;
  } else {
    delay(500); // waits for some operation ...
    return Failed;
  }
}

// changes the state and dsiplays reltaed information
void actionChangeState(int nextState) {
  state = nextState;
  if (! Display::ready) return;
  if (state == STATE_INIT) {
    Display::showState("Initializing ..");
  } else if (state == STATE_SCAN) {
    Display::showState("Scanning ..");
    Display::show_chart();
  } else if (state == STATE_RUN) {
    Display::showState("Tracking ..");
    Display::show_chart();
  } else if (state == STATE_DELAY) {
    Display::showState("Waiting ..");
    delay(1000);
  } else if (state == STATE_READY) {
    Display::showState("Components ..");
  } else if (state == STATE_CLOCK) {
    Display::showState("Date and time ..");
  } else if (state == STATE_COMPASS) {
    Display::showState("Compass ..");
  } else if (state == STATE_DAYLIGHT) {
    Display::showState("Daylight ..");
  }
}

// displays ready information for the components attached.
void actionReady() {
  Display::show_ready("Clock", Clock::ready, 1);
  Display::show_ready("Display", Display::ready, 2);
  Display::show_ready("Sensor", Sensor::ready, 3);
  Display::show_ready("Motion", true, 4);
  Display::show_ready("Compass", Compass::ready, 5);
  delay(3000); // give some time to read
  Display::clear_area(1);
  Display::show_ready("Daylight", Daylight::ready, 1);
  delay(3000); // give some time to read
}

// Action to show the clock
void actionClock()
{
  Display::show_clock(Clock::year(), Clock::month(), Clock::day(), Clock::hour(), Clock::minute());
  delay(3000); // give some time to read
}

// Action to show the compass
void actionCompass()
{
  Display::show_compass("Rotation", Compass::angle());
  delay(3000); // give some time to read
}

// Action to show 
void actionDaylight()
{
  Display::show_daylight("It is", Daylight::day());
  delay(3000); // give some time to read
}

void loop() {
  if (state == STATE_INIT) {
    Motion::home();
    Serial.print(F("Clock: ")); Serial.println(Clock::ready ? F("ready") : F("off"));
    actionChangeState(STATE_READY);
  } else if (state == STATE_READY) {
    actionReady();
    actionChangeState(STATE_CLOCK);
  } else if (state == STATE_CLOCK) {
    actionClock();
    actionChangeState(STATE_COMPASS);
  } else if (state == STATE_COMPASS) {
    actionCompass();
    actionChangeState(STATE_DAYLIGHT);
  } else if (state == STATE_DAYLIGHT) {
    actionDaylight();
    // if at night wait for sun rise :)
    if (Daylight::day())
      actionChangeState(STATE_SCAN);
    else
      Serial.println(F("Waiting for sun rise ..."));
  } else if (state == STATE_SCAN) {
      TrackerState trackerState = actionScanning();
      if (trackerState == OnTarget)
        actionChangeState(STATE_RUN);
      else if (trackerState == Failed)
        actionChangeState(STATE_DELAY);
  } else if (state == STATE_RUN) {
      TrackerState trackerState = actionTracking();
      if (trackerState == Failed)
        actionChangeState(STATE_SCAN);
  } else if (state == STATE_DELAY) {
    delay(1000);
  }
}
