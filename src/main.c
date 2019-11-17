#include "lcd/lcd.h"
#include  "lcd/fontx2.h"
#include "gd32vf103.h"
#include "systick.h"
#include <stdio.h>
#include <string.h>

// メインプログラム
int main( void ) {
	// LCD制御サンプルプログラム
    uint8_t str[10];

    LCD_Init();				// lcdを初期化する
    LCD_Clear( BLACK );		// lcdをクリアする

    LCD_ShowString( 0, 0, "Fontname:" , WHITE );
    strcpy( str, font[0].fontname );
    str[8] = 0;
    LCD_ShowString( 80, 0, str, CYAN );

    LCD_ShowString( 0, 10, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", CYAN );

    LCD_ShowString( 0, 30, "あ", CYAN );

/*
    LCD_ShowString( 0, 30, "美咲フォントでも日本語文字列を表示できます", CYAN );
*/
    LCD_ShowChar( 0, 40, 0x8341, 0, YELLOW );

    while( 1 ) {


    }
}
