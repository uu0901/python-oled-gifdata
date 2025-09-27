#ifndef __PWM_H
#define __PWM_H

#define PWM_TIME                        TIM2
#define PWM_TIME_CLK_ENABLE()           do{RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);}while(0)

#define PWMA_GPIO_PORT                  GPIOA
#define PWMA_GPIO_PIN                    GPIO_Pin_0
#define PWMA_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0)

#define PWMB_GPIO_PORT                  GPIOA
#define PWMB_GPIO_PIN                    GPIO_Pin_1
#define PWMB_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0)

void pwmInit(uint16_t pre, uint16_t arr);
void pwmaSetCompare(uint16_t Compare);
void pwmbSetCompare(uint16_t Compare);
#endif
