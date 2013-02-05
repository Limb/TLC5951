#include "tlc5951.h"
#include <SPI.h>

#define bit_set(p,m) ((p) |= (m)) 
#define bit_clear(p,m) ((p) &= ~(m)) 
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m)) 

void TLC5951::init(uint8_t gslat, uint8_t xblnk) {
	_gslat = gslat;
	_xblnk = xblnk;
	
	_bufferCount = 0;
	
	pinMode(_gslat, OUTPUT);
	pinMode(_xblnk, OUTPUT);
	
	digitalWrite(_gslat, LOW);
	digitalWrite(_xblnk, LOW);
}

void TLC5951::init(uint8_t gslat, uint8_t xblnk, uint16_t grayscale) {
	_gslat = gslat;
	_xblnk = xblnk;
	
	_bufferCount = 0;
	
	pinMode(_gslat, OUTPUT);
	pinMode(_xblnk, OUTPUT);

	digitalWrite(_gslat, LOW);
	digitalWrite(_xblnk, LOW);
	
	setAllGSData(grayscale);
}

void TLC5951::setAllGSData(uint16_t gsvalue) {
	for(int8_t a = 0; a < 8; a++) {
		for(int8_t b = 0; b < 3; b++) {
			_gsData[a][b] = gsvalue;
		}
	}
}

void TLC5951::updateGS() {
	_bufferCount = 0;
	
	//digitalWrite(_xblnk, LOW); // Turn off the LED's since we're clocking in data
	digitalWrite(_gslat, LOW); // GS Latch low, so it goes into the GS data latch
	
	for(int8_t a = 7; a >= 0; a--) { // We have 8 LED's. Start at the last since thats how we clock data out
		for(int8_t b = 2; b >= 0; b--) { // Each with 3 colors
			for(int8_t c = 11; c >= 0; c--) { // each consiting of 12 bits
				setBuffer((_gsData[a][b] & (1<<c)));
			}
		}
	}
	
	delay(1);
	
	digitalWrite(_gslat, HIGH);
	digitalWrite(_gslat, LOW);
	
	//digitalWrite(_xblnk, HIGH);
}

void TLC5951::setLED(uint8_t led, uint16_t red, uint16_t green, uint16_t blue) {
	_gsData[led][2] = blue;
	_gsData[led][1] = green;
	_gsData[led][0] = red;
}

void TLC5951::setFunctionData(uint8_t data) {
	_functionData = data;
}

void TLC5951::setBrightness(uint8_t red, uint8_t green, uint8_t blue) {
	_brightRed = red;
	_brightGreen = green;
	_brightBlue = blue;
}

void TLC5951::setAllDCData(uint8_t dcvalue) {
	for(int8_t a = 0; a < 8; a++) {
		for(int8_t b = 0; b < 3; b++) {
			_dcData[a][b] = dcvalue;
		}
	}
}


void TLC5951::updateControl() {
	_bufferCount = 0;
	
	//digitalWrite(_xblnk, LOW);
	digitalWrite(_gslat, HIGH);	
		
	// 89 blank bits to get to correct position for DC/DC/FC (Disregard UD) data
	for(int8_t a = 0; a < 89; a++) {
		setBuffer(0);
	}
	
	// 7-bit Function Data
	for(int8_t a = 6; a >= 0; a--) {
		setBuffer((_functionData & (1<<a)));
	}
	
	// Blue Brightness
	for(int8_t a = 7; a >= 0; a--) {
		setBuffer((_brightBlue & (1<<a)));
	}
	
	// Green Brightness
	for(int8_t a = 7; a >= 0; a--) {
		setBuffer((_brightGreen & (1<<a)));
	}
	
	// Red Brightness
	for(int8_t a = 7; a >= 0; a--) {
		setBuffer((_brightRed & (1<<a)));
	}
	
	// Dot Correctness data
	for(int8_t a = 7; a >= 0; a--) {
		for(int8_t b = 2; b >= 0; b--) {
			for(int8_t c = 6; c >= 0; c--) {
				setBuffer(_dcData[a][b] & (1<<c));
			}
		}
	}
	
	digitalWrite(_gslat, LOW);
	delayMicroseconds(10);
	
	digitalWrite(_gslat, HIGH);
	digitalWrite(_gslat, LOW);
	
	//digitalWrite(_xblnk, HIGH);		
}

void TLC5951::update() {
	updateControl();
	updateGS();
}

void TLC5951::setBuffer(uint8_t bit){
	bitWrite(_buffer, _bufferCount, bit);
	_bufferCount++;
	if(_bufferCount == 8)
	{
	    SPI.setBitOrder(LSBFIRST);
		SPI.transfer(_buffer);
		_bufferCount = 0;
	}
}
