Mibus's Random Arduino Bits
===========================

Much (particularly the LCD & rotary-encoder code) was munged from the Pebble v2 example code from LCA2012.

All of the below is released under the GPLv3.

bigclock
--------

A clock with a larger display, only settable via USB at this point. Can also drive the LEDs by serial command (eg. for load display)

clock
-----

A clock settable with the rotary encoder. Can be programmed with the time from a USB-connected computer.

falltoy
-------

Uses a 3-axis accelerometer and piezo; shows different colours on the RGB LED depending on which way is up, and 'screams' if you drop it :)

textout
-------

A Twitter display for #lca2012, much like a tiny version of the big screens that were up around the place :-)

pong
----

What it says on the box; a super-simplistic character-based version of Pong.

