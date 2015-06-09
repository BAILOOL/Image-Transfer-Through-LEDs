import os, sys
from PIL import Image, ImageTk, ImageFilter
import Tkinter
import serial
import time
from random import randint
import numpy as np
#from scipy import ndimage, misc
import array, PIL, Image

inputcount = 0

arduino = serial.Serial('COM14', 9600)
print "Delay for 5"
time.sleep(5)
print "Starting sending"

fname = "panda.raw";
with open(fname) as f:
	line = f.readlines()[0]

while False:
	try:
		arduino.write(bytes("0"))
		time.sleep(0.35)
		arduino.write(bytes("0"))
		time.sleep(0.35)
		arduino.write(bytes("0"))
		time.sleep(0.35)
		arduino.write(bytes("0"))
		time.sleep(0.35)
	except KeyboardInterrupt:
		arduino.close()
		sys.exit("Suspended")

length = len(line)
print length
charArray = [];
for i in range(0,len(line)):
	if (ord(line[i]) == 255):
		charArray.append("1") ;
	else:
		charArray.append("0") ;

while  True:
	try:
		for i in xrange(0,length,4):
			arduino.write(bytes(charArray[i]))
			print charArray[i]
			#time.sleep(0.4)
			arduino.write(bytes(charArray[i+1]))
			print charArray[i+1]
			#time.sleep(0.4)
			arduino.write(bytes(charArray[i+2]))
			print charArray[i+2]
			#time.sleep(0.4)
			arduino.write(bytes(charArray[i+3]))
			print charArray[i+3]
			#time.sleep(0.4)
		print "Finished"
		sys.exit("SENDING IS COMPLETED")
		#sys.exit("Finished")
	except KeyboardInterrupt:
		arduino.close()
		sys.exit("Suspended")