/*
 Arduino Library for I2C display peripheral.
 It is based on the 4D Systems uOLED-160-G2 Module and 5 push buttons
 go to http://www.4dsystems.com.au/product/4D_Intelligent_Display_Modules/uOLED_160_G2/ for more information
 
 License: CC BY-SA 3.0: Creative Commons Share-alike 3.0. Feel free 
 to use and abuse this code however you'd like. If you find it useful
 please attribute, and SHARE-ALIKE!
 
 Created April 2014
 by Jonathan Ruiz de Garibay
*/ 

#include "Arduino.h"
#include "GraphicDisplay.h"
#include <Wire.h>

//
// begin
//
// Init I2C bus
void GraphicDisplay::begin(uint8_t address){

	_address = address;
	_buttonsChangePin = 0xFF;
	Wire.begin();
}

void GraphicDisplay::begin(uint8_t address, uint8_t buttonsChangePin){

	_address = address;
	_buttonsChangePin = buttonsChangePin;
	pinMode(_buttonsChangePin, INPUT);
	Wire.begin();
}

//
// getVersion
//
// Get firmware version
uint8_t GraphicDisplay::getVersion(){

	Wire.beginTransmission(_address);
    Wire.write(CMD_GETVERSION);
    Wire.endTransmission();
	//wait for operation
    delay(5);
    //request version value
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1) {
		return Wire.read();
	}
	return 0xFF;
}

//
// idReady
//
// Get display module state
boolean GraphicDisplay::isReady(){

	Wire.beginTransmission(_address);
    Wire.write(CMD_READY);
    Wire.endTransmission();
	//wait for operation
    delay(5);
    //request version value
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1) {
		return Wire.read() == 0x21;
	}
	return false;
}

uint8_t GraphicDisplay::clear(){

	Wire.beginTransmission(_address);
    Wire.write(CMD_CLEAR);
	Wire.endTransmission();
	//wait for operation
    delay(100);
    //request version value
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1) {
		return Wire.read();
	}
	return 0xFF;
}

uint8_t GraphicDisplay::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t colour){

	Wire.beginTransmission(_address);
    Wire.write(CMD_DRAWLINE);
	Wire.write(x1);
	Wire.write(y1);
	Wire.write(x2);
	Wire.write(y2);
    Wire.write(colour >> 8);
    Wire.write(colour & 0x00FF);
    Wire.endTransmission();
	//wait for operation
    delay(5);
    //request version value
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1) {
		return Wire.read();
	}
	return 0xFF;
}

uint8_t GraphicDisplay::drawRectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t colour){

	Wire.beginTransmission(_address);
    Wire.write(CMD_DRAWRECTANGLE);
	Wire.write(x1);
	Wire.write(y1);
	Wire.write(x2);
	Wire.write(y2);
    Wire.write(colour >> 8);
    Wire.write(colour & 0x00FF);
    Wire.endTransmission();
	//wait for operation
    delay(5);
    //request version value
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1) {
		return Wire.read();
	}
	return 0xFF;
}

uint8_t GraphicDisplay::drawCircle(uint8_t x1, uint8_t y1, uint8_t radius, uint16_t colour){

	Wire.beginTransmission(_address);
    Wire.write(CMD_DRAWCIRCLE);
	Wire.write(x1);
	Wire.write(y1);
	Wire.write(radius);
	Wire.write(colour >> 8);
    Wire.write(colour & 0x00FF);
    Wire.endTransmission();
	//wait for operation
    delay(5);
    //request version value
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1) {
		return Wire.read();
	}
	return 0xFF;
}

uint8_t GraphicDisplay::drawFillCircle(uint8_t x1, uint8_t y1, uint8_t radius, uint16_t colour){

	Wire.beginTransmission(_address);
    Wire.write(CMD_DRAWFILLCIRCLE);
	Wire.write(x1);
	Wire.write(y1);
	Wire.write(radius);
	Wire.write(colour >> 8);
    Wire.write(colour & 0x00FF);
    Wire.endTransmission();
	//wait for operation
    delay(5);
    //request version value
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1) {
		return Wire.read();
	}
	return 0xFF;
}

uint8_t GraphicDisplay::drawImage(uint16_t dirH, uint16_t dirL, uint8_t x, uint8_t y){

	Wire.beginTransmission(_address);
    Wire.write(CMD_DRAWIMAGE);
    Wire.write(dirH >> 8);
    Wire.write(dirH & 0x00FF);
    Wire.write(dirL >> 8);
    Wire.write(dirL & 0x00FF);
	Wire.write(x);
	Wire.write(y);
	Wire.endTransmission();
	//wait for operation
    delay(5);
    //request version value
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1) {
		return Wire.read();
	}
	return 0xFF;
}

uint8_t GraphicDisplay::printText(String text, uint8_t line, uint8_t column, uint16_t colour) {

	Wire.beginTransmission(_address);
    Wire.write(CMD_PRINTTEXT);
	Wire.write(line);
	Wire.write(column);
    Wire.write(colour >> 8);
    Wire.write(colour & 0x00FF);
	for(int i = 0; i < text.length(); i++)
		Wire.write(text[i]);
	Wire.endTransmission();
	//wait for operation
    delay(5);
    //request version value
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1) {
		return Wire.read();
	}
	return 0xFF;
}

boolean GraphicDisplay::buttonsChanged() {

	if (_buttonsChangePin == 0xFF)
		return false;
	else
		return (digitalRead(_buttonsChangePin) == LOW);
}


uint8_t GraphicDisplay::getButtons() {
	
	Wire.beginTransmission(_address);
    Wire.write(CMD_GETBUTTONS);
	Wire.endTransmission();
	//wait for operation
    delay(5);
    //request version value
    Wire.requestFrom((int)_address, 1);
    if (Wire.available() == 1) {
		return Wire.read();
	}
	return 0xFF;
}