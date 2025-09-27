#include "stm32f10x.h"                  // Device header
#include "adc.h"
/*
多通道

	单次转换
	非扫描
*/
void adcInit(void)
{
	// ADC为12位精度
	ADC_CLK_ENABLE();
	ADC_CH2_GPIO_CLK_ENABLE();
	ADC_CH3_GPIO_CLK_ENABLE();
	ADC_CH4_GPIO_CLK_ENABLE();
	ADC_CH5_GPIO_CLK_ENABLE();
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//ADC最高14Mhz  APB2 72Mhz  6分频之后为12Mhz满足要求
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;

	GPIO_InitStructure.GPIO_Pin = ADC_CH2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADC_CH2_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADC_CH3_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADC_CH3_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADC_CH4_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADC_CH4_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = ADC_CH5_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ADC_CH5_GPIO_PORT, &GPIO_InitStructure);

	/*
	数据对齐：寄存器为16位
	右对齐就是将12为的ADC往右靠，高位多出来补0，直接读出来直接读出来就是数据了
	左对齐是将12位的ADC左靠，低位多出来补0，直接读出来数值偏大，因为左移了4位置，
	二进制做移一位就是x2，所以这里读出来的数值是扩大了16倍，这样的好处是可以选择低分辨率，把数据的高8位取出来，这样12位的ADC就变成了8位的ADC

	单次转换，转换结束后就停下，再次转换需要重新调用
	连续转换，转换结束后会继续转换

	非扫描，不会用到菜单列表，只会选择第一个作为转换，在adc结构体的通道数目，在非扫描模式下次参数无意义，无论写了多少数目，都只有序列1有效
	扫描，用到菜单列表，会有一个通道数目可以配置，如7，那么就是只看前7个位置，每次触发之后，就会对前7个位置进行AD转换，转换结果只放在1个数据寄存器里，为了防止被覆盖，就需要用DMA及时把数据挪走【扫描模式DMA用的多】
	*/
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//独立模式 ADC1和ADC2无配合
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//数据右对齐 ADC数据取低12位
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//无外部触发
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//单次
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描
	ADC_InitStructure.ADC_NbrOfChannel = 1;//这里关注是不是扫描模式，如果非扫描，也只能填1，扫描模式则得按需求填写
	ADC_Init(ADCX, &ADC_InitStructure);
	
	ADC_Cmd(ADCX, ENABLE);
	
	ADC_ResetCalibration(ADCX);
	while (ADC_GetResetCalibrationStatus(ADCX) == SET);
	ADC_StartCalibration(ADCX);
	while (ADC_GetCalibrationStatus(ADCX) == SET);
}

uint16_t adcGetValue(uint8_t n)
{
	if (n == 1)
	{
		ADC_RegularChannelConfig(ADCX, ADC_Channel_2, 1, ADC_SampleTime_55Cycles5);
	}
	else if (n == 2)
	{
		ADC_RegularChannelConfig(ADCX, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);
	}
	else if (n == 3)
	{
		ADC_RegularChannelConfig(ADCX, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5);
	}
	else if (n == 4)
	{
		ADC_RegularChannelConfig(ADCX, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5);
	}
	
	//这里读取ADC需要每次调用需要软件触发和等待标志位，是因为是单次转换
	/*
	若为连续转换就不需要，直接返回值就行
	连续转换，仅需要在最开始触发一次就行，这时内部的ADC就会一次接着一次、连续不断地对指定的通道进行转换，
	转换结果放在数据寄存器里，此时数据寄存器就会不断地刷新最新的转换结果所以在获取ad值这里就不需要判断标志位，直接返回数据寄存器的值
	*/
	ADC_SoftwareStartConvCmd(ADCX, ENABLE);		//软件触发，因为选择无外部触发
	while (ADC_GetFlagStatus(ADCX, ADC_FLAG_EOC) == RESET);//等待规则组转换完成
	return ADC_GetConversionValue(ADCX);//每次读取ADC的DR寄存器会自动清除EOC标志位，无需再次清除标志位
}
