/* lcd.ino
 * ~~~~~~~
 * Please do not remove the following notices.
 * License: GPLv3. http://geekscape.org/static/arduino_license.html
 * ----------------------------------------------------------------------------
 *
 * To Do
 * ~~~~~
 * - None, yet.
 *
 * ---------------------------------------------------------------------------- 
 * LCD KS0066 4-bit data interface, 3 Arduino pins and MC14094 8-bit register
 * http://www.datasheetsite.com/datasheet/KS0066
 *
 * MC14094 input:  Arduino digital pin 2=Clock, pin 3=Data, pin 4=Strobe
 * MC14094 output: Q8=DB4, Q7=DB5, Q6=DB6, Q5=DB7, Q4=E, Q3=RW, Q2=RS, Q1=None
 * http://www.ee.mut.ac.th/datasheet/MC14094.pdf
 *
 *   +--------------------------------------------+
 *   |    Arduino (ATMega 168 or 328)             |
 *   |    D02           D03           D04         |
 *   +----+-------------+-------------+-----------+
 *        |4            |5            |6
 *        |1            |2            |3
 *   +----+-------------+-------------+-----------+
 *   |    Strobe        Data          Clock       |
 *   |    MC14094 8-bit shift/latch register      |
 *   |    Q8   Q7   Q6   Q5   Q4   Q3   Q2   Q1   |
 *   +----+----+----+----+----+----+----+----+----+
 *        |11  |12  |13  |14  |7   |6   |5   |4
 *        |11  |12  |13  |14  |6   |5   |4
 *   +----+----+----+----+----+----+----+---------+
 *   |    DB4  DB5  DB6  DB7  E    RW   RS        |
 *   |               LCD KS0066                   |
 *   +--------------------------------------------+
 */

// LCD pin bit-patterns, output from MC14094 -> LCD KS0066 input
#define LCD_ENABLE_HIGH 0x10  // MC14094 Q4 -> LCD E
#define LCD_ENABLE_LOW  0xEF  //   Enable (high) / Disable (low)
#define LCD_RW_HIGH     0x20  // MC14094 Q3 -> LCD RW
#define LCD_RW_LOW      0xDF  //   Read (high) / Write (low)
#define LCD_RS_HIGH     0x40  // MC14094 Q2 -> LCD RS
#define LCD_RS_LOW      0xBF  //   Data (high) / Instruction (low) Select

// LCD Commands
#define LCD_COMMAND_CLEAR             0x01  // Clear display
#define LCD_COMMAND_HOME              0x02  // Set DD RAM address counter to (0, 0)
#define LCD_COMMAND_ENTRY_SET         0x06  // Entry mode set
#define LCD_COMMAND_DISPLAY_SET       0x0C  // Display on/off control
#define LCD_COMMAND_FUNCTION_SET      0x28  // Function set
#define LCD_COMMAND_SET_DDRAM_ADDRESS 0x80  // Set DD RAM address counter (row, column)

#define LCD_ODD_ROW_OFFSET  0x40  // For first and third row

byte lcdSetup[] = {         // LCD command, delay time in milliseconds
  LCD_COMMAND_HOME,         50,  // wait for LCD controller to be initialized
  LCD_COMMAND_HOME,         50,  // ditto
  LCD_COMMAND_FUNCTION_SET,  1,  // 4-bit interface, 2 display lines, 5x8 font
  LCD_COMMAND_DISPLAY_SET,   1,  // turn display on, cursor off, blinking off
  LCD_COMMAND_CLEAR,         2,  // clear display
  LCD_COMMAND_ENTRY_SET,     1   // increment mode, display shift off
};

byte lcdInitialized = false;

void lcdInitialize(void) {
  pinMode(PIN_LCD_STROBE,    OUTPUT);
  pinMode(PIN_LCD_DATA,      OUTPUT);
  pinMode(PIN_LCD_CLOCK,     OUTPUT);
  pinMode(PIN_LCD_BACKLIGHT, OUTPUT);

  analogWrite(PIN_LCD_BACKLIGHT, DEFAULT_LCD_BACKLIGHT);

  byte length = sizeof(lcdSetup) / sizeof(*lcdSetup);
  byte index = 0;

  while (index < length) {
    lcdWrite(lcdSetup[index ++], false);
    delay(lcdSetup[index ++]);
  }

  lcdInitialized = true;
}

int flashThing=0;
int flashToggle=0;

long lastbtn=0;

void rotaryButtonHandler () {
  long now=millis();
  // Debounce
  if (now-lastbtn < 1000)
    return;
  // Is the button down?
  if (digitalRead(8) == HIGH)
    return;
  // Note it
  lastbtn = now;
  // Flash something else (or nothing)
  flashThing++;
//  Serial.write("Flash thing is now ");
//  Serial.write(flashThing);
//  Serial.write("...\r\n");
  if (flashThing > 3)
    flashThing = 0;
}

void rotaryDelta(int8_t delta) {
  long now=millis();
  
  // Debounce
  if (now-lastbtn < 200)
    return;
  lastbtn = now;
  
  switch (flashThing) {
    case 1:
      hour += (delta>0?1:-1);
      break;
    case 2:
      minute += (delta>0?1:-1);
      break;
    case 3:
      second += (delta>0?1:-1);
      break;
  }

  // Sanity FTW
  if (hour < 0) hour = 23;
  if (hour > 23) hour = 0;
  if (minute < 0) minute = 59;
  if (minute > 59) minute = 0;
  if (second < 0) second = 59;
  if (second > 59) second = 0;

}

void writeBigDigit (int spot, int num) {
  int left = spot * 5 + (spot > 1 ? 1 : 0);
  char *numbers[10] = {
    " 00 0  00  0 00 ", // 0
    " 11  11  11  11 ", // 1
    " 22 2  2  2 2222", // 2
    "3333  33  333333", // 3
    "4  44444   4   4", // 4
    "55555    5555555", // 5
    " 6666    66  66 ", // 6
    "7777   7  7  7  ", // 7
    " 88 8888 88 8888", // 8
    " 99 9  9 999   9", // 9
  };
    
  if (num < 0 || num > 9)
    return;

  char *t;
  t=numbers[num];
    
  for (int i=0; i<4; i++) {
    lcdPosition (i, left);
    for (int j=0;j<4;j++) {
      lcdWrite(t[i*4+j], 1);
    }
  }
}

void lcdHandler(void) {

  if (! lcdInitialized) {
    lcdInitialize();
    lcdClear();
  }

  writeBigDigit (0, hour/10);
  writeBigDigit (1, hour % 10);
  writeBigDigit (2, minute/10);
  writeBigDigit (3, minute % 10);
  
  // Draw a colon!!
  lcdPosition (1, 9);
  lcdWrite(']',1);
  lcdWrite('[',1);
  lcdPosition (2, 9);
  lcdWrite(']',1);
  lcdWrite('[',1);
}

void lcdWrite(
  byte value,
  byte dataFlag) {

  digitalWrite(PIN_LCD_STROBE, LOW);

  byte output = value >> 4;                        // Most Significant Nibble

  if (dataFlag) {
    output = (output | LCD_RS_HIGH) & LCD_RW_LOW;  // Command or Data ?
  }

  for (byte loop1 = 0; loop1 < 2; loop1 ++) {   // First MSN, then LSN
    for (byte loop2 = 0; loop2 < 3; loop2 ++) { // LCD ENABLE LOW -> HIGH -> LOW
      output = (loop2 == 1) ?
       (output | LCD_ENABLE_HIGH) : (output & LCD_ENABLE_LOW);

      shiftOut(PIN_LCD_DATA, PIN_LCD_CLOCK, LSBFIRST, output);
      digitalWrite(PIN_LCD_STROBE, HIGH);
      delayMicroseconds(10);
      digitalWrite(PIN_LCD_STROBE,LOW);
    }
delay(1);
    output = value & 0x0F;                           // Least Significant Nibble

    if (dataFlag) {
      output = (output | LCD_RS_HIGH) & LCD_RW_LOW;  // Command or Data ?
    }
  }
}

void lcdClear(void) {
  lcdWrite(LCD_COMMAND_CLEAR, false);
  delay(2);
}

void lcdPosition(
  byte row,        // Must be either 0 (first row), 1, 2 or 3 (last row)
  byte column) {   // Must be between 0 and 19

  if (row > 1) column += 20;
  row = (row & 1)  ?  LCD_ODD_ROW_OFFSET  :  0;

  lcdWrite(LCD_COMMAND_SET_DDRAM_ADDRESS | row | column, false);
  delayMicroseconds(40);
}

void lcdWriteString(
  char message[]) {

  while (*message) lcdWrite((*message ++), true);
}

int estimateDigits(int nr) {
  int dec = 10;
  int temp = 1;
  int div = nr/dec;
  while (div > 0) {
    dec *= 10;
    div = nr/dec;
    temp++;
  }
  return temp;
}

int pow(int base, int expo) {
  int temp = 1;
  for (int c = 1; c <= expo; c++) {
    temp *= base;
  }
  return temp;
}

void lcdWriteNumber(int nr, int digits) {
  for (int i = digits-1; i >= 0; i--) {
    int dec = pow(10,i);
    int div = nr/dec;
    lcdWrite(div+48, true);
    if (div > 0) {
      nr -= div*dec;
    }
  }
}

void lcdWriteNumber(int nr) {
  int value = nr;

  if (value < 0) {
    lcdWrite('-', true);
    value = - nr;
  }

  int digits = estimateDigits(value);
  lcdWriteNumber(value, digits);
}

/* ------------------------------------------------------------------------- */
