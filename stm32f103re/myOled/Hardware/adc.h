#ifndef __RP_H
#define __RP_H


#define ADCX                            ADC2
#define ADC_CLK_ENABLE()                do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);}while(0)

#define ADC_CH2_GPIO_PORT               GPIOA
#define ADC_CH2_GPIO_PIN                GPIO_Pin_2
#define ADC_CH2_GPIO_CLK_ENABLE()       do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0)

#define ADC_CH3_GPIO_PORT               GPIOA
#define ADC_CH3_GPIO_PIN                GPIO_Pin_3
#define ADC_CH3_GPIO_CLK_ENABLE()       do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0)

#define ADC_CH4_GPIO_PORT               GPIOA
#define ADC_CH4_GPIO_PIN                GPIO_Pin_4
#define ADC_CH4_GPIO_CLK_ENABLE()       do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0)

#define ADC_CH5_GPIO_PORT               GPIOA
#define ADC_CH5_GPIO_PIN                GPIO_Pin_5
#define ADC_CH5_GPIO_CLK_ENABLE()       do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0)

void adcInit(void);
uint16_t adcGetValue(uint8_t n);

#endif
