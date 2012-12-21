#ifndef tlc5951_h
#define tcl5951_h

#include "Arduino.h"

class TLC5951
{
	public:
		void init(int gssin, int gssck, int gslat, int xblnk, int gsckrgb);
		void init(int gssin, int gssck, int gslat, int xblnk, int gsckrgb, int grayscale);
		void setAllGSData(int gsvalue);
		void updateGS();
		void setLED(unsigned int led, unsigned int red, unsigned int green, unsigned int blue);
		void setFunctionData(byte data);
		void setBrightness(byte red, byte green, byte blue);
		void setAllDCData(int dcvalue);
		void updateControl();
		void update();
	
	private:
		int _gssin;
		int _gssck;
		int _gslat;
		int _xblnk;
		int _gsckrgb;
		int _dcsin;
		int _dcsck;
		
		byte _functionData;
		byte _brightRed;
		byte _brightGreen;
		byte _brightBlue;
		byte _dcData[8][3];
		
		// [0-7 LED][0-2 RGB]
		unsigned int _gsData[8][3];
};

#endif