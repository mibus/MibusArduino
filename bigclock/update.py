#!/usr/bin/python
import serial
import time
import re
import os

s = serial.Serial('/dev/ttyACM0',baudrate=38400)

class CPUsage:
   def __init__(self, interval=0.1, percentage=True):
       self.interval=interval
       self.percentage=percentage
       self.result=self.compute()
      
   def get_time(self):
       stat_file=file("/proc/stat", "r")
       time_list=stat_file.readline().split(" ")[2:6]
       stat_file.close()
       for i in range(len(time_list))  :
           time_list[i]=int(time_list[i])
       return time_list
  
   def delta_time(self):
       x=self.get_time()
       time.sleep(self.interval)
       y=self.get_time()
       for i in range(len(x)):
           y[i]-=x[i]
       return y   

   def compute(self):
       t=self.delta_time()
       if sum(t) == 0: return 0
       if self.percentage:
           result=100-(t[len(t)-1]*100.00/sum(t))
       else:
           result=sum(t)
       return result
  
   def __repr__(self):
       return str(self.result)

cpu = CPUsage()
r=re.compile('.*Link Quality=([0-9]*)/([0-9]*).*')
wl=0
while True:
	p = os.popen('/sbin/iwconfig')
	pl = p.readlines()
	pl = [r.match(x) for x in pl if r.match(x)]
	if len(pl):
		wl = float(pl[0].group(1)) / float(pl[0].group(2))
	p=None
	print pl
	
	s.write(time.strftime('t%H%M%S'))
	s.write("r%d\n" % ((255 * cpu.compute()) / 100))
	s.write("g%d\n" % (255 * wl))
	print "Using %d\n" % (255 * wl)
	s.flush()
