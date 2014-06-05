/*
  LoadImageFromSD
  Load an image located into 0x0000-0x0000 position over the display module
 
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
  Serial.println("Load Image form SD test program");

  display.begin(DISPLAY_ADDRESS);
  //wait for display initialization
  do {
    delay(50);
  }
  while(display.isReady() != 0x21);
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
  display.drawImage(0x0000, 0x0000, 50, 50);
  delay(1000); 
}
