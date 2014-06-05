#include <Wire.h>
#include "SoftwareSerial.h"
#include "Goldelox_Serial_4DLib.h"
#include "Goldelox_const4D.h"
#include "Firmware.h"

//Firmware version
#define VERSION        0
//I2C address
#define I2C_ADDRESS  0x51
//IO pins
#define BUTTON_RED     A2
#define BUTTON_BLUE    A1
#define BUTTON_GREEN   A0
#define BUTTON_UP      6
#define BUTTON_DOWN    9
#define TX_PIN         3
#define RX_PIN         4
#define RESET_PIN      5
#define I2C_WARNING    A3
#define I2C_TRANSFER   7
#define BUTTON_EVENT   8

//#define DEBUG
#ifdef DEBUG
//#define COMMAND_DEBUG
//#define OPERATION_TIME
#endif

//Variables
SoftwareSerial displaySerial(TX_PIN, RX_PIN); 
Goldelox_Serial_4DLib display(&displaySerial, RESET_PIN);
boolean executeCommand = false;
boolean isReady = false;
byte command = 0xFF;
byte reply = 0xFF;

int x1, y1, x2, y2, radius, line, column;
unsigned int colour, dirH, dirL;
String text = "";

const byte BUTTON_PINS[5] = {
  (int)BUTTON_RED, (int)BUTTON_BLUE, (int)BUTTON_GREEN, BUTTON_UP, BUTTON_DOWN};
byte buttonStatus[5] = {
  HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long prevTime = 0x00;

void setup() {

#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Start I2C display program");
#endif

  //init IO pins
  pinMode(I2C_WARNING, OUTPUT);
  digitalWrite(I2C_WARNING, HIGH);
  pinMode(I2C_TRANSFER, OUTPUT);
  digitalWrite(I2C_TRANSFER, LOW);
  pinMode(BUTTON_EVENT, OUTPUT);
  digitalWrite(BUTTON_EVENT, LOW);
  for (int i = 0; i < 5; i++)
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  //init I2C bus
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);   //reception event
  Wire.onRequest(requestEvent);   //request event
  //init display
  displaySerial.begin(115200);  
  display.reset();
  display.TimeLimit4D = 2000;
  display.media_Init();
  isReady = true;
}

void loop() {

  //execute display commands
  if (executeCommand) {
#ifdef OPERATION_TIME
    long opTime = micros();
#endif
    switch(command) {
    case CMD_DRAWLINE:    //draw line
      display.gfx_Line(x1, y1, x2, y2, colour);
      break;
    case CMD_DRAWRECTANGLE:    //draw rectangle
      display.gfx_Rectangle(x1,y1,x2,y2,colour);
      break;
    case CMD_DRAWIMAGE:    //draw image from sd
      display.media_SetSector(dirH, dirL);
      display.media_Image(x1, y1);
      break;
    case CMD_CLEAR:    //clear display
      display.gfx_Cls() ;
      break;
    case CMD_PRINTTEXT:  //print text
      display.txt_Opacity(TRANSPARENT);
      display.txt_MoveCursor(line, column);
      display.txt_FGcolour(colour);
      display.putstr(&text[0]);
      text = "";
      break;
    case CMD_DRAWCIRCLE:
      display.gfx_Circle(x1, y1, radius, colour);
      break;
    case CMD_DRAWFILLCIRCLE:
      display.gfx_CircleFilled(x1, y1, radius, colour);
      break;
    }
    executeCommand = false; 
#ifdef OPERATION_TIME
    opTime = micros() - opTime;
    Serial.print("Tiempo de operación: ");
    Serial.print(opTime, DEC);
    Serial.println(" microsegundos");
#endif
  }

  if ((millis() - prevTime) > 25) {
    prevTime = millis();
    //check button status
    for (int i = 0; i < 5; i++){
      byte state = digitalRead(BUTTON_PINS[i]);
      if (state != buttonStatus[i]) {
        digitalWrite(I2C_WARNING, LOW);
        digitalWrite(BUTTON_EVENT, HIGH);
#ifdef DEBUG
        Serial.print("Buttons changed: ");
        Serial.println(i, DEC);
#endif
      }
      buttonStatus[i] = state;
    }
  }
}

//
// receiveEvent
//
// This function is executed when the I2C master sends data
//
void receiveEvent(int howMany) {

  //I2C LED on
  digitalWrite(I2C_TRANSFER, HIGH);
  //check if Arduino can accept commands
  //get commmand and set default state for request
  command = Wire.read();
  reply = 0x4F;
  //execute command
  switch (command)
  {
  case CMD_GETVERSION:  //get version
    //no operation, request set firmware version
    break;
  case CMD_READY:  //ready
    //no operation, request module state
    break;
  case CMD_GETBUTTONS:  //get buttons
    //mount reply
    digitalWrite(BUTTON_EVENT, LOW);
    reply = 0x06;
    for (int i = 0; i < 5; i++) {
      reply = (reply << 1) | buttonStatus[i];
    }
    break;
  case CMD_DRAWLINE:  //draw line
    if (!executeCommand)
    {
      x1 = Wire.read();
      y1 = Wire.read();
      x2 = Wire.read();
      y2 = Wire.read();
      colour = (Wire.read() << 8) + Wire.read();
      executeCommand = true;
      reply = 0x4F;
    }
    else
      reply = 0x40;
    break;
  case CMD_DRAWRECTANGLE:  //draw rectangle
    if (!executeCommand)
    {
      x1 = Wire.read();
      y1 = Wire.read();
      x2 = Wire.read();
      y2 = Wire.read();
      colour = (Wire.read() << 8) + Wire.read();
      executeCommand = true;
      reply = 0x4F;
    }
    else
      reply = 0x40;
    break;
  case CMD_DRAWIMAGE:  //draw image from sd
    if (!executeCommand)
    {
      dirH = (Wire.read() << 8) + Wire.read();
      dirL = (Wire.read() << 8) + Wire.read();
      x1 = Wire.read();
      y1 = Wire.read();
      executeCommand = true;
      reply = 0x4F;
    }
    else
      reply = 0x40;
    break;
  case CMD_CLEAR:  //clear screen
    if (!executeCommand)
    {
      executeCommand = true;
      reply = 0x4F;
    }
    else
      reply = 0x40;
    break;    
  case CMD_PRINTTEXT:
    if (!executeCommand)
    {
      line = Wire.read();
      column = Wire.read();
      colour = (Wire.read() << 8) + Wire.read();
      for (int i = 4; i < howMany; i++)
        text += (char)Wire.read();
      executeCommand = true;
      reply = 0x4F;
    }
    else
      reply = 0x40;
    break;    
  case CMD_DRAWCIRCLE:
  case CMD_DRAWFILLCIRCLE:
    if (!executeCommand)
    {
      x1 = Wire.read();
      y1 = Wire.read();
      radius = Wire.read();
      colour = (Wire.read() << 8) + Wire.read();
      executeCommand = true;
      reply = 0x4F;
    }
    else
      reply = 0x40;
    break;    
  }
  //clear I2C buffer
  while (Wire.available() != 0)
    Wire.read();
}

//
// requestEvent
//
// This function is executed when the I2C master request for data
// Request depend on the last command received
//
void requestEvent() {

  //return a reply depends on the command
  switch (command)
  {
  case CMD_GETVERSION:  //get version
    //return firmware version
    Wire.write(0x10 + VERSION);
    break;
  case CMD_READY:  //Ready
    //return module state
    Wire.write(0x20 | (isReady && !executeCommand));
    break;
  case CMD_GETBUTTONS:  //get button status
    Wire.write(reply);
    digitalWrite(I2C_WARNING, HIGH);
    break;
  case CMD_DRAWLINE:       //draw line
  case CMD_DRAWRECTANGLE:  //draw rectangle
  case CMD_DRAWIMAGE:      //draw image from sd
  case CMD_CLEAR:          //clear screen
  case CMD_PRINTTEXT:
  case CMD_DRAWCIRCLE:
  case CMD_DRAWFILLCIRCLE:
    Wire.write(reply);
    break;
  }
  reply = 0xFF;
  //I2C LED off
  digitalWrite(I2C_TRANSFER, LOW);
}





















