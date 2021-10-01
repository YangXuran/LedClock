#include "fonts.h"

const uint8_t timeNumber[] = {0xF6, 0xDE, 0x59, 0x2E, 0xE7, 0xCE, 0xE7, 0x9E, 0xB7, 0x92,
                              0xF3, 0x9E, 0xF3, 0xDE, 0xE4, 0x92, 0xF7, 0xDE, 0xF7, 0x9E,
                              0x08, 0x20, 0x00, 0x02}; /*0123456789:.*/

const rgbPoint_u wifiIcon[] = {{0x000000}, {0x000000}, {0xEDAFFA}, {0xE276F8}, {0xAA3AFF}, {0x000000}, {0x000000},
                               {0x000000}, {0xEDAFFA}, {0x000000}, {0x000000}, {0x000000}, {0xAA3AFF}, {0x000000},
                               {0xEDAFFA}, {0x000000}, {0xEDAFFA}, {0xE276F8}, {0xAA3AFF}, {0x000000}, {0xAA3AFF},
                               {0x000000}, {0xEDAFFA}, {0x000000}, {0x000000}, {0x000000}, {0xAA3AFF}, {0x000000},
                               {0x000000}, {0x000000}, {0xEDAFFA}, {0xE276F8}, {0xAA3AFF}, {0x000000}, {0x000000}, 
                               {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000},
                               {0x000000}, {0x000000}, {0x000000}, {0xAA3AFF}, {0x000000}, {0x000000}, {0x000000},
                               
                               {0x000000}, {0x000000}, {0xEDAFFA}, {0xE276F8}, {0xAA3AFF}, {0x000000}, {0x2CFF2C},
                               {0x000000}, {0xEDAFFA}, {0x000000}, {0x000000}, {0x000000}, {0x2CFF2C}, {0x000000},
                               {0xEDAFFA}, {0x000000}, {0xEDAFFA}, {0xE276F8}, {0x2CFF2C}, {0x000000}, {0xAA3AFF},
                               {0x000000}, {0xEDAFFA}, {0x000000}, {0x2CFF2C}, {0x000000}, {0xAA3AFF}, {0x000000},
                               {0x000000}, {0x000000}, {0x2CFF2C}, {0xE276F8}, {0xAA3AFF}, {0x000000}, {0x000000}, 
                               {0x000000}, {0x2CFF2C}, {0x000000}, {0x000000}, {0x000000}, {0x000000}, {0x000000},
                               {0x2CFF2C}, {0x000000}, {0x000000}, {0xAA3AFF}, {0x000000}, {0x000000}, {0x000000}};
                        
extern const rgbPoint_u weatherIcon[];
extern int weatherIconIndex[];

int getTimeFonts(int number, font_t *font)
{
    if(number > 10 || font == NULL)
        return -1;
    font->width = TIME_NUMBER_WIDTH;
    font->hight = TIME_NUMBER_HIGHT;
    font->pixel = timeNumber+number*2;
    return 0;
}

int getWifiPattern(int index, pattern_t *pattern)
{
    pattern->width = 7;
    pattern->hight = 7;
    pattern->pixel = (rgbPoint_u*)(wifiIcon+7*7*index);
    return 0;
}

int getWeatherPattern(int index, pattern_t *pattern)
{
    int arrayIndex;

    for(arrayIndex=0; arrayIndex<70; arrayIndex++)
    {
        if(weatherIconIndex[arrayIndex] == index)
            break;
    }
    if(arrayIndex == 70)
        arrayIndex = 69;
    pattern->width = 7;
    pattern->hight = 7;
    pattern->pixel = (rgbPoint_u*)(weatherIcon+7*7*arrayIndex);
    return 0;
}

void adjustPixelBrightness(uint8_t value, rgbPoint_u *pixel, uint32_t length)
{
    int i;

    for(i=0; i<length; i++)
    {
        pixel[i].rgb.red = (uint8_t)(pixel[i].rgb.red*(1.0*value/255));
        pixel[i].rgb.green = (uint8_t)(pixel[i].rgb.green*(1.0*value/255));
        pixel[i].rgb.blue = (uint8_t)(pixel[i].rgb.blue*(1.0*value/255));
    }
}

void mixChar2Pattern(pattern_t *pattern,
                     const font_t *font1, const rgbPoint_u color1,
                     const font_t *font2, const rgbPoint_u color2)
{
    int point = 0, bit = 0;
    rgbPoint_u *pixel = pattern->pixel;
    rgbPoint_u rColor1, rColor2;

    while(1)
    {
        for(bit=0; bit<8; bit++)
        {
            rColor1.color = ((font1->pixel[point/8]>>(7-bit)) & 1) ? color1.color : 0;
            rColor2.color = ((font2->pixel[point/8]>>(7-bit)) & 1) ? color2.color : 0;
            if(rColor1.color == 0 || rColor2.color == 0)
                pixel[point].color = rColor1.color | rColor2.color;
            else
            {
                pixel[point].rgb.red = rColor1.rgb.red > rColor2.rgb.red ? rColor1.rgb.red : rColor2.rgb.red;
                pixel[point].rgb.green = rColor1.rgb.green > rColor2.rgb.green ? rColor1.rgb.red : rColor2.rgb.green;
                pixel[point].rgb.blue = rColor1.rgb.blue > rColor2.rgb.blue ? rColor1.rgb.blue : rColor2.rgb.blue;
            }
            point++;
            if(point > pattern->width*pattern->hight)
                return;
        }
    }
}
