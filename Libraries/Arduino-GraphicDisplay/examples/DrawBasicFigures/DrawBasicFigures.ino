/*
  DrawBasicFigures
  Draw basic figures over the display module
 
  This example code is in the public domain.
  
  Connections:
    I2C bus connectios    
*/

#include <Wire.h>
#include <GraphicDisplay.h>

#define DISPLAY_ADDRESS  0x51

GraphicDisplay display;

void setup() {

  Serial.begin(9600);
  Serial.println("Draw Basic Figures test program");

  display.begin(DISPLAY_ADDRESS);
  //wait for display initialization
  do {
    delay(50);
  }
  while(!display.isReady());
  //get firmware version
  int ver = display.getVersion();
  switch(ver)
  {
  case 0x10:
    Serial.println("Version 1.0");
    break;
  }

}

void loop() {

  display.clear();
  display.drawLine(30, 40, 10, 100, BLUE);
  display.drawRectangle(50, 50, 100, 100, GREEN);
  display.drawCircle(80, 60, 40, RED);
  display.drawFillCircle(120, 80, 20, YELLOW);
  delay(1000);
}

