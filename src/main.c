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
    strncpy( str, font[0].fontname, 8 );
    str[8] = 0;
    LCD_ShowString( ankfont_width * 10, 0, str, CYAN );
    LCD_ShowString( 0, FONT_HEIGHT + 1 , "ABCDEabcde01234", CYAN );
    LCD_ShowChar( 0, 40,  0x81fc, 0, CYAN);
    // LCD_ShowChar( 20, 20,  0x829f, 1, CYAN);


    // LCD_ShowString( 0, ( FONT_HEIGHT + 1 ) * 2, "ぱう", CYAN );

    while( 1 ) {
    }
}
