#include "stm32f10x.h"                  // Device header
#include "pwm.h"

//20khz
//此定时器的周期是0.05ms 所以定时器的更新频率为1/0.05ms=20khz
void pwmInit(uint16_t pre, uint16_t arr)// pre 36-1 arr 100-1
{
	PWM_TIME_CLK_ENABLE();
	PWMA_GPIO_CLK_ENABLE();
	PWMB_GPIO_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

	GPIO_InitStructure.GPIO_Pin = PWMA_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PWMA_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PWMB_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PWMB_GPIO_PORT, &GPIO_InitStructure);

	TIM_InternalClockConfig(PWM_TIME);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = arr;			//ARR 	100*0.5us=50us=0.05ms
	TIM_TimeBaseInitStructure.TIM_Prescaler = pre;		//PSC   72M/36=2Mhz     0.5us
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(PWM_TIME, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR 
	TIM_OC1Init(PWM_TIME, &TIM_OCInitStructure);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR 
	TIM_OC2Init(PWM_TIME, &TIM_OCInitStructure);
	
	/*
    TIM_Pulse 是比较寄存器（CCR）的值。
    在 PWM 模式 1（TIM_OCMode_PWM1） 下：
	当计数器值小于 CCR 时，输出高电平；
	当计数器值大于或等于 CCR 时，输出低电平。

所以：
	如果你设置 CCR = 50，那么：
	定时器从 0 到 49 时输出高电平（共 50 个周期）
	定时器从 50 到 99 时输出低电平（共 50 个周期）

	?? 注意：虽然最大值是 99，但可以设置 TIM_Pulse = 100，此时在整个周期中都保持高电平（占空比为 100%）
	占空比	TIM_Pulse 值
	0%		0
	25%		25
	50%		50
	75%		75
	100%	100
	*/
	TIM_Cmd(TIM2, ENABLE);
}

void pwmaSetCompare(uint16_t Compare)
{
	TIM_SetCompare1(PWM_TIME, Compare);
}
void pwmbSetCompare(uint16_t Compare)
{
	TIM_SetCompare2(PWM_TIME, Compare);
}
