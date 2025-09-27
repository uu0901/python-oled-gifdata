#ifndef __MOTOR_H
#define __MOTOR_H
#include "PWM.h"

#define AIN1_GPIO_PORT                  GPIOB 
#define AIN1_GPIO_PIN                   GPIO_Pin_12
#define AIN1_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}while(0)

#define AIN2_GPIO_PORT                  GPIOB 
#define AIN2_GPIO_PIN                   GPIO_Pin_13
#define AIN2_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}while(0)

#define BIN1_GPIO_PORT                  GPIOB 
#define BIN1_GPIO_PIN                   GPIO_Pin_14
#define BIN1_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}while(0)

#define BIN2_GPIO_PORT                  GPIOB 
#define BIN2_GPIO_PIN                   GPIO_Pin_15
#define BIN2_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}while(0)

#define AIN1_SET_HEIGH()                do{GPIO_SetBits(AIN1_GPIO_PORT, AIN1_GPIO_PIN);}while(0)
#define AIN1_SET_LOW()                  do{GPIO_ResetBits(AIN1_GPIO_PORT, AIN1_GPIO_PIN);}while(0)

#define AIN2_SET_HEIGH()                do{GPIO_SetBits(AIN2_GPIO_PORT, AIN2_GPIO_PIN);}while(0)
#define AIN2_SET_LOW()                  do{GPIO_ResetBits(AIN2_GPIO_PORT, AIN2_GPIO_PIN);}while(0)

#define BIN1_SET_HEIGH()                do{GPIO_SetBits(BIN1_GPIO_PORT, BIN1_GPIO_PIN);}while(0)
#define BIN1_SET_LOW()                  do{GPIO_ResetBits(BIN1_GPIO_PORT, BIN1_GPIO_PIN);}while(0)

#define BIN2_SET_HEIGH()                do{GPIO_SetBits(BIN2_GPIO_PORT, BIN2_GPIO_PIN);}while(0)
#define BIN2_SET_LOW()                  do{GPIO_ResetBits(BIN2_GPIO_PORT, BIN2_GPIO_PIN);}while(0)

void motorABInit(void);
void motorA_setPwm(int8_t PWM);
void motorB_setPwm(int8_t pwm);
#endif
