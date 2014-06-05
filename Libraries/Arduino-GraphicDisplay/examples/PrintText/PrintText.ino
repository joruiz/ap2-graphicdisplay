/*
  PrintText
  Print a text over the dispaly module
 
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
  Serial.println("Print Text test program");

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

  display.clear();
  display.printText("Hello World!!!", 6, 1, BLUE);
}

void loop() {

  delay(10); 
}
