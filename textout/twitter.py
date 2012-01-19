#!/usr/bin/python
import serial
import time
import re
import os
import random
import feedparser
import math

s = serial.Serial('/dev/ttyACM0',baudrate=38400)

rss_age = 0
feed = None
while True:
	time.sleep (2)
	now = time.time()
	if now-rss_age > 60:
		print " ** Updating RSS... ** "
		feed = feedparser.parse('http://search.twitter.com/search.rss?q=lca2012')
		print " ** Done **"
		rss_age = now
	whichone = int(math.floor(random.random()*max(10,len(feed.entries))))
	print " ** Picked %d **" % (whichone)
	#print feed.entries[whichone].author_detail['email']
	tweet = feed.entries[whichone].author_detail['email'].split('@')[0] +': '+feed.entries[whichone].title
	tweet = re.sub('(.{20})', "\\1\n", tweet)
	if len(tweet) > 84: # 80 plus newlines
		tweet_text = tweet[:81] + '..'
	else:
		tweet_text = tweet
	s.write('\000')
	print "----"
	print tweet_text[:42]
	s.write(tweet_text[:42])
	s.flush()
	time.sleep(0.3)
	s.write(tweet_text[42:])
	print tweet_text[42:]
	print "----"
	time.sleep (8)
