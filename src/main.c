#include "lcd/lcd.h"
#include  "lcd/fontx2.h"
#include "gd32vf103.h"
#include "systick.h"
#include <stdio.h>

// メインプログラム
int main( void ) {
	// LCD制御サンプルプログラム
    Lcd_Init();				// lcdを初期化する
    LCD_Clear( BLACK );		// lcdをクリアする




    LCD_ShowString( 0, 0, "JPNHN4X / MISAKI", WHITE );
    LCD_ShowString( 0, 0, "JPNHN4X / MISAKI", WHITE );


/*    LCD_ShowString( 0, 10, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", CYAN );
    LCD_ShowChar( 0, 0, 65, 0, RED );
    LCD_ShowString( 0, 30, "美咲フォントでも日本語文字列を表示できます", CYAN );
*/

    while( 1 ) {


    }
}