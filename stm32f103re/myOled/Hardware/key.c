#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "serial.h"
Key_TypeDef g_keys[MAX_KEY_COUNT];
static uint8_t g_key_count = 0;
/*
	7	
	6	KEY_REPEAT		重复		按键长按后，每隔重复时间阈值置一次1，直到按键松开
	5	KEY_LONG		长按		按键按住不放，超过长按时间阈值的时刻置1
	4	KEY_DOUBLE		双击		按键按下松开后，在双击时间阈值内再次按下的时刻置1
	3	KEY_SINGLE		单击	    按键按下松开后，没有再次按下，超过双击时间阈值的时刻置1
	2	KEY_UP			松开时刻	按键松开的时刻置1
	1	KEY_DOWN		按下时刻	按键按下的时刻置1
	0	KEY_HOLD		按住不放	按键按住不放时置1，按键松开时置0

note:
	HOLD、DOWN、UP，在任何时刻，只要检测到对应的事件，就会置标志位
	SINGLE、DOUBLE、LONG/REPEAT，三者互斥，一次完整的按键流程，只会置其中一类标志位
	HOLD自动置1和清0，其余标志位在检测到指定事件的时刻置1，读后清0

	S=0 	空闲，检测按键按下  ->(按键按下，设定长按时间)->		S=1		按键已经按下，检测按键松开，等待长按时间	->(按键松开，设定双击时间)->	S=2		按键已经松开，检测按键按下，等待双击时间	->(双击时间到，SINGLE=1）
	|																			V																				V										|
	|																（长按时间到，设定重复时间，LONG=1）									              （按键按下，DOUBLE=1）						    |
	|																			|																				|										|
	|																			V																				V										|
	|														--->S=4		按键已经长按，检测按键松开，等待重复时间	->(按键松开)					S=3		按键已经双击，检测按键松开						  |
	|														|					|									|											|										|
	|														|					V								    |											V										|
	|														|		（重复时间到，设定重复时间，REPEAT=1）			   |										（按键松开）								|
	|														|					|									|											|										|
	|														--------------------									|											|										|
	 ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------											
*/
void keyInit(void)
{
	// 启用所有按键的GPIO时钟
    KEY1_GPIO_CLK_ENABLE();
    KEY2_GPIO_CLK_ENABLE();
    KEY3_GPIO_CLK_ENABLE();
    KEY4_GPIO_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // 注册并配置按键
    if (g_key_count < MAX_KEY_COUNT) 
	{
        g_keys[g_key_count].port = KEY1_GPIO_PORT;
        g_keys[g_key_count].pin = KEY1_GPIO_PIN;
        GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
        GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
        g_key_count++;
    }

    if (g_key_count < MAX_KEY_COUNT) 
	{
        g_keys[g_key_count].port = KEY2_GPIO_PORT;
        g_keys[g_key_count].pin = KEY2_GPIO_PIN;
        GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
        GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);
        g_key_count++;
    }

    if (g_key_count < MAX_KEY_COUNT) 
	{
        g_keys[g_key_count].port = KEY3_GPIO_PORT;
        g_keys[g_key_count].pin = KEY3_GPIO_PIN;
        GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN;
        GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);
        g_key_count++;
    }

    if (g_key_count < MAX_KEY_COUNT) 
	{
        g_keys[g_key_count].port = KEY4_GPIO_PORT;
        g_keys[g_key_count].pin = KEY4_GPIO_PIN;
        GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN;
        GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);
        g_key_count++;
//				serialPrintf("aaa");
    }
}

uint8_t keyGetState(uint8_t n)
{
    if (GPIO_ReadInputDataBit(g_keys[n].port, g_keys[n].pin) == 0) 
	{
        return KEY_PRESSED;
    } 
	else 
	{
        return KEY_UNPRESSED;
    }
}

uint8_t keyCheck(uint8_t n, uint8_t flag)
{
    if (g_keys[n].flag & flag) 
	{
        if (flag != KEY_HOLD) {
            g_keys[n].flag &= ~flag;  // 清除非保持型标志
        }
        return 1;
    }
    return 0;
}

void keyTick(void)
{
	static uint8_t Count, i;	
	for (i = 0; i < MAX_KEY_COUNT; i ++)
	{
		if (g_keys[i].timer > 0)
		{
			g_keys[i].timer --;
		}
	}
	
	Count ++;
	if (Count >= 20)
	{
		Count = 0;
		
		for (i = 0; i < MAX_KEY_COUNT; i ++)
		{
			g_keys[i].PrevState = g_keys[i].CurrState;
			g_keys[i].CurrState = keyGetState(i);
			
			if (g_keys[i].CurrState == KEY_PRESSED)
			{
				g_keys[i].flag |= KEY_HOLD;
			}
			else
			{
				g_keys[i].flag &= ~KEY_HOLD;
			}
			
			if (g_keys[i].CurrState == KEY_PRESSED && g_keys[i].PrevState == KEY_UNPRESSED)
			{
				g_keys[i].flag |= KEY_DOWN;
			}
			
			if (g_keys[i].CurrState == KEY_UNPRESSED && g_keys[i].PrevState == KEY_PRESSED)
			{
				g_keys[i].flag |= KEY_UP;
			}
			
			if (g_keys[i].S == 0)
			{
				if (g_keys[i].CurrState == KEY_PRESSED)
				{
					g_keys[i].timer = KEY_TIME_LONG;
					g_keys[i].S= 1;
				}
			}
			else if (g_keys[i].S == 1)
			{
				if (g_keys[i].CurrState == KEY_UNPRESSED)
				{
					g_keys[i].timer = KEY_TIME_DOUBLE;
					g_keys[i].S = 2;
				}
				else if (g_keys[i].timer == 0)
				{
					g_keys[i].timer = KEY_TIME_REPEAT;
					g_keys[i].flag |= KEY_LONG;
					g_keys[i].S = 4;
				}
			}
			else if (g_keys[i].S == 2)
			{
				if (g_keys[i].CurrState == KEY_PRESSED)
				{
					g_keys[i].flag |= KEY_DOUBLE;
					g_keys[i].S = 3;
				}
				else if (g_keys[i].timer == 0)
				{
					g_keys[i].flag |= KEY_SINGLE;
					g_keys[i].S = 0;
				}
			}
			else if (g_keys[i].S == 3)
			{
				if (g_keys[i].CurrState == KEY_UNPRESSED)
				{
					g_keys[i].S = 0;
				}
			}
			else if (g_keys[i].S == 4)
			{
				if (g_keys[i].CurrState == KEY_UNPRESSED)
				{
					g_keys[i].S = 0;
				}
				else if (g_keys[i].timer == 0)
				{
					g_keys[i].timer = KEY_TIME_REPEAT;
					g_keys[i].flag |= KEY_REPEAT;
					g_keys[i].S = 4;
				}
			}
		}
	}
}
