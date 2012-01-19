/* Mibus' big clock
 * Designed for Pebble v2 board
 * (C) Robert Mibus <mibus@mibus.org> 2012
 * License: GPLv3
 */

#include <AikoCommands.h>
#include <AikoEvents.h>

using namespace Aiko;
using namespace Command;

#include "aiko_pebble_v2.h"

void setup(void) {
  Serial.begin(38400);
  Events.addHandler(lcdHandler,            500);
  Events.addHandler(clockHandler,         1000);
  Events.addHandler(mySerialHandler,       200);
  Events.addHandler(rotaryEncoderHandler,    1);
  Events.addHandler(rotaryButtonHandler,     1);
}

enum msh_states {
  msh_cmd,
  msh_hr1,
  msh_hr2,
  msh_min1,
  msh_min2,
  msh_sec1,
  msh_sec2,
  msh_c,
  msh_c_val,
};

msh_states msh_state = msh_cmd;
int msh_hr=0;
int msh_min=0;
int msh_sec=0;
char msh_col=' ';
int msh_col_value=0;

void mySerialHandler(void) {

  // Drop out if there's no data
  while (Serial.available()) {
    // Grab a char
    char ch = Serial.read();

    // Munge to numeric, check it's correct-looking
    int n = (ch-'0');
    if ((n < 0 || n > 9) && (msh_state != msh_cmd && msh_state != msh_c && msh_state != msh_c_val))
      return;
  
    // Finite State Machine...    
    switch (msh_state) {
      case msh_cmd:
        if (ch == 't')
          msh_state = msh_hr1;
        else if (ch == 'r') {
          msh_state = msh_c_val;
          msh_col = 'r';
          msh_col_value=0;
        } else if (ch == 'g') {
          msh_state = msh_c_val;
          msh_col = 'g';
          msh_col_value=0;
        } else if (ch == 'b') {
          msh_state = msh_c_val;
          msh_col = 'b';
          msh_col_value=0;
        }
        break;
      case msh_c_val:
        if (ch == '\r' || ch == '\n') {
          if (msh_col == 'r') {
            analogWrite(9, msh_col_value);
          } else if (msh_col == 'g') {
            analogWrite(10, msh_col_value);
          } else if (msh_col == 'b') {
            analogWrite(11, msh_col_value);
          }
          msh_state = msh_cmd;
        } else {
          if (n < 0 || n > 9)
            return;
          msh_col_value *= 10;
          msh_col_value += n;
        }
        break;
      case msh_hr1:
        msh_hr = 10 * n;
        msh_state=msh_hr2;
        break;
      case msh_hr2:
        msh_hr += n;
        msh_state=msh_min1;
        break;
      case msh_min1:
        msh_min = 10 * n;
        msh_state=msh_min2;
        break;
      case msh_min2:
        msh_min += n;
        msh_state=msh_sec1;
        break;
      case msh_sec1:
        msh_sec = 10 * n;
        msh_state=msh_sec2;
        break;
      case msh_sec2:
        msh_sec += n;
        hour = msh_hr;
        minute = msh_min;
        second = msh_sec;
        msh_state=msh_cmd;      
        break;
    }
  }
}

void loop(void) {
  Events.loop();
}
