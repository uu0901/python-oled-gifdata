#ifndef __KEY_H
#define __KEY_H
#define MAX_KEY_COUNT           4
/* 键值确定 配合硬件上下拉进行选择 */
#define KEY_PRESSED				1
#define KEY_UNPRESSED			0
/* 双击 长按 重复的时间确定 */
#define KEY_TIME_DOUBLE			200
#define KEY_TIME_LONG			2000
#define KEY_TIME_REPEAT			100
/* 按键索引 */
#define KEY_1					0
#define KEY_2					1
#define KEY_3					2
#define KEY_4					3
/* 按键事件 */
#define KEY_HOLD				1 << 1
#define KEY_DOWN				1 << 2
#define KEY_UP					1 << 3
#define KEY_SINGLE				1 << 4
#define KEY_DOUBLE				1 << 5
#define KEY_LONG				1 << 6
#define KEY_REPEAT				1 << 7
/* 按键结构体 */
typedef struct 
{
    GPIO_TypeDef* port;       // GPIO端口
    uint16_t pin;             // 引脚编号
	uint8_t CurrState;		  //当前值
	uint8_t PrevState;		  //上一次值
	uint8_t S;				  //状态机
    uint8_t flag;             // 事件标志
    uint16_t timer;           // 计时器
} Key_TypeDef;
/* 按键配置 */
#define KEY1_GPIO_PORT                  GPIOB
#define KEY1_GPIO_PIN                   GPIO_Pin_10
#define KEY1_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}while(0)

#define KEY2_GPIO_PORT                  GPIOB
#define KEY2_GPIO_PIN                   GPIO_Pin_11
#define KEY2_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}while(0)

#define KEY3_GPIO_PORT                  GPIOA
#define KEY3_GPIO_PIN                   GPIO_Pin_11
#define KEY3_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0)

#define KEY4_GPIO_PORT                  GPIOA
#define KEY4_GPIO_PIN                   GPIO_Pin_12
#define KEY4_GPIO_CLK_ENABLE()          do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);}while(0)

#define KEY1                            GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN)
#define KEY2                            GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN)
#define KEY3                            GPIO_ReadInputDataBit(KEY3_GPIO_PORT, KEY3_GPIO_PIN)
#define KEY4                            GPIO_ReadInputDataBit(KEY4_GPIO_PORT, KEY4_GPIO_PIN)
/* 函数接口 */
void keyInit(void);
uint8_t keyCheck(uint8_t n, uint8_t Flag);
void keyTick(void);

#endif
