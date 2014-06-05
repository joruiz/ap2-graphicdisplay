#include "SoftwareSerial.h"
#include "Goldelox_Serial_4DLib.h"
#include "Goldelox_const4D.h"

#define TX_PIN         3
#define RX_PIN         4
#define RESET_PIN      5

//Variables
SoftwareSerial displaySerial(TX_PIN, RX_PIN); 
Goldelox_Serial_4DLib display(&displaySerial, RESET_PIN);

void setup() {

  Serial.begin(9600);
  Serial.println("Start display tes program");
  //init display
  displaySerial.begin(115200);  
  display.reset();
  display.TimeLimit4D = 2000;
  display.media_Init();
  display.gfx_Rectangle(3, 25, 33, 55, RED);
  display.gfx_Triangle(3, 25, 18, 4, 33, 25, BLUE);
}

void loop() {

  delay(10);
}
