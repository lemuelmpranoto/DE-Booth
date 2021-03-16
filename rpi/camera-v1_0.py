from picamera import PiCamera
from time import sleep
import numpy as np

camera = PiCamera()
#camera.resolution = (133,100)
camera.capture('/home/pi/Desktop/group_image.jpg')
