#ifndef SunTrackerDisplay_h
#define SunTrackerDisplay_h

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SPLASH
#ifdef SPLASH
#include <splash.h>
#endif

namespace Display
{
  int ready = false;
  // OLED display width and height in pixels
  #define SCREEN_WIDTH  128
  #define SCREEN_HEIGHT 64

  // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
  #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
  Adafruit_SSD1306 oledDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  void setup() {
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      ready = false;
    } else {
      ready = true;
  #ifdef SPLASH
      oledDisplay.display();
      delay(2000); // Pause for 2 seconds
  #endif
    }
  }

  void showState(const char* msg) {
    if (! ready) return;
    if (msg == 0) return;
    
    oledDisplay.clearDisplay();
    oledDisplay.setTextSize(1); // Draw 2X-scale text
    oledDisplay.setTextColor(SSD1306_WHITE);
    oledDisplay.setCursor(10, 0);
    oledDisplay.println(msg);
    oledDisplay.display();      // Show initial text
  }

  void show_chart_border(int margin) {
    oledDisplay.drawRect(margin, margin, oledDisplay.width()- 2*margin, oledDisplay.height()-2*margin, SSD1306_WHITE);   
  }

  void show_hour_line(int tick, int margin) {
    int len = oledDisplay.height() - 2*margin;
    for (int i = 0; i < len; i += 4)
      oledDisplay.drawLine(tick, margin+i, tick, margin+i+2, SSD1306_WHITE);
  }

  void show_angle_line(int tick, int margin) {
    int len = oledDisplay.width() - 2*margin;
    for (int i = 0; i < len; i += 4)
      oledDisplay.drawLine(margin+i, tick, margin+i+2, tick, SSD1306_WHITE);    
  }
  
  void show_chart() {
    if (! ready) return;
  
    int margin = 10;
    show_chart_border(margin);
    int tick9h = margin + 27;
    int tick12h = margin + 27 + 27;
    int tick15h = oledDisplay.width()-margin-27;
    show_hour_line(tick9h, margin+3);
    show_hour_line(tick12h, margin+3);
    show_hour_line(tick15h, margin+3);
    int angle60 = margin+15;
    int angle30 = oledDisplay.height()-margin-15;
    show_angle_line(angle60, margin+3);
    show_angle_line(angle30, margin+3);
    oledDisplay.display();
    delay(1);
  }

  void show_ready(const char *msg, bool ready, int line=1) {
    if (! ready) return;
  
    oledDisplay.setTextSize(1); // Draw 2X-scale text
    oledDisplay.setTextColor(SSD1306_WHITE);
    oledDisplay.setCursor(10, 10*line);
    oledDisplay.print(msg);
    oledDisplay.setCursor(oledDisplay.width()/2, 10*line);
    oledDisplay.print(ready ? "ready" : "off");
    oledDisplay.display();
  }

  void show_clock(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute) {
    if (! ready) return;
  
    // clear the "active" area);
    oledDisplay.fillRect(0, 10, oledDisplay.width(), oledDisplay.height(), SSD1306_BLACK);
    // draw the information
    oledDisplay.setTextSize(1); 
    oledDisplay.setTextColor(SSD1306_WHITE);
    oledDisplay.setCursor(10, 10);
    oledDisplay.print(year); oledDisplay.print("/"); 
    oledDisplay.print(month); oledDisplay.print("/");
    oledDisplay.println(day);   
    oledDisplay.setCursor(10, 20);
    oledDisplay.print(hour); oledDisplay.print(":"); oledDisplay.println(minute);
    oledDisplay.display();
  }

  void show_compass(const char *msg, int angle) {
    if (! ready) return;
  
    // clear the "active" area);
    oledDisplay.fillRect(0, 10, oledDisplay.width(), oledDisplay.height(), SSD1306_BLACK);
    oledDisplay.setTextSize(1); 
    oledDisplay.setTextColor(SSD1306_WHITE);
    oledDisplay.setCursor(10, 10);
    oledDisplay.println(msg); 
    oledDisplay.setCursor(10, 20);
    oledDisplay.println(angle); 
    oledDisplay.display();
  }

  void show_daylight(const char *msg, int day) {
    if (! ready) return;
  
    // clear the "active" area);
    oledDisplay.fillRect(0, 10, oledDisplay.width(), oledDisplay.height(), SSD1306_BLACK);
    oledDisplay.setTextSize(1); 
    oledDisplay.setTextColor(SSD1306_WHITE);
    oledDisplay.setCursor(10, 10);
    oledDisplay.println(msg); 
    oledDisplay.setCursor(10, 20);
    oledDisplay.println(day ? "Day" : "Night"); 
    oledDisplay.display();
  }

  void clear_area(int display=0) {
    oledDisplay.fillRect(0, 10, oledDisplay.width(), oledDisplay.height(), SSD1306_BLACK);
    if (display)
      oledDisplay.display();
  }
};

#endif
