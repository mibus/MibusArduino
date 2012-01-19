/* aiko_pebble_v2.h
 * ~~~~~~~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * ----------------------------------------------------------------------------
 *
 * To Do
 * ~~~~~
 * None, yet !
 */

// Allocate strings in Flash program memory to save RAM

#include <avr/pgmspace.h>
#define PROGSTRING(name) static const prog_char name[] PROGMEM

// LCD backlight intensity
const int DEFAULT_LCD_BACKLIGHT = 80;

// Serial communications speed
const long DEFAULT_BAUD_RATE = 38400;

/* ------------------------------------------------------------------------- */
// Digital Input/Output pins
const int PIN_SERIAL_RX          =  0;  // In:  USB or ZigBee receive
const int PIN_SERIAL_TX          =  1;  // Out: USB or ZigBee transmit

// Standard HD44780 20x4 LCD display driven via
// 3 output pins to a 4094 8-bit shift register
const int PIN_LCD_STROBE         =  2;  // Out: LCD IC4094 shift-register strobe
const int PIN_LCD_DATA           =  3;  // Out: LCD IC4094 shift-register data
const int PIN_LCD_CLOCK          =  4;  // Out: LCD IC4094 shift-register clock
const int PIN_LCD_BACKLIGHT      =  5;  // Out: LCD backlight (PWM)

// Rotary encoder inputs are all normally high
const int PIN_ROTARY_A           =  6;  // In:  Rotary encoder A contact
const int PIN_ROTARY_B           =  7;  // In:  Rotary encoder B contact
const int PIN_ROTARY_PUSH        =  8;  // In:  Rotary encoder push down switch

#define ROTARY_ENCODER_PORT PIND
#define ROTARY_ENCODER_MASK 0xc0

extern uint8_t rotaryEncoderValue;

// RGB LED PWM outputs
const int PIN_LED_RED            =  9;  // Out: Red LED (PWM)
const int PIN_LED_GREEN          = 10;  // Out: Red LED (PWM)
const int PIN_LED_BLUE           = 11;  // Out: Red LED (PWM)

// Open-collector (transistor switch) outputs with LED
// Standard Arduino "blink" example sketch will flash red LED 2
const int PIN_OUTPUT_1           = 12;  // Out: Output transistor and LED 1
const int PIN_OUTPUT_2           = 13;  // Out: Output transistor and LED 2

// Analog Input pins
const int PIN_ANALOG_TEMPERATURE =  0;  // In:  Temperature sensor MCP9701

extern byte temperatureWhole;
extern byte temperatureFraction;

const int PIN_ANALOG_LIGHT       =  1;  // In:  Light sensor TEMT6000

extern byte lightValue;

// Nintendo DS touch panel
const int PIN_ANALOG_X1          =  2;  // In/Out: Touch panel X-axis left
const int PIN_ANALOG_Y2          =  3;  // In/Out: Touch panel Y-axis top
const int PIN_ANALOG_X2          =  4;  // In/Out: Touch panel X-axis right
const int PIN_ANALOG_Y1          =  5;  // In/Out: Touch panel Y-axis bottom

/* ------------------------------------------------------------------------- */
