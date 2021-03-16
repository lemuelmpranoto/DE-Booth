import PIL
from PIL import Image, ImageFilter
import sys

"""
filterCodes:
    0       blur
    1       edgeDetection
    2       detail
    3       sharpen
    4       contur
    else    unmodified
"""
def softwareFilter(filterCode):
    try:
        img = Image.open("../391_Images/captured_image.jpg")
    except IOError:
        print("err on loading image file")
        sys.exit(1)
    
    if filterCode == 0:
        filterType = ImageFilter.BLUR   
    elif filterCode == 1:
        filterType = ImageFilter.FIND_EDGES  
    elif filterCode == 2:
        filterType = ImageFilter.DETAIL
    elif filterCode == 3:
        filterType = ImageFilter.SHARPEN
    elif filterCode == 4:
        filterType = ImageFilter.CONTOUR
    elif filterCode == 5:
	img.save("../391_Images/filtered_image.jpg")
	return	
    else:
        sys.exit(1)
    

    product = img.filter(filterType)
    product.save("../391_Images/filtered_image.jpg")


def applyCustom5x5Filter(W11,W12,W13,W14,W15,W21,W22,W23,W24,W25,W31,W32,W33,W34,W35,W41,W42,W43,W44,W45,W51,W52,W53,W54,W55):
    try:
        img = Image.open("img.jpg")
    except IOError:
        print("err on loading img.jpg")
        sys.exit(1)
    
    product = img.filter(ImageFilter.Kernel((5, 5), (W11,W12,W13,W14,W15,W21,W22,W23,W24,W25,W31,W32,W33,W34,W35,W41,W42,W43,W44,W45,W51,W52,W53,W54,W55), 1, 0))
    product.save("imgP.jpg")


def main():
    applyCustom5x5Filter(-5,-5,10,-5,-5,-5,-5,10,-5,-5,-5,-5,10,-5,-5,-5,-5,10,-5,-5,-5,-5,10,-5,-5)


if __name__ == '__main__':
    main()
