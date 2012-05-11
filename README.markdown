Mibus's Random Arduino Bits
===========================

Much (particularly the LCD & rotary-encoder code) was munged from the Pebble v2 example code from LCA2012.

Projects below are released under the GPLv3, unless otherwise noted.

bigclock
--------

A clock with a larger display, only settable via USB at this point. Can also drive the LEDs by serial command (eg. for load display)

clock
-----

A clock settable with the rotary encoder. Can be programmed with the time from a USB-connected computer.

falltoy
-------

Uses a 3-axis accelerometer and piezo; shows different colours on the RGB LED depending on which way is up, and 'screams' if you drop it :)

MothersDayFlasher
-----------------

A simple LED chaser built for Mother's Day 2012 for my kids to give my lovely wife :)

Raining
-------

Uses the Pebble v2's 20x4 LCD display and custom characters to display a "rain" effect (the LCD needs to be mounted sideways). Could readily be extended to have more rain based on CPU load or net traffic.

Licensed under: CC BY-SA 3.0.

StairAlert1
-----------

An attempt at using the 3-axis accelerometer as a movement alarm. Doesn't work well on my battery-based setup, but could be a game (move it but not too fast!) on stable power (eg. USB).

textout
-------

A Twitter display for #lca2012, much like a tiny version of the big screens that were up around the place :-)

pong
----

What it says on the box; a super-simplistic character-based version of Pong.

