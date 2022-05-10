"""
By: Noah Schliesman
Utilizes face_recognition library for RasPi cam and checks if that face recognition is in
the list of blacklisted users. If the raspberry pi camera detects a blacklisted face,
a series of Neopixel lights alternate in a blue-red pattern.
"""


import face_recognition
import picamera
import numpy as np
import board
import neopixel
import time

# initializes pi camera and neopixels
cam = picamera.PiCamera()
cam.resolution = (320, 240)
output = np.empty((240, 320, 3), dtype=np.uint8)
pixels = neopixel.NeoPixel(board.D18, 30)

# fill in this list with blacklisted faces (images)
not_allowed = []

# initialize 2 empty lists for image and encoding face_recognition
images = []
blacklisted_encoding = []

def blacklisting():
    """
    Sorts through blacklisted images and adds them to images list.
    Adds blacklisted face encodings to blacklisted_encoding list.
    """
    for i in range(len(not_allowed)):
        images.append(face_recognition.load_image_file(not_allowed[i]))
        blacklisted_encoding.append(face_recognition.face_encodings(images[i]))
        
    # validation pattern to capture current faces
    valid = True
    while valid:
        cam.capture(output, format="rgb")
        locations = face_recognition.face_locations(output)
        encodings = face_recognition.face_encodings(output, locations)
  
        # checks to see if current face is blacklisted 
        # could implement better sort algorithm in future (w/ better O(n))?
        for i in encodings:
            for j in range(len(blacklisted_encoding)):
                if (face_recognition.compare_faces(blacklisted_encoding[j], i)):
                    return 1
               
if __name__ = "__main__":
    while (blacklisting() != 1): # if blacklisting yields 1 (match), alternating lights shine 
        pixels.fill((255, 0, 0))
        time.sleep(1)
        pixels.fill(0, 0, 255)
        time.sleep(1)
