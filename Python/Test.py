from scipy.spatial import distance as dist
from imutils import face_utils
import numpy as np
import imutils
import time
from datetime import datetime
import dlib
import cv2
import math 
import serial #pip install pyserial

arduino = serial.Serial('COM3', 9600, timeout=1) #Arduino serial defintions


def send_to_arduino(data):
    arduino.write(data.encode())
	
def send_number_to_arduino(number):
    arduino.write(str(number).encode())

def read_from_arduino():
    return arduino.readline().decode().strip()


SHAPE_PREDICTOR_PATH = r'C:\Python\shape_predictor_68_face_landmarks.dat'


def eye_aspect_ratio(eye):
	# compute the euclidean distances between the two sets of
	# vertical eye landmarks (x, y)-coordinates
	A = dist.euclidean(eye[1], eye[5])
	B = dist.euclidean(eye[2], eye[4])
	# compute the euclidean distance between the horizontal
	# eye landmark (x, y)-coordinates
	C = dist.euclidean(eye[0], eye[3])
	# compute the eye aspect ratio
	ear = (A + B) / (2.0 * C)
	# return the eye aspect ratio
	return ear

shape_predictor = dlib.shape_predictor(SHAPE_PREDICTOR_PATH)

EYE_AR_THRESH = 0.25
EYE_AR_CONSEC_FRAMES = 1 #number of frames to be acceptable
# initializeations
COUNTER = 0 
TOTAL = 0 
Data=""
count=0

print("[INFO] loading facial landmark predictor...")
detector = dlib.get_frontal_face_detector()
predictor = dlib.shape_predictor(SHAPE_PREDICTOR_PATH)

(lStart, lEnd) = face_utils.FACIAL_LANDMARKS_IDXS["left_eye"]
(rStart, rEnd) = face_utils.FACIAL_LANDMARKS_IDXS["right_eye"]
fileStream = False
time.sleep(1.0)

cap= cv2.VideoCapture(0)
cap.set(3,680*2)
cap.set(4,480*2)
cap.set(cv2.CAP_PROP_FPS, 120)
time1 = datetime.now()
time2 = datetime.now()
time_difference_str = ""
NBT_difference_str = ""

while True:
	ret, frame = cap.read()
	gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
	# detect faces in the grayscale frame
	rects = detector(gray, 0)
		# loop over the face detections
	for (i, rect) in enumerate(rects):
		shape = predictor(gray, rect)
		shape = face_utils.shape_to_np(shape)
		# extract the left and right eye coordinates, then use the
		# coordinates to compute the eye aspect ratio for both eyes
		leftEye = shape[lStart:lEnd]
		rightEye = shape[rStart:rEnd]
		leftEAR = eye_aspect_ratio(leftEye)
		rightEAR = eye_aspect_ratio(rightEye)
		# average the eye aspect ratio together for both eyes
		ear = (leftEAR + rightEAR) / 2.0

		# compute the convex hull for the left and right eye, then
		# visualize each of the eyes
		leftEyeHull = cv2.convexHull(leftEye)
		rightEyeHull = cv2.convexHull(rightEye)
		cv2.drawContours(frame, [leftEyeHull], -1, (255,0,0), 1)
		cv2.drawContours(frame, [rightEyeHull],-1, (255,0,0), 1)
		time=str(datetime.now())
		if ear < EYE_AR_THRESH:
			COUNTER += 1

		
		else:
			if COUNTER >= EYE_AR_CONSEC_FRAMES:
				TOTAL += 1
			# reset the eye frame counter
			COUNTER = 0
			time1 = datetime.now()
			NBT_difference = time1 - time2
			time_difference_str = str(0)
			data_int = 0
			data_bytes = data_int.to_bytes(1, byteorder='big')
			if Data !=data_bytes:
				Data=data_bytes
				arduino.write(data_bytes)

			if str(NBT_difference) > NBT_difference_str:
				NBT_difference_str = str(NBT_difference)
			cv2.putText(frame, f'MAX No Blink Time: {(NBT_difference_str[3:9])} S', (10, 500),
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2, cv2.LINE_AA)

		
		if ear < EYE_AR_THRESH:
			# Get the current time
			time2 = datetime.now()

			# Calculate the time difference
			time_difference = time2 - time1
			
			# Convert timedelta to string
			time_difference_str = str(time_difference)
			
			cv2.putText(frame, f'MAX No Blink Time: {(NBT_difference_str[3:9])} S', (10, 500),
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2, cv2.LINE_AA)

			if time_difference_str <="0:00:02" :
				count=0
				cv2.rectangle(frame,(0,670),(510,710),(0,255,0),-1)
			elif time_difference_str >"0:00:02"  and time_difference_str <="0:00:04":
				data_int = 1
				data_bytes = data_int.to_bytes(1, byteorder='big')
				if Data !=data_bytes:
					Data=data_bytes
					arduino.write(data_bytes)  #send to areduino
				cv2.rectangle(frame,(0,670),(510,710),(255,0,0),-1)
			elif time_difference_str >"0:00:04" :
				data_int = 2
				data_bytes = data_int.to_bytes(1, byteorder='big')
				if count<=3:
					count+=1
					Data=data_bytes
					arduino.write(data_bytes)  #send to areduino

				cv2.rectangle(frame,(0,670),(510,710),(0,0,255),-1)


		cv2.putText(frame, "EAR: {:.2f}".format(ear), (1160, 30),
			cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
		cv2.putText(frame,time,(5,700),cv2.FONT_HERSHEY_TRIPLEX,1,(0,0,0),2,cv2.LINE_AA)
		cv2.putText(frame, f'Blind Time: {(time_difference_str[2:9])} S', (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2, cv2.LINE_AA)

		for (x, y) in shape:
			cv2.circle(frame, (x, y), 1, (0, 0, 255), -1)

	cv2.imshow("Frame", frame)

	esc= cv2.waitKey(1)
	if esc == 27 :
		break

arduino.close()
cap.release()
cv2.destroyAllWindows()
