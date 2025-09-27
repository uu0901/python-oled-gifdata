#ifndef __LED_H
#define __LED_H

#define MAX_LED_COUNT                   5

#define LED1_GPIO_PORT                  GPIOB 
#define LED1_GPIO_PIN                   GPIO_Pin_3
#define LED1_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}while(0)

#define LED2_GPIO_PORT                  GPIOC
#define LED2_GPIO_PIN                   GPIO_Pin_14
#define LED2_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);}while(0)
#define LED3_GPIO_PORT                  GPIOC
#define LED3_GPIO_PIN                   GPIO_Pin_14
#define LED3_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);}while(0)

#define LED4_GPIO_PORT                  GPIOC
#define LED4_GPIO_PIN                   GPIO_Pin_15
#define LED4_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);}while(0)

#define LED5_GPIO_PORT                  GPIOB
#define LED5_GPIO_PIN                   GPIO_Pin_4
#define LED5_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}while(0)
typedef enum 
{
    LED_MODE_OFF = 0,
    LED_MODE_ON,
    LED_MODE_BLINK_SLOW,
    LED_MODE_BLINK_FAST,
    LED_MODE_PULSE_SHORT
} LED_Mode_TypeDef;

// 定义LED结构�?
typedef struct 
{
    LED_Mode_TypeDef mode;               // 当前模式
    uint16_t counter;           // 计数�?
    uint16_t on_threshold;      // 点亮阈�?
    uint16_t total_period;      // 周期长度
    GPIO_TypeDef* port;         // GPIO�?�?
    uint16_t pin;               // 引脚编号
} LED_TypeDef;


void ledInit(void);
void ledSetMode(LED_TypeDef* led, LED_Mode_TypeDef mode);
void ledTick(void);

#endif
