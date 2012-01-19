#!/usr/bin/python
import serial
import time

s = serial.Serial('/dev/ttyACM1',baudrate=38400)
s.write(time.strftime('%H%M%S'))
s.flush()

