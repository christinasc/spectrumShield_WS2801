#include "SPI.h"
#include "Adafruit_WS2801.h"

int spectrumReset=5;
int spectrumStrobe=4;
int spectrumAnalog=0;  //0 for left channel, 1 for right.

// Spectrum analyzer read values will be kept here.
int Spectrum[7];

//WS2801 strip = WS2801(32);

//int dataPin = 2;
//int clockPin = 3;

int dataPin = 11;
int clockPin = 12;

//WS2801 strip = WS2801(32, dataPin, clockPin);
Adafruit_WS2801 strip = Adafruit_WS2801(25, dataPin, clockPin);

void setup() {
  Serial.begin(9600);
  spectrum_init();
  
  strip.begin();
  strip.show(); // all off
  
  //rainbowCycle(5);
  
}

int pos = 0;

void loop() {
  // Some example procedures showing how to display to the pixels
  
  //colorWipe(Color(255, 0, 0), 10);
  //colorWipe(Color(0, 255, 0), 10);
  //colorWipe(Color(0, 0, 255), 10);
  //rainbow(10);
  //rainbowCycle(5);
  unsigned int Counter, Counter2;  
  readSpectrum();  
  
  Counter = (Spectrum[0]/10); 
  //Counter = ((Spectrum[0] + Spectrum[1]+ Spectrum[2]+ Spectrum[3]+ Spectrum[4]+ Spectrum[5]+ Spectrum[6])/7)/10;
    Serial.println(Counter);
    //if(pos > 256 * 5){ // 5 strips 25 colors
   if(pos > 150){ // 1 strips 25 colors 
      pos = 0;
    }

   if(Counter > 5){ 
    pos += Counter;
    //pos += 2;
    rainbowCycleOne(pos);     
   }
   else{
    pos += 2;
    rainbowCycleOne(pos);
    delay(10);
   }
  //} 
  
  /*
  for (pos=0; pos < 256 * 5; pos++) {     // 5 cycles of all 25 colors in the wheel
    rainbowCycleOne(pos);
  }*/
}

void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}


void rainbowCycleOne(int j) {
  int i;
 
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(15);
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void readSpectrum()
{
 
  byte Band;
  for(Band=0;Band <7; Band++)
  {
    Spectrum[Band] = (analogRead(spectrumAnalog) + analogRead(spectrumAnalog) ) >>1; //Read twice and take the average by dividing by 2
    digitalWrite(spectrumStrobe,HIGH);
    digitalWrite(spectrumStrobe,LOW);     
  }
}

void spectrum_init()
{
    //Setup pins to drive the spectrum analyzer. 
  pinMode(spectrumReset, OUTPUT);
  pinMode(spectrumStrobe, OUTPUT);

  //Init spectrum analyzer
  digitalWrite(spectrumStrobe,LOW);
    delay(1);
  digitalWrite(spectrumReset,HIGH);
    delay(1);
  digitalWrite(spectrumStrobe,HIGH);
    delay(1);
  digitalWrite(spectrumStrobe,LOW);
    delay(1);
  digitalWrite(spectrumReset,LOW);
    delay(5);
} 
