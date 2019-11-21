// LCD描画ライブラリ FONTX2対応版 Ver 1.0beta1
// Original library developed by Sipeed.
// 2019/11/19 modified by Kyoro
//
// このライブラリはsipeed社提供のLCD表示サンプルプログラムを一部改変して作成しました

#ifndef __LCD_H
#define __LCD_H		

#include <stdint.h>
#include "systick.h"
#include "stdlib.h"	
#include "gd32vf103_gpio.h"
#include "lcd/fontx2.h"			// FONTX2ライブラリ

#define USE_HORIZONTAL	2		// 表示向き(0-1:縦、2-3:横)
#define HAS_BLK_CNTL	0

// FONTX2 の設定
#define USE_FONTX2	2				// FONTX2の設定(0:不使用 1:2バイト文字のみ 2:1バイト文字も)
#define AFONT "PAW16A.FNT"			// 1バイトフォントファイル名(ぱうフォント半角)
#define KFONT "PAW16K.FNT"			// 2バイトフォントファイル名(ぱうフォント全角)
// #define AFONT "JPNHN4X.FNT"		// 1バイトフォントファイル名(8x4フォント)
// #define KFONT "MISAKI.FNT"		// 2バイトフォントファイル名(美咲フォント)
#define ASPACE		0				// FONTX2 1バイトフォントの横方向スペーシング
#define KSPACE		0				// FONTX2 2バイトフォントの横方向スペーシング
#define FONT_HEIGHT 17				// 自動改行時の1行の高さ
#define USE_UTF8STR					// UTF-8の文字列を扱う場合に定義(変換テーブルが必要なためプログラムサイズが大きくなります)
#define FONTX2_USELED				// LEDをステータス表示に使用する場合に定義(赤：フォント読み込みエラー　緑：SDアクセス中)

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 80
#define LCD_H 160
#else
#define LCD_W 160
#define LCD_H 80
#endif

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;    			

#ifdef FONTX2_USELED	// LED制御ライブラリ
#include "led.h"
#endif
#ifdef USE_UTF8STR		// Uicode-SJIS変換ライブラリ
#include "unicode.h"
#endif

#define SPI0_CFG 1  //hardware spi
// #define SPI0_CFG 2  //hardware spi dma
// #define SPI0_CFG 3  //software spi

#define FRAME_SIZE  25600

//　OLED接続設定
#if SPI0_CFG == 1
#define OLED_SCLK_Clr() 
#define OLED_SCLK_Set() 

#define OLED_SDIN_Clr()
#define OLED_SDIN_Set()

#define OLED_CS_Clr() gpio_bit_reset(GPIOB,GPIO_PIN_2)     //CS PB2
#define OLED_CS_Set() gpio_bit_set(GPIOB,GPIO_PIN_2)
#elif SPI0_CFG == 2
#define OLED_SCLK_Clr() 
#define OLED_SCLK_Set() 

#define OLED_SDIN_Clr()
#define OLED_SDIN_Set()

#define OLED_CS_Clr()
#define OLED_CS_Set()
#else /* SPI0_CFG */
#define OLED_SCLK_Clr() gpio_bit_reset(GPIOA,GPIO_PIN_5)    //CLK PA5
#define OLED_SCLK_Set() gpio_bit_set(GPIOA,GPIO_PIN_5)

#define OLED_SDIN_Clr() gpio_bit_reset(GPIOA,GPIO_PIN_7)    //DIN PA7
#define OLED_SDIN_Set() gpio_bit_set(GPIOA,GPIO_PIN_7)

#define OLED_CS_Clr()  gpio_bit_reset(GPIOB,GPIO_PIN_2)     //CS PB2
#define OLED_CS_Set()  gpio_bit_set(GPIOB,GPIO_PIN_2)
#endif /* SPI0_CFG */

#define OLED_RST_Clr() gpio_bit_reset(GPIOB,GPIO_PIN_1)     //RES PB1
#define OLED_RST_Set() gpio_bit_set(GPIOB,GPIO_PIN_1)

#define OLED_DC_Clr() gpio_bit_reset(GPIOB,GPIO_PIN_0)      //DC PB0
#define OLED_DC_Set() gpio_bit_set(GPIOB,GPIO_PIN_0)


#if     HAS_BLK_CNTL
#define OLED_BLK_Clr()  gpio_bit_reset(GPIOA,GPIO_PIN_5)//BLK
#define OLED_BLK_Set()  gpio_bit_set(GPIOA,GPIO_PIN_5)
#else
#define OLED_BLK_Clr()
#define OLED_BLK_Set()
#endif

#define OLED_CMD  0	// LCDコマンド
#define OLED_DATA 1	// LCDデータ

extern uint16_t BACK_COLOR;			// 背景色
extern uint8_t ankfont_width;		// 半角フォントの横幅
extern unsigned char image[12800];

void LCD_Writ_Bus(u8 dat);
void LCD_WR_DATA8(u8 dat);
void LCD_WR_DATA(u16 dat);
void LCD_WR_REG(u8 dat);
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);
extern void LCD_Init(void);		// renamed
extern void LCD_Clear(u16 Color);
extern void LCD_ShowChinese(u16 x,u16 y,u8 index,u8 size,u16 color);
extern void LCD_DrawPoint(u16 x,u16 y,u16 color);
extern void LCD_DrawPoint_big(u16 x,u16 y,u16 color);
extern void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
extern void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);
extern void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
extern void LCD_DrawCircle(u16 x0,u16 y0,u8 r,u16 color);		// renamed
extern uint8_t LCD_ShowChar(u16 x,u16 y,u16 num,u8 mode,u16 color);
extern void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 color);
u32 mypow(u8 m,u8 n);
extern void LCD_ShowNum(u16 x,u16 y,u16 num,u8 len,u16 color);
extern void LCD_ShowNum1(u16 x,u16 y,float num,u8 len,u16 color);
extern void LCD_ShowHex(u16 x,u16 y,u16 num,u8 len,u16 color);		// new funtcion
extern void LCD_ShowPicture(u16 x1,u16 y1,u16 x2,u16 y2);
extern void LCD_ShowLogo(void);
#define Lcd_Init() LCD_Init()	// old name for compatibility
#define Draw_Circle(x0,y0,r,color) LCD_DrawCircle(x0,y0,r,color)	// old name for compatibility

// LCD_ShowChar 関数の戻り値
#define SC_UNDEF	0	// 未定義文字
#define SC_NL		254	// 改行が必要
#define SC_OVER		255	// 表示領域外

// 表示色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40
#define BRRED 			 0XFC07
#define GRAY  			 0X8430

#define DARKBLUE      	 0X01CF
#define LIGHTBLUE      	 0X7D7C
#define GRAYBLUE       	 0X5458
 
#define LIGHTGREEN     	 0X841F
#define LGRAY 			 0XC618

#define LGRAYBLUE        0XA651
#define LBBLUE           0X2B12
					  		 
#endif  

