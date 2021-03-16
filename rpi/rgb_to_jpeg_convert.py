from PIL import Image
import numpy as np

width = 400
height = 400
#Taken from https://stackoverflow.com/questions/43258461/convert-png-to-jpeg-using-pillow
def rgb565_to_jpeg_convert_func():
    pixels = open("../391_Images/captured_image_pixel_file2")
    #Thanks to https://github.com/humbertokramm/RGB565toRGB888toPNG_-python-/blob/master/teste.py
    a = np.zeros((height,width,3),'uint8')
    print(a.shape)
    x=0
    y=0
    for y in range(0,height):
        for x in range(0, width):
            r = int(pixels.read(5),2)*8 
            g = int(pixels.read(6),2)*4
            b = int(pixels.read(5),2)*8
            a[y,x,0]= r
            a[y,x,1] = g
            a[y,x,2] = b
    
    new_image = Image.fromarray(a)
    new_image.save("../391_Images/tweet_image.jpg")
