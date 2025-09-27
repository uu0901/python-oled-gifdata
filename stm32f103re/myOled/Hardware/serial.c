#include "stm32f10x.h"                  // Device header
#include "Serial.h"
#include <stdio.h>
#include <stdarg.h>

#if HEX_MODE == 1
uint8_t serialTxPacket[MAX_TX_LEN];				//FF 01 02 03 04 FE
uint8_t serialRxPacket[MAX_RX_LEN];
#endif

#if STRING_MODE == 1
char serialRxPacket[100];				//"@MSG\r\n"
#endif


uint8_t serialRxFlag;
void serialInit(uint32_t BaudRate)
{
	USART_UX_CLK_ENABLE();
	USART_TX_GPIO_CLK_ENABLE();
	USART_RX_GPIO_CLK_ENABLE();
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART_UX, &USART_InitStructure);
	
	USART_ITConfig(USART_UX, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART_UX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART_UX, ENABLE);
}

void serialSendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void serialSendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		serialSendByte(Array[i]);
	}
}

void serialSendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		serialSendByte(String[i]);
	}
}

uint32_t serialPow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void serialSendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		serialSendByte(Number / serialPow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	serialSendByte(ch);
	return ch;
}

void serialPrintf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	serialSendString(String);
}


void serialSendPacket(uint8_t head, uint8_t tail, uint8_t *Array, uint16_t Length)
{
	serialSendByte(head);
	serialSendArray(Array, Length);
	serialSendByte(tail);
	// serialSendByte(0x0D);
	// serialSendByte(0x0A);
	/*
	换行是文本模式的一种体现
	*/
}

uint8_t serialGetRxFlag(void)
{
	if (serialRxFlag == 1)
	{
		serialRxFlag = 0;
		return 1;
	}
	return 0;
}
#if HEX_MODE == 1
void USART1_IRQHandler(void)
{
	static uint8_t rxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(USART_UX, USART_IT_RXNE) == SET)
	{
		uint8_t rxData = USART_ReceiveData(USART_UX);
		
		if (rxState == 0)
		{
			if (rxData == 0xFF)
			{
				rxState = 1;
				pRxPacket = 0;
			}
		}
		else if (rxState == 1)
		{
			serialRxPacket[pRxPacket] = rxData;
			pRxPacket ++;
			if (pRxPacket >= 4)
			{
				rxState = 2;
			}
		}
		else if (rxState == 2)
		{
			if (rxData == 0xFE)
			{
				rxState = 3;
			}
		}
		else if (rxState == 3)
		{
			if (rxData == 0x0D)
			{
				rxState = 4;
			}
		}
		else if (rxState == 4)
		{
			if (rxData == 0x0A)
			{
				rxState = 0;
				serialRxFlag = 1;
			}
		}
		USART_ClearITPendingBit(USART_UX, USART_IT_RXNE);
	}
}
#endif

#if STRING_MODE == 1
void USART1_IRQHandler(void)
{
	static uint8_t rxState = 0;
	static uint8_t pRxPacket = 0;
	if (USART_GetITStatus(USART_UX, USART_IT_RXNE) == SET)
	{
		uint8_t rxData = USART_ReceiveData(USART_UX);
		
		if (rxState == 0)
		{
			if (rxData == '@' && serialRxFlag == 0)
			{
				rxState = 1;
				pRxPacket = 0;
			}
		}
		else if (rxState == 1)
		{
			if (rxData == '\r')
			{
				rxState = 2;
			}
			else
			{
				serialRxPacket[pRxPacket] = rxData;
				pRxPacket ++;
			}
		}
		else if (rxState == 2)
		{
			if (rxData == '\n')
			{
				rxState = 0;
				serialRxPacket[pRxPacket] = '\0';
				serialRxFlag = 1;
			}
		}
		
		USART_ClearITPendingBit(USART_UX, USART_IT_RXNE);
	}
}
#endif