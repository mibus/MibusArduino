/* Mibus' text displayer
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
  Events.addHandler(mySerialHandler,       200);
}

int row=0;

void mySerialHandler(void) {

  // Drop out if there's no data
  while (Serial.available()) {
    // Grab a char
    char ch = Serial.read();

    if (ch == 0) {
      lcdClear();
      lcdPosition(row=0,0);
    } else if (ch == '\n') {
      row++;
      lcdPosition(row,0);
    } else
      lcdWrite(ch,1);  
  }
}

void loop(void) {
  Events.loop();
}

