#include "stm32f10x.h"                  // Device header
#include "encoder.h"
void encoderAInit(void)
{
	A_ENCODER_TIME_CLK_ENABLE(); 
	A_ENCODERA_GPIO_CLK_ENABLE(); 
	A_ENCODERB_GPIO_CLK_ENABLE(); 	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = A_ENCODERA_GPIO_PIN;
	GPIO_Init(A_ENCODERA_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = A_ENCODERB_GPIO_PIN;
	GPIO_Init(A_ENCODERB_GPIO_PORT, &GPIO_InitStructure);	

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(A_ENCODER_TIME, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(A_ENCODER_TIME, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(A_ENCODER_TIME, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(A_ENCODER_TIME, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
	
	TIM_Cmd(A_ENCODER_TIME, ENABLE);
}
void encoderBInit(void)
{
	B_ENCODER_TIME_CLK_ENABLE(); 
	B_ENCODERA_GPIO_CLK_ENABLE(); 
	B_ENCODERB_GPIO_CLK_ENABLE(); 	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin = B_ENCODERA_GPIO_PIN;
	GPIO_Init(B_ENCODERA_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = B_ENCODERB_GPIO_PIN;
	GPIO_Init(B_ENCODERB_GPIO_PORT, &GPIO_InitStructure);	

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(B_ENCODER_TIME, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(B_ENCODER_TIME, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(B_ENCODER_TIME, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(B_ENCODER_TIME, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Rising);//调换可改变极性
	
	TIM_Cmd(B_ENCODER_TIME, ENABLE);
}

int16_t encoderAGet(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(A_ENCODER_TIME);
	TIM_SetCounter(A_ENCODER_TIME, 0);
	return Temp;
}

int16_t encoderBGet(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(B_ENCODER_TIME);
	TIM_SetCounter(B_ENCODER_TIME, 0);
	return Temp;
}
