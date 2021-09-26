#ifndef __FONTS_H
#define __FONTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define TIME_NUMBER_WIDTH 3
#define TIME_NUMBER_HIGHT 5

typedef struct __rgbColor{
    uint8_t blue;
    uint8_t red;
    uint8_t green;
    uint8_t res;
} regColor_t;

typedef union __rgbPoint{
    uint32_t color;
    regColor_t rgb;
} rgbPoint_u;

typedef struct{
    int width;
    int hight;
    const uint8_t *pixel;
} font_t;

typedef struct{
    int width;
    int hight;
    rgbPoint_u *pixel;
} pattern_t;

int getTimeFonts(int number, font_t *font);
int getWifiPattern(int index, pattern_t *pattern);
int getWeatherPattern(int index, pattern_t *pattern);
void adjustPixelBrightness(uint8_t value, rgbPoint_u *pixel, uint32_t length);
void mixChar2Pattern(pattern_t *pattern,
                     const font_t *font1, const rgbPoint_u color1,
                     const font_t *font2, const rgbPoint_u color2);

#ifdef __cplusplus
}
#endif

#endif /* __FONTS_H */
