from PIL import Image
import sys
import os

def getFileName(path):
    path_list = []
    f_list = os.listdir(path)
    for i in f_list:
        if os.path.splitext(i)[1] == '.png':
            path_list.append(i)
    return path_list

def getColorPoint(fileName):
    img = Image.open('./tools/'+fileName)
    img = img.convert('RGB')
    width, height = img.size
    src_strlist = img.load()
    wrap_count = 0
    print("/* {} */".format(fileName))
    for y in range(height):
        for x in range(width):
            rgb = src_strlist[x, y]
            print("{{0x{:02X}{:02X}{:02X}}}, ".format(rgb[1], rgb[0], rgb[2]),end='')
            wrap_count+=1
            if wrap_count == width:
                print("")
                wrap_count = 0

sys.stdout = open('./Core/Src/weatherIcon.c', 'w')
file_list = getFileName('./tools')
print('#include "fonts.h"')
print('const rgbPoint_u weatherIcon[] = {')
for file in file_list:
    getColorPoint(file)
print('};')
