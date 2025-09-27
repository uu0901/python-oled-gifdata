#ifndef __ENCODER_H
#define __ENCODER_H

#define A_ENCODER_TIME                         TIM3
#define A_ENCODER_TIME_CLK_ENABLE()            do{RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);}while(0)

#define A_ENCODERA_GPIO_PORT                   GPIOA
#define A_ENCODERA_GPIO_PIN                    GPIO_Pin_6
#define A_ENCODERA_GPIO_CLK_ENABLE()           do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0)

#define A_ENCODERB_GPIO_PORT                   GPIOA
#define A_ENCODERB_GPIO_PIN                    GPIO_Pin_7
#define A_ENCODERB_GPIO_CLK_ENABLE()           do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0)

#define B_ENCODER_TIME                         TIM4
#define B_ENCODER_TIME_CLK_ENABLE()            do{RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);}while(0)

#define B_ENCODERA_GPIO_PORT                   GPIOB
#define B_ENCODERA_GPIO_PIN                    GPIO_Pin_6
#define B_ENCODERA_GPIO_CLK_ENABLE()           do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}while(0)

#define B_ENCODERB_GPIO_PORT                   GPIOB
#define B_ENCODERB_GPIO_PIN                    GPIO_Pin_7
#define B_ENCODERB_GPIO_CLK_ENABLE()           do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}while(0)

void encoderAInit(void);
void encoderBInit(void);
int16_t encoderAGet(void);
int16_t encoderBGet(void);
#endif
