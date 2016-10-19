import cv2 
import imutils 
from collections import deque
import numpy as np
import serial
import time

greenLower = (29, 86, 6)
greenUpper = (64, 255, 255)
#greenLower=(150,50,50)
#greenUpper=(130,255,255)
pts = deque(maxlen=64)
camera = cv2.VideoCapture(0)
arduino = serial.Serial('COM4', 9600, timeout=.1)
time.sleep(1)
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
		cmd=b'F'
	if len(cnts) > 0:
		center = None
		c = max(cnts, key=cv2.contourArea)
		((x, y), radius) = cv2.minEnclosingCircle(c)
		M = cv2.moments(c)
		x=int(M["m10"] / M["m00"])
		y=int(M["m01"] / M["m00"])
		if radius>10:
			if (x<280):
				cmd=b'R'
			elif (x>380):
				cmd=b'L'
			else:
				if (radius>10 and radius <40):
					cmd=b'S'
				elif (radius>40):
					cmd=b'W'
		else:
			cmd=b'F'
	arduino.write(cmd)
	message = arduino.readline()
	print (message)
	#time.sleep(0.1)
		
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