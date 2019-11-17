// オンボードLED制御ライブラリ Ver 1.0
// 2019/11/17 by Kyoro

#ifndef __LED_H_
#define __LED_H_

#include <stdint.h>
#include "gd32vf103.h"

// LED色指定のための記号定数
#define LED_R	1
#define LED_G	2
#define LED_B	4

// ポートの指定
#define GPIO_PORT_LED_R	GPIOC
#define GPIO_PORT_LED_G	GPIOA
#define GPIO_PORT_LED_B	GPIOA
#define LED_GPIO_CLK_R RCU_GPIOC
#define LED_GPIO_CLK_G RCU_GPIOA
#define LED_GPIO_CLK_B RCU_GPIOA
#define GPIO_PIN_LED_R	GPIO_PIN_13
#define GPIO_PIN_LED_G	GPIO_PIN_1
#define GPIO_PIN_LED_B	GPIO_PIN_2

// 関数
extern void led_init( void );
extern void led_on( uint8_t color );
extern void led_off( uint8_t color );

#endif