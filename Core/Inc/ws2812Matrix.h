#ifndef __WS2812MATRIX_H
#define __WS2812MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "fonts.h"

#define MATRIX_ROW      8
#define MATRIX_COLUMN   32

void* getPwmDmaRamAddr(void);
int getPwmDmaRamSize(void);
void matrix2pwm(void);
void displayChar(int x, int y, const font_t *font, const rgbPoint_u fontColor);
void displayPattern(int x, int y, const pattern_t *pattern);

#ifdef __cplusplus
}
#endif

#endif /* __WS2812MATRIX_H */
