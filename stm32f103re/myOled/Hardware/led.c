#include "stm32f10x.h"
#include "led.h"

// 全局 LED 实例数组（最多支持 4 个 LED）
LED_TypeDef g_leds[MAX_LED_COUNT];
static uint8_t g_led_count = 0;

// 模式参数配置表（保持不变）
const struct 
{
    uint16_t on_threshold;
    uint16_t total_period;
} mode_config[] = {
    {0, 0},                   // LED_MODE_OFF
    {0, 0},                   // LED_MODE_ON
    {500, 1000},              // LED_MODE_BLINK_SLOW
    {50, 100},                // LED_MODE_BLINK_FAST
    {100, 1000}               // LED_MODE_PULSE_SHORT
};

// 初始化函数：注册并配置所有 LED
void ledInit(void)
{
    LED1_GPIO_CLK_ENABLE();
    LED2_GPIO_CLK_ENABLE();
    LED3_GPIO_CLK_ENABLE();
    LED4_GPIO_CLK_ENABLE();
    LED5_GPIO_CLK_ENABLE();
    // 定义 GPIO 配置结构体
    GPIO_InitTypeDef GPIO_InitStructure;

    // 注册并配置 LED1（GPIOC, Pin14）
    if (g_led_count < MAX_LED_COUNT) 
    {
        g_leds[g_led_count].mode = LED_MODE_OFF;
        g_leds[g_led_count].counter = 0;
        g_leds[g_led_count].on_threshold = mode_config[LED_MODE_OFF].on_threshold;
        g_leds[g_led_count].total_period = mode_config[LED_MODE_OFF].total_period;
        g_leds[g_led_count].port = LED1_GPIO_PORT;
        g_leds[g_led_count].pin = LED1_GPIO_PIN;

        // 配置 GPIOC_Pin14
        GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
        GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
        g_led_count++;
    }

    if (g_led_count < MAX_LED_COUNT) 
    {
        g_leds[g_led_count].mode = LED_MODE_OFF;
        g_leds[g_led_count].counter = 0;
        g_leds[g_led_count].on_threshold = mode_config[LED_MODE_OFF].on_threshold;
        g_leds[g_led_count].total_period = mode_config[LED_MODE_OFF].total_period;
        g_leds[g_led_count].port = LED2_GPIO_PORT;
        g_leds[g_led_count].pin = LED2_GPIO_PIN;

        // 配置 GPIOC_Pin14
        GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
        GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
        g_led_count++;
    }
    if (g_led_count < MAX_LED_COUNT) 
    {
        g_leds[g_led_count].mode = LED_MODE_OFF;
        g_leds[g_led_count].counter = 0;
        g_leds[g_led_count].on_threshold = mode_config[LED_MODE_OFF].on_threshold;
        g_leds[g_led_count].total_period = mode_config[LED_MODE_OFF].total_period;
        g_leds[g_led_count].port = LED3_GPIO_PORT;
        g_leds[g_led_count].pin = LED3_GPIO_PIN;

        // 配置 GPIOC_Pin14
        GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);
        GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
        g_led_count++;
    }
    if (g_led_count < MAX_LED_COUNT) 
    {
        g_leds[g_led_count].mode = LED_MODE_OFF;
        g_leds[g_led_count].counter = 0;
        g_leds[g_led_count].on_threshold = mode_config[LED_MODE_OFF].on_threshold;
        g_leds[g_led_count].total_period = mode_config[LED_MODE_OFF].total_period;
        g_leds[g_led_count].port = LED4_GPIO_PORT;
        g_leds[g_led_count].pin = LED4_GPIO_PIN;

        // 配置 GPIOC_Pin14
        GPIO_InitStructure.GPIO_Pin = LED4_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStructure);
        GPIO_SetBits(LED4_GPIO_PORT, LED4_GPIO_PIN);
        g_led_count++;
    }
    if (g_led_count < MAX_LED_COUNT) 
    {
        g_leds[g_led_count].mode = LED_MODE_OFF;
        g_leds[g_led_count].counter = 0;
        g_leds[g_led_count].on_threshold = mode_config[LED_MODE_OFF].on_threshold;
        g_leds[g_led_count].total_period = mode_config[LED_MODE_OFF].total_period;
        g_leds[g_led_count].port = LED5_GPIO_PORT;
        g_leds[g_led_count].pin = LED5_GPIO_PIN;

        // 配置 GPIOC_Pin14
        GPIO_InitStructure.GPIO_Pin = LED5_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(LED5_GPIO_PORT, &GPIO_InitStructure);
        GPIO_SetBits(LED5_GPIO_PORT, LED5_GPIO_PIN);
        g_led_count++;
    }
}

// 设置 LED 模式
void ledSetMode(LED_TypeDef* led, LED_Mode_TypeDef mode)
{
    if (mode != led->mode) 
    {
        led->mode = mode;
        led->counter = 0;

//        if (mode >= sizeof(mode_config) / sizeof(mode_config[0])) 
//				{
//            mode = LED_MODE_OFF;
//        }
        led->on_threshold = mode_config[mode].on_threshold;
        led->total_period = mode_config[mode].total_period;
    }
}

// 更新 LED 状态
void ledUpdate(LED_TypeDef* led)
{
    if (led->mode == LED_MODE_ON) 
    {
        GPIO_ResetBits(led->port, led->pin);
    } 
    else if (led->mode == LED_MODE_OFF) 
    {
        GPIO_SetBits(led->port, led->pin);
    } 
    else 
    {
        led->counter++;
        if (led->counter >= led->total_period) 
        {
            led->counter = 0;
        }
        if (led->counter < led->on_threshold)
        {
            GPIO_ResetBits(led->port, led->pin);
        } 
        else 
        {
            GPIO_SetBits(led->port, led->pin);
        }
    }
}

// 主要的定时更新函数
void ledTick(void)
{
    for (uint8_t i = 0; i < g_led_count; i++) {
        ledUpdate(&g_leds[i]);
    }
}
