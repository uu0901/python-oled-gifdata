#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>
#define HEX_MODE            0
#define STRING_MODE         0


#if HEX_MODE == 1
#define MAX_TX_LEN          4
#define MAX_RX_LEN          4
extern uint8_t serialTxPacket[];
extern uint8_t serialRxPacket[];
#endif

#if STRING_MODE == 1
#define MAX_RX_LEN          100
extern char serialRxPacket[MAX_RX_LEN];				//"@MSG\r\n"
#endif

#define USART_TX_GPIO_PORT                  GPIOA
#define USART_TX_GPIO_PIN                   GPIO_Pin_9
#define USART_TX_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)

#define USART_RX_GPIO_PORT                  GPIOA
#define USART_RX_GPIO_PIN                   GPIO_Pin_10
#define USART_RX_GPIO_CLK_ENABLE()          do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); }while(0)

#define USART_UX                            USART1
#define USART_UX_IRQn                       USART1_IRQn
#define USART_UX_IRQHandler                 USART1_IRQHandler
#define USART_UX_CLK_ENABLE()               do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); }while(0)

void serialInit(uint32_t BaudRate);
void serialSendByte(uint8_t Byte);
void serialSendArray(uint8_t *Array, uint16_t Length);
void serialSendString(char *String);
void serialSendNumber(uint32_t Number, uint8_t Length);
void serialPrintf(char *format, ...);

void serialSendPacket(uint8_t head, uint8_t tail, uint8_t *Array, uint16_t Length);
uint8_t serialGetRxFlag(void);

#endif
