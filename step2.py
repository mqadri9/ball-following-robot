import cv2 
import imutils 
from collections import deque
import numpy as np
import serial
import time
import math
import struct

def pController(radius,mode,x=0):
	referenceRadius=30
	referenceCenter=300
	errorInRadius=referenceRadius-radius	
	if mode==1:
		BaseSpeed=40+errorInRadius*(1/5)
		BaseSpeed=math.floor(BaseSpeed)
		if BaseSpeed>90:
			BaseSpeed=90
		errorInCenter=referenceCenter-x
		u=math.floor(errorInCenter*0.15)
		if u>40:	
			u=40
		u=str(u)
		if len(u)==1:
			u="0"+u
		return "L"+u+str(BaseSpeed)
	elif mode==2:
		BaseSpeed=40+errorInRadius*(1/15)
		BaseSpeed=math.floor(BaseSpeed)
		if BaseSpeed>90:
			BaseSpeed=90
		errorInCenter=x-referenceCenter
		u=math.floor(errorInCenter*0.05)
		if u>40:	
			u=40
		u=str(u)
		if len(u)==1:
			u="0"+u
		return "R"+u+str(BaseSpeed)
	elif mode==3:
		BaseSpeed=40+errorInRadius*(1/10)
		BaseSpeed=math.floor(BaseSpeed)
		if BaseSpeed>90:
			BaseSpeed=90
		return "S00"+str(BaseSpeed)
		
	
	
greenLower = (29, 86, 6)
greenUpper = (64, 255, 255)
#greenLower=(150,50,50)
#greenUpper=(130,255,255)
pts = deque(maxlen=64)
camera = cv2.VideoCapture(0)
arduino = serial.Serial('COM4', 9600, timeout=.1)
time.sleep(1)
k=0
while True:
	(grabbed, frame) = camera.read()
	frame = imutils.resize(frame, width=600)
	hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
	mask = cv2.inRange(hsv, greenLower, greenUpper)
	mask = cv2.erode(mask, None, iterations=2)
	mask = cv2.dilate(mask, None, iterations=2)
	cnts = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL,
	cv2.CHAIN_APPROX_SIMPLE)[-2]
	if (len(cnts))==0:
		cmd="F0000"
	if len(cnts) > 0:
		center = None
		c = max(cnts, key=cv2.contourArea)
		((x, y), radius) = cv2.minEnclosingCircle(c)
		M = cv2.moments(c)
		x=int(M["m10"] / M["m00"])
		y=int(M["m01"] / M["m00"])
		if radius>10:
			if (x<260):
				cmd=pController(radius,1,x)
			elif (x>360):
				cmd=pController(radius,2,x)
			else:
				if (radius>10 and radius <40):
					cmd=pController(radius,3)
				elif (radius>40):
					cmd="W0000"
		else:
			cmd="F0000"
	#print(cmd)
	arduino.write(cmd.encode())
	message = arduino.readline()
	print (k)
	print (message)
	k=k+1
	time.sleep(0.1)
		
#camera.release()
#cv2.destroyAllWindows()


#cv2.circle(frame, (int(x), int(y)), int(radius),(0, 255, 255), 2)
#		cv2.circle(frame, center, 5, (0, 0, 255), -1)
#		pts.appendleft(center)
#		for i in range(1, len(pts)):
#			if pts[i - 1] is None or pts[i] is None:
#				continue
#				thickness = int(np.sqrt(64/ float(i + 1)) * 2.5)
#				cv2.line(frame, pts[i - 1], pts[i], (0, 0, 255), thickness)
#			cv2.imshow("Frame", frame)
#			key = cv2.waitKey(1) & 0xFF
#			if key == ord("q"):
#				break '''