// オンボードLED制御ライブラリ Ver 1.0
// (C) 2019/11/17 by Kyoro

#include "led.h"

void led_init( void )
{
	// GPIOクロック供給設定
	rcu_periph_clock_enable( LED_GPIO_CLK_R );
	rcu_periph_clock_enable( LED_GPIO_CLK_G );
	// rcu_periph_clock_enable( LED_GPIO_CLK_B );

	// GPIO初期設定
	gpio_init( GPIO_PORT_LED_R, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_LED_R );
	gpio_init( GPIO_PORT_LED_G, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_LED_G );
    gpio_init( GPIO_PORT_LED_B, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_LED_B );

	// ポートにHを出力してLEDを消す
	GPIO_BOP( GPIO_PORT_LED_R ) = GPIO_PIN_LED_R;
	GPIO_BOP( GPIO_PORT_LED_G ) = GPIO_PIN_LED_G;
	GPIO_BOP( GPIO_PORT_LED_B ) = GPIO_PIN_LED_B;
}

void led_on( uint8_t color )
{
	// 指定色のLEDを点灯する
	// color = 色 (bit 0:赤、bit 1:緑、bit 2:青)
	if( color & LED_R ) {
		gpio_bit_reset( GPIO_PORT_LED_R, GPIO_PIN_LED_R );
	}
	if( color & LED_G ) {
		gpio_bit_reset( GPIO_PORT_LED_G, GPIO_PIN_LED_G );
	}
	if( color & LED_B ) {
		gpio_bit_reset( GPIO_PORT_LED_B, GPIO_PIN_LED_B );
	}
}

void led_off( uint8_t color )
{
	// 指定色のLEDを消灯する
	// color = 色 (bit 0:赤、bit 1:緑、bit 2:青)
	if( color & LED_R ) {
		gpio_bit_set( GPIO_PORT_LED_R, GPIO_PIN_LED_R );
	}
	if( color & LED_G ) {
		gpio_bit_set( GPIO_PORT_LED_G, GPIO_PIN_LED_G );
	}
	if( color & LED_B ) {
		gpio_bit_set( GPIO_PORT_LED_B, GPIO_PIN_LED_B );
	}
}
