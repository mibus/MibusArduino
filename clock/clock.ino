#include <AikoCommands.h>
#include <AikoEvents.h>

using namespace Aiko;
using namespace Command;

#include "aiko_pebble_v2.h"

void setup(void) {
  Serial.begin(38400);
  Events.addHandler(lcdHandler,            500);
  Events.addHandler(clockHandler,         1000);
  Events.addHandler(mySerialHandler,      1000);
  Events.addHandler(rotaryEncoderHandler,    1);
  Events.addHandler(rotaryButtonHandler,     1);
}

enum msh_states {
  msh_hr1,
  msh_hr2,
  msh_min1,
  msh_min2,
  msh_sec1,
  msh_sec2,
};

msh_states msh_state = msh_hr1;
int msh_hr=0;
int msh_min=0;
int msh_sec=0;

void mySerialHandler(void) {

  // Drop out if there's no data
  while (Serial.available()) {
    // Grab a char
    char ch = Serial.read();

//    Serial.write ("Read ");
//    Serial.write (ch);
//    Serial.write("\r\n");

    // Munge to numeric, check it's correct-looking
    int n = (ch-'0');
    if (n < 0 || n > 9)
      return;
  
    // Finite State Machine...    
    switch (msh_state) {
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
        msh_state=msh_hr1;      
        break;
    }
  }
}

void loop(void) {
  Events.loop();
}
