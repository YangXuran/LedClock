#include "ws2812Matrix.h"
#include <string.h>

/*
 * PWM刷新一帧8*32点WS2812B矩阵时间为1.25us*(80+8*32*3*8)=7780us=7.78ms
 * */

#define RESET_ZERO_NUM  80
#define WS2812_ZERO     29
#define WS2812_ONE      59

uint8_t pwmFrame[RESET_ZERO_NUM+MATRIX_ROW*MATRIX_COLUMN*3*8] = {0};	/* PWM控制器DMA帧RAM */
uint8_t (*pwmMatrix)[MATRIX_COLUMN][MATRIX_ROW][3][8] = (uint8_t (*)[MATRIX_COLUMN][MATRIX_ROW][3][8])(pwmFrame+RESET_ZERO_NUM);
rgbPoint_u matrixRam[MATRIX_ROW][MATRIX_COLUMN] = {0};	                /* LED点阵映射 */

void* getPwmDmaRamAddr(void)
{
    return (void*)pwmFrame;
}

int getPwmDmaRamSize(void)
{
    return sizeof(pwmFrame);
}

void matrix2pwm(void)
{
	int row, column, color, bit;

	for(row=0; row<MATRIX_ROW; row++)
		for(column=0; column<MATRIX_COLUMN; column++)
		    for(color=0; color<3; color++)
		        for(bit=0; bit<8; bit++)
		            if(matrixRam[row][column].color & (0x800000>>(color*8+bit)))
		                (*pwmMatrix)[column][column%2 ? (MATRIX_ROW-1-row) : row][color][bit] = WS2812_ONE;
		            else
		                (*pwmMatrix)[column][column%2 ? (MATRIX_ROW-1-row) : row][color][bit] = WS2812_ZERO;
}

void displayChar(int x, int y, const font_t *font, const rgbPoint_u fontColor)
{
    int point = 0, bit = 0;
    int row = y, column = x;

    while(point < font->width*font->hight)
    {
        for(bit=0; bit<8; bit++)
        {
            if(font->pixel[point/8] & (0x80>>bit))
                matrixRam[row][column].color = fontColor.color;
            else
                matrixRam[row][column].color = 0x0;

            column++;
            if(column == x+font->width)
            {
                row++;
                column = x;
            }
            point++;
        }
    }
}

void displayPattern(int x, int y, const pattern_t *pattern)
{
    int point = 0;
    int row = y, column = x;

    while(point < pattern->width*pattern->hight)
    {
        matrixRam[row][column].color = pattern->pixel[point].color;

        column++;
        if(column == x+pattern->width)
        {
            row++;
            column = x;
        }
        point++;
    }
}

