import socket
import sys
from picamera import PiCamera 
import io
import os
import time
from tweet_pi import tweet
from jpeg_to_rgb565_conversion import jpeg_to_pixel_conversion_func
from rgb_to_jpeg_convert import rgb565_to_jpeg_convert_func
camera = PiCamera()
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('10.42.0.44',1234))

s.listen(1)
print("Server started looking for client connections")
c, addr = s.accept()
print("The Client Connected from", addr)
#c.send("What do you request?")
answer = c.recv(1024)

while True:
    if answer !="":
        print("De1 wants ",answer)
    else:
        #print("No request")
        continue
    if answer == "image":
        #Taking new picture with the rpi camera
        camera.resolution = (400,400)
        camera.capture('/home/pi/Desktop/391_Images/captured_image.jpg')
        jpeg_to_pixel_conversion_func()
        os.system("scp ../391_Images/captured_image_pixel_file efeberkeevci@10.42.0.1:/home/efeberkeevci/Desktop/CPEN391/module1/DE1_SOC_SOCKET_FILES")
        c.send("Rpi to De1 transfer completed")
        #f = open('../391_Images/captured_image_pixel_file','rb')
        #image_size = os.path.getsize("../391_Images/captured_image_pixel_file")
        #c.send(str(image_size))
        #Taken from https://gist.github.com/giefko/2fa22e01ff98e72a5be2
        #l = f.read(1024)
        #package = 1
        #while(l):
        #    c.send(l)
        #    #print("Package sent num", package)
        #    l= f.read(1024)
        #    package += 1
        #print("Sent image")
        #end_of_file = "end_of_file" 
        #c.send(end_of_file)
        #break
    elif answer == "tweet":
        #Ask for the image to tweet
        print("Tweet requested") 

        #Tweet the image processed in de1soc
        print("Waiting for de1 to send the processed image")
        de1_tweet_image_transfer_response = c.recv(1024)
        while(de1_tweet_image_transfer_response != "Completed"):
                print(de1_tweet_image_transfer_response)
        print("Image received in RGB565 format now converting to JPEG")
        rgb565_to_jpeg_convert_func()
        print("Conversion completed. Image is in jpeg format")
        #Tweet the image processed in de1soc
        print("Starting the tweeting process")
        tweet()
        print("Tweeted!")
        c.send("tweeted succesfully")
    elif answer == "close":
        s.close()
        print("Connection Closed")
   #time.sleep(1) 
    answer = c.recv(1024)
