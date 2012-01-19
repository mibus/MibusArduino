/* rotary_encoder.ino
 * ~~~~~~~~~~~~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * ----------------------------------------------------------------------------
 * See http://www.circuitsathome.com/mcu/reading-rotary-encoder-on-arduino
 *
 * To Do
 * ~~~~~
 * - None, yet.
 */

byte rotaryEncoderInitialized = false;

uint8_t rotaryEncoderValue = DEFAULT_LCD_BACKLIGHT;

void rotaryEncoderInitialize(void) {
  pinMode(PIN_ROTARY_A,    INPUT);
  pinMode(PIN_ROTARY_B,    INPUT);
  pinMode(PIN_ROTARY_PUSH, INPUT);

  digitalWrite(PIN_ROTARY_A,    HIGH);
  digitalWrite(PIN_ROTARY_B,    HIGH);
  digitalWrite(PIN_ROTARY_PUSH, HIGH);

  rotaryEncoderInitialized = true;
}

const int ROTARY_ENCODER_FACTOR = 5;

void rotaryEncoderHandler(void) {
  if (! rotaryEncoderInitialized) rotaryEncoderInitialize();

  int8_t delta;

  delta = readRotaryEncoder() * ROTARY_ENCODER_FACTOR;
/*
  if (delta < 0  &&  rotaryEncoderValue < ROTARY_ENCODER_FACTOR) return;
  if (delta > 0  &&  rotaryEncoderValue > (255 - ROTARY_ENCODER_FACTOR)) return;
*/
  if (delta) {
//    rotaryEncoderValue += delta;

//    analogWrite(PIN_LCD_BACKLIGHT, rotaryEncoderValue);
    rotaryDelta(delta);

//  Serial.print("Rotary encoder: ");
//  Serial.println(rotaryEncoderValue);
  }
}

int8_t readRotaryEncoder(void) {
  static int8_t  encoderStates[] = {0, 1, -1, 0, -1, 0, 0, 1, 1, 0, 0, -1, 0, -1, 1, 0};
  static uint8_t encoderBits = 0;

  encoderBits <<= 2;
  encoderBits |= (ROTARY_ENCODER_PORT & ROTARY_ENCODER_MASK) >> 6;
  return(encoderStates[(encoderBits & 0x0f)]);
}

/* ------------------------------------------------------------------------- */
