//-------------------------------------------
// Example usage for Arduino TLC5951 Library
// Written by Kenneth Lorthioir - 2/3/2014
// The follow code assumes a TLC5951 connected to an Arduino Mega 2560
// GSSCK and GSSIN are connected to SPI Clock and MOSI pins respectively
// GSLAT is on pin 22 and XBLNK to 23
// All the Grayscale clocks for Green, Red, and Blue (GSCKG, GSCKR, GSCKB)
// are connected to the same arduino pin (9)
//-------------------------------------------

#include <TLC5951.h>
#include <SPI.h>

#define GSLAT 22
#define XBLNK 23
#define GSCKGRB 9

TLC5951 tlc;

void setup() {
  // Set the timer to max speed for fastest PWM generation
  // This is required so that the Grayscale clocks on the TLC5951 are fast enough to prevent visible jittering in the LEDS
  // The current values are for a Arduino Mega 2560
  // Link to find values for you arduino/pins: http://playground.arduino.cc/Main/TimerPWMCheatsheet
  TCCR2B = TCCR2B & 0b11111000 | 0x01; // Pins 9 & 10 @ 31250 Hz

  // Now set the GSCKGRB to an output and a 50% PWM duty-cycle
  // For simplicity all three grayscale clocks are tied to the same pin
  pinMode(GSCKGRB, OUTPUT);
  analogWrite(GSCKGRB, 127);

  // The library does not ininiate SPI for you, so as to prevent issues with other SPI libraries
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  // init(GSLAT pin, XBLNK pin, default grayscale values for all LEDS)
  tlc.init(GSLAT, XBLNK, 0);

  // setLED(Output number, Red value, Green value, Blue value)
  // The next lines set all LED's to the brightest blue setting (0-4095)
  tlc.setLED(0, 0, 0, 4095);
  tlc.setLED(1, 0, 0,  4095);
  tlc.setLED(2, 0, 0, 4095);
  tlc.setLED(3, 0, 0, 4095);
  tlc.setLED(4, 0, 0, 4095);
  tlc.setLED(5, 0, 0, 4095);
  tlc.setLED(6, 0, 0, 4095);
  tlc.setLED(7, 0, 0, 4095);

  // We must set dot correction values, so set them all to the brightest adjustment
  tlc.setAllDCData(127);

  // Set Function Control Data Latch values. See the TLC5951 Datasheet for the purpose of this latch.
  // Data is input LSB first, ie bit 192 (for red DC range) is first, a 1
  // bit 198 (0) is last, for the grayscale counter mode
  tlc.setFunctionData(B1111100);

  // set all brightness levels to max (127)
  tlc.setBrightness(127,127,127);

  // Finally, call the function to send the data to the TLC5951
  tlc.update();
}                                                                                                                                                                                                  

void loop() {
  // The follow code will cycle through a range of rainbow colors to demonstrate the functionality.
  // Code was adapted from https://gist.github.com/jamesotron/766994

  unsigned int rgbColour[3];

  // Start off with red.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;  

  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;

    // cross-fade the two colours.
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      
   for(int a = 0; a < 7; a++)
      {
        tlc.setLED(a, rgbColour[0], rgbColour[1], rgbColour[2]);
      }
      tlc.updateGS();
    delay(10);
    }
  }
}                  
  
  
