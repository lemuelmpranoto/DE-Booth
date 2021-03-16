from PIL import Image

def jpeg_to_pixel_conversion_filter_func():
    im = Image.open('../391_Images/filtered_image.jpg')

    pix = im.load()
    target_file = open("../391_Images/filtered_image_pixel_file", "w")
    im_size = im.size
    im_width, im_height = im_size[0], im_size[1]
    print("width",im_width)

    print("height", im_height)
    x=0
    y=0
    pixel_count =0
    bit_count = 0
    while y<im_height:
        x=0
        while x<im_width:
            cur_pix = pix[x,y]
            r_int = cur_pix[0]/8
            g_int = cur_pix[1]/4
            b_int = cur_pix[2]/8
            cur_pix_sum = b_int + g_int*32 + r_int*2048
            #r_bits = str(format(cur_pix[0]/8,"05b"))
            #g_bits = str(format(cur_pix[1]/4,"06b"))
            #b_bits = str(format(cur_pix[2]/8,"05b"))
            #print(r_bits+g_bits+b_bits)
            #print(cur_pix_sum)
            #target_file.write(r_bits)
            #target_file.write(g_bits)
            #target_file.write(b_bits)
            #print(r_bits)
            #print(g_bits)
            #print(b_bits)
            target_file.write(str(cur_pix_sum)+"\n")
            #bit_count += len(r_bits) + len(g_bits) + len(b_bits)
            #pixel_count += 1
            x +=1
        y += 1
    #print("Pixel Count is", pixel_count)
    #print("Bit Count is", bit_count)
    #print("Bit Count should be equal to", str(pixel_count * 16))
    target_file.write("\n")
    target_file.close()

def jpeg_to_pixel_conversion_func():
    im = Image.open('../391_Images/captured_image.jpg')

    pix = im.load()
    target_file = open("../391_Images/captured_image_pixel_file", "w")
    im_size = im.size
    im_width, im_height = im_size[0], im_size[1]
    print("width",im_width)

    print("height", im_height)
    x=0
    y=0
    pixel_count =0
    bit_count = 0
    while y<im_height:
        x=0
        while x<im_width:
            cur_pix = pix[x,y]
            r_int = cur_pix[0]/8
            g_int = cur_pix[1]/4
            b_int = cur_pix[2]/8
            cur_pix_sum = b_int + g_int*32 + r_int*2048
            #r_bits = str(format(cur_pix[0]/8,"05b"))
            #g_bits = str(format(cur_pix[1]/4,"06b"))
            #b_bits = str(format(cur_pix[2]/8,"05b"))
            #print(r_bits+g_bits+b_bits)
            #print(cur_pix_sum)
            #target_file.write(r_bits)
            #target_file.write(g_bits)
            #target_file.write(b_bits)
            #print(r_bits)
            #print(g_bits)
            #print(b_bits)
            target_file.write(str(cur_pix_sum)+"\n")
            #bit_count += len(r_bits) + len(g_bits) + len(b_bits)
            #pixel_count += 1
            x +=1
        y += 1
    #print("Pixel Count is", pixel_count)
    #print("Bit Count is", bit_count)
    #print("Bit Count should be equal to", str(pixel_count * 16))
    target_file.write("\n")
    target_file.close()
