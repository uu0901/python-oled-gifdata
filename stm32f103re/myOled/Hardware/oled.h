#ifndef __OLED_H
#define __OLED_H

#include <stdint.h>
#include "OLED_Data.h"


#define OLED_SCL_GPIO_PORT                  GPIOB
#define OLED_SCL_GPIO_PIN                   GPIO_Pin_8
#define OLED_SCL_CLK_ENABLE()               do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0)

#define OLED_SDA_GPIO_PORT                  GPIOB
#define OLED_SDA_GPIO_PIN                   GPIO_Pin_9
#define OLED_SDA_CLK_ENABLE()               do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0)
/*参数宏定义*********************/

/*FontSize参数取值*/
/*此参数值不仅用于判断，而且用于计算横向字符偏移，默认值为字体像素宽度*/
#define OLED_8X16				8
#define OLED_6X8				6

/*IsFilled参数数值*/
#define OLED_UNFILLED			0
#define OLED_FILLED				1

/*********************参数宏定义*/


/*函数声明*********************/

/*初始化函数*/
void oledInit(void);

/*更新函数*/
void oledUpdate(void);
void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显存控制函数*/
void oledClear(void);
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);
void oledReverse(void);
void oledReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显示函数*/
void oledShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize);
void oledShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize);
void oledShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void oledShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);
void oledShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void oledShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void oledShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);
void oledShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);
void oledShowImageAddInvert(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image, uint8_t Invert);
void oledPrintf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...);

/*绘图函数*/
void oledDrawPoint(int16_t X, int16_t Y);
uint8_t oledGetPoint(int16_t X, int16_t Y);
void oledDrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void oledDrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);
void oledDrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);
void oledDrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled);
void oledDrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);
void oledDrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);

/*********************函数声明*/

#endif


/*****************江协科技|版权所有****************/
/*****************jiangxiekeji.com*****************/
