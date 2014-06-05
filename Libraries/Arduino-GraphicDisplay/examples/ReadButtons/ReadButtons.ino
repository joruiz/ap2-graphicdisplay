/*
  ReadButtons
  Read dispaly module buttons when one of them is pushed or released
 
  This example code is in the public domain.
  
  Connections:
    I2C bus connectios
    Button change pin to Arduino Pin 8
*/

#include <Wire.h>
#include <GraphicDisplay.h>

#define DISPLAY_ADDRESS  0x51
#define BUTTONS_CHANGE   8

GraphicDisplay display;

void setup() {

  Serial.begin(9600);
  Serial.println("Read Buttons test program");

  pinMode(BUTTONS_CHANGE, INPUT);
  display.begin(DISPLAY_ADDRESS, BUTTONS_CHANGE);

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
    Serial.println("Version: 1.0");
    break;
  }

}

void loop() {

  if (display.buttonsChanged()) {
    byte reply = display.getButtons();
    if ((reply & 0xE0) == 0xC0) {
      Serial.print("Reply = ");
      Serial.println(reply, BIN);
      if ((reply & 0x01) == 0x00)
        Serial.println("Button DOWN activated");
      if ((reply & 0x02) == 0x00)
        Serial.println("Button UP activated");
      if ((reply & 0x04) == 0x00)
        Serial.println("Button GREEN activated");
      if ((reply & 0x08) == 0x00)
        Serial.println("Button BLUE activated");
      if ((reply & 0x10) == 0x00)
        Serial.println("Button RED activated");
    }
  }
  delay(5); 
}
