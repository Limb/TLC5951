#ifndef TLC5951_h
#define TLC5951_h

#include "Arduino.h"
#include "SPI.h"

class TLC5951
{
	public:
		void init(uint8_t gslat, uint8_t xblnk);
		void init(uint8_t gslat, uint8_t xblnk, uint16_t grayscale);
		void setAllGSData(uint16_t gsvalue);
		void updateGS();
		void setLED(uint8_t led, uint16_t red, uint16_t green, uint16_t blue);
		void setFunctionData(uint8_t data);
		void setBrightness(uint8_t red, uint8_t green, uint8_t blue);
		void setAllDCData(uint8_t dcvalue);
		void updateControl();
		void update();
		void setBuffer(uint8_t bit);
	
	private:
		uint8_t _gslat;
		uint8_t _xblnk;
		
		uint8_t _functionData;
		uint8_t _brightRed;
		uint8_t _brightGreen;
		uint8_t _brightBlue;
		uint8_t _dcData[8][3];
		
		// [0-7 LED][0-2 RGB]
		uint16_t _gsData[8][3];
		
		// SPI
		uint8_t _buffer;
		uint8_t _bufferCount;
};

#endif