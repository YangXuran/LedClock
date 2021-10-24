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

void generateWeekDayPattern(int weekDay, pattern_t *pattern)
{
    rgbPoint_u weekDayColor[] = {{.color = 0xe9fa57}, {.color = 0x97fc17}, {.color = 0x4df52d}, {.color = 0x33bc5e},
                                 {.color = 0x2e788c}, {.color = 0x284a8a}, {.color = 0x242d6c}};

    if(weekDay > 7 || weekDay < 0)
        return;

    weekDay --;
    pattern->hight = 7;
    pattern->width = 4;
    pattern->pixel = rt_malloc(sizeof(rgbPoint_u)*pattern->hight*pattern->width);
    rt_memset(pattern->pixel, 0, sizeof(rgbPoint_u)*pattern->hight*pattern->width);

    for(int i=6; i>=0; i--)
        for(int j=0; j<pattern->width; j++)
            if(weekDay >= (6-i))
                pattern->pixel[pattern->width*i+j].color = weekDayColor[i].color;
            else
                pattern->pixel[pattern->width*i+j].color = 0x020202;

    pattern->pixel[2].color = 0;
    pattern->pixel[3].color = 0;
    pattern->pixel[7].color = 0;
    pattern->pixel[20].color = 0;
    pattern->pixel[24].color = 0;
    pattern->pixel[25].color = 0;
}

void generateTemperaturePattern(int temp, pattern_t *pattern)
{
    int i, j;
    int label;
    rgbPoint_u tempColor[] = {{.color = 0x0f0f8d}, {.color = 0x8917b0}, {.color = 0x26b03b}, {.color = 0x54bb00}};

    pattern->hight = 1;
    pattern->width = 19;
    pattern->pixel = rt_malloc(sizeof(rgbPoint_u)*pattern->hight*pattern->width);
    rt_memset(pattern->pixel, 0, sizeof(rgbPoint_u)*pattern->hight*pattern->width);

    if(temp == -273)    /* temp == -273时返回空条 */
        label = -1;
    else if(temp <= 0)
        label = 0;
    else if(temp <= 10)
        label = 1;
    else if(temp <= 20)
        label = 2;
    else 
        label = 3;

    for(i=0; i<4; i++)
    {
        for(j=0; j<4; j++)
        {
            if(i <= label)
                pattern->pixel[5*i+j].color = tempColor[i].color;
            else
                pattern->pixel[5*i+j].color = 0x020202;
        }
    }
}
