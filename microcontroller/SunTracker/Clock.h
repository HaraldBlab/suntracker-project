#ifndef Clock_h
#define Clock_h

#include <Arduino.h>
#include "RTClib.h"

/*
 *  A sensor to provide date and time information.
 */
namespace Clock {

  RTC_DS1307 rtc;
  
  /* Allows you to check if the clock is available */
  int ready = false;
  
  /* Setup the clock sensor */
  void setup() {
   if (! rtc.begin()) {
     Serial.println(F("Couldn't find RTC"));
   }
   else if (! rtc.isrunning()) {
     Serial.println(F("RTC is NOT running!"));
     // Sets the RTC to the date & time this sketch was compiled
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
   } else
     // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    ready = true;
  }
  /* Gets the year */
  uint16_t year() { return rtc.now().year(); }
  /* Gets the month */
  uint8_t month() { return rtc.now().month(); }
  /* Gets the day */
  uint8_t day() { return rtc.now().day(); }
  /* Gets the hour */
  uint8_t hour() { return rtc.now().hour(); }
    /* Gets the minute */
  uint8_t minute() { return rtc.now().minute(); } 
};

#endif
