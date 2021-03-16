import socket
import sys
import picamera
import io
import os
import time
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('172.20.10.9',1234))

s.listen(1)
print("Server started looking for client connections")
while True:
    c, addr = s.accept()
    print("The Client Connected from", addr)
    c.send("What do you request?")
    answer = c.recv(1024)
    print(answer)
        
    if answer == "image":
        f = open('../secondimage.jpg','rb')
        image_size = os.path.getsize("../secondimage.jpg")
        c.send(str(image_size))
        #Taken from https://gist.github.com/giefko/2fa22e01ff98e72a5be2
        l = f.read(1024)
        package = 1
        while(l):
            c.send(l)
            print("Package sent num", package)
            l= f.read(1024)
            package += 1

        print("Sent image")
        break
s.close()
print("Connection Closed")
