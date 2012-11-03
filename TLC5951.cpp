#include "tlc5951.h"
#include <SPI.h>

void TLC5951::init(int gssin, int gssck, int gslat, int xblnk, int gsckrgb) {
	_gssin = gssin;
	_gssck = gssck;
	_gslat = gslat;
	_xblnk = xblnk;
	_gsckrgb = gsckrgb;
	
	pinMode(_gssin, OUTPUT);
	pinMode(_gssck, OUTPUT);
	pinMode(_gslat, OUTPUT);
	pinMode(_xblnk, OUTPUT);
	pinMode(_gsckrgb, OUTPUT);
	
	digitalWrite(_gssin, LOW);
	digitalWrite(_gssck, LOW);
	digitalWrite(_gslat, LOW);
	digitalWrite(_xblnk, LOW);
	analogWrite(_gsckrgb, 127);
	
	SPI.begin();
    SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
}

void TLC5951::init(int gssin, int gssck, int gslat, int xblnk, int gsckrgb, int grayscale) {
	_gssin = gssin;
	_gssck = gssck;
	_gslat = gslat;
	_xblnk = xblnk;
	_gsckrgb = gsckrgb;
	
	pinMode(_gssin, OUTPUT);
	pinMode(_gssck, OUTPUT);
	pinMode(_gslat, OUTPUT);
	pinMode(_xblnk, OUTPUT);
	pinMode(_gsckrgb, OUTPUT);
	
	digitalWrite(_gssin, LOW);
	digitalWrite(_gssck, LOW);
	digitalWrite(_gslat, LOW);
	digitalWrite(_xblnk, LOW);
	analogWrite(_gsckrgb, 127);
	
	setAllGSData(grayscale);
	
	SPI.begin();
    SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
}

void TLC5951::setAllGSData(int gsvalue) {
	for(int a = 0; a < 8; a++) {
		for(int b = 0; b < 3; b++) {
			_gsData[a][b] = gsvalue;
		}
	}
}

void TLC5951::updateGS() {
	/*digitalWrite(_xblnk, LOW);
	digitalWrite(_gslat, LOW);
	
	for(int a = 7; a >= 0; a--) {
		for(int b = 2; b >= 0; b--) {
			for(int c = 11; c >= 0; c--) {
				digitalWrite(_gssin, bitRead(_gsData[a][b], c));
				digitalWrite(_gssck, HIGH);
				digitalWrite(_gssck, LOW);
			}
		}
	}
	
	digitalWrite(_gslat, HIGH);
	digitalWrite(_gslat, LOW);
	
	digitalWrite(_xblnk, HIGH);*/
	
	byte buffer = 0;	
	int index = 7;
	
	digitalWrite(_xblnk, LOW);
	digitalWrite(_gslat, LOW);
	
	for(int a = 7; a >= 0; a--) {
		for(int b = 2; b >= 0; b--) {
			for(int c = 11; c >= 0; c--) {
				if(index == -1)
				{
					SPI.transfer(buffer);
					index = 7;
				}
				bitWrite(buffer, index, bitRead(_gsData[a][b], c));
				index--;
				if(a == 0 && b == 0 && c == 0)
					SPI.transfer(buffer);		
			}
		}
	}
	
	digitalWrite(_gslat, HIGH);
	digitalWrite(_gslat, LOW);
	
	digitalWrite(_xblnk, HIGH);
}

void TLC5951::setLED(unsigned int led, unsigned int red, unsigned int green, unsigned int blue) {
	_gsData[led][2] = blue;
	_gsData[led][1] = green;
	_gsData[led][0] = red;
}

void TLC5951::setFunctionData(byte data) {
	_functionData = data;
}

void TLC5951::setBrightness(byte red, byte green, byte blue) {
	_brightRed = red;
	_brightGreen = green;
	_brightBlue = blue;
}

void TLC5951::setAllDCData(int dcvalue) {
	for(int a = 0; a < 8; a++) {
		for(int b = 0; b < 3; b++) {
			_dcData[a][b] = dcvalue;
		}
	}
}


void TLC5951::updateControl() {
	byte buffer = 0;
	int index = 0;
		
	digitalWrite(_xblnk, LOW);
	digitalWrite(_gslat, HIGH);	
		
	// dont care about these bits anyway
	for(int i = 0; i < 11; i++)
	{
		SPI.transfer(buffer);
	}
	bitWrite(buffer, 7, 0);
	bitWrite(buffer, 6, 0);
	
	// Function data
	for(int i = 6; i >= 1; i--) {
		bitWrite(buffer, i-1, bitRead(_functionData, i));
	}
	SPI.transfer(buffer);
	bitWrite(buffer, 7, bitRead(_functionData, 0));
	
	// Blue Brightness
	for(int i = 7; i >= 1; i--) {
		bitWrite(buffer, i-1, bitRead(_brightBlue, i));
	}
	SPI.transfer(buffer);
	bitWrite(buffer, 7, bitRead(_brightBlue, 0));
	
	// Green Brightness
	for(int i = 7; i >= 1; i--) {
		bitWrite(buffer, i-1, bitRead(_brightGreen, i));
	}
	SPI.transfer(buffer);
	bitWrite(buffer, 7, bitRead(_brightGreen, 0));
	
	// Red Brightness
	for(int i = 7; i >= 1; i--) {
		bitWrite(buffer, i-1, bitRead(_brightRed, i));
	}
	SPI.transfer(buffer);
	bitWrite(buffer, 7, bitRead(_brightRed, 0));
	
	index = 6;
	// Dot Correctness data
	for(int a = 7; a >= 0; a--) {
		for(int b = 2; b >= 0; b--) {
			for(int c = 6; c >= 0; c--) {
				if(index == -1)
				{
					SPI.transfer(buffer);
					index = 7;
				}
				bitWrite(buffer, index, bitRead(_dcData[a][b], c));
				index--;	
				if(a == 0 && b == 0 && c == 0)
					SPI.transfer(buffer);
				
			}
		}
	}
	digitalWrite(_gslat, LOW);
	delayMicroseconds(10);
	
	digitalWrite(_gslat, HIGH);
	digitalWrite(_gslat, LOW);
	
	digitalWrite(_xblnk, HIGH);		
}

void TLC5951::update() {
	updateGS();
	updateControl();
}