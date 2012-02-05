// Requires Marc Merlin's Pebble-v2-compatible version of the LCD library.
// https://github.com/marcmerlin/NewLiquidCrystal
// Let's be easy and use the same license as the library...
// Copyright (C) Robert Mibus <mibus@mibus.org> 2012, licensed under CC BY-SA 3.0

#define RAIN_PERC 20

#include <LiquidCrystal_SR_LCD3.h>

// Data are the input bits
// Clock tells the SR that the data pin holds the next valid bit
// Strobe gets data input to be made visible on the parallel output pins.

const int PIN_LCD_STROBE         =  2;  // Out: LCD IC4094 shift-register strobe
const int PIN_LCD_DATA           =  3;  // Out: LCD IC4094 shift-register data
const int PIN_LCD_CLOCK          =  4;  // Out: LCD IC4094 shift-register clock
const int PIN_LCD_BACKLIGHT      =  5;  // Out: LCD backlight (PWM)

// srdata / srclock / strobe
LiquidCrystal_SR_LCD3 lcd(PIN_LCD_DATA, PIN_LCD_CLOCK, PIN_LCD_STROBE);

byte blob[8] = {
  0b00000,
  0b00000,
  0b01110,
  0b11111,
  0b11111,
  0b01110,
  0b00000,
  0b00000,
};
byte littleblob[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b01110,
  0b00100,
  0b00000,
  0b00000,
};


void setup(){
    pinMode(PIN_LCD_BACKLIGHT, OUTPUT);
    analogWrite(PIN_LCD_BACKLIGHT, 80);

    lcd.begin(20, 4);               // initialize the lcd 

    // load characters to the LCD
    lcd.createChar(0, blob);
    lcd.createChar(1, littleblob);

    lcd.home ();                   // go home
}

int loops=0;

void loop()
{
   loops = (loops + 1) % 20;    
   // scroll one position left:
   lcd.scrollDisplayRight(); 
   // wait a bit:
   delay(50);
   int where=random(4);
   lcd.setCursor (19-loops, where);
   int r=random(100);
   lcd.print(r > RAIN_PERC ? ' ' : (r > RAIN_PERC/2 ? char(0) : char(1)));    
}
