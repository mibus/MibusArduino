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
  Events.addHandler(rotaryEncoderHandler,    1);
}

void loop(void) {
  Events.loop();
}

