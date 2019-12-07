// LCD描画ライブラリ FONTX2対応版 Ver 1.0
// Original library developed by Sipeed.
// 2019/12/1 modified by Kyoro
//
// このライブラリはsipeed社提供のLCD表示サンプルプログラムを一部改変して作成しました

#include "lcd/lcd.h"
#include "lcd/oledfont.h"		// デフォルトのフォント
#include "lcd/bmp.h"			// ロゴマーク画像

uint16_t BACK_COLOR = BLACK;	// 背景色
uint8_t ankfont_width = 8;		// 1バイトフォントの表示幅


/******************************************************************************
      関数説明：   LCDCのSPIバスへデータを送信する
      パラメータ： dat = 送信データ
      戻り値：     なし
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{
#if SPI0_CFG == 1
	OLED_CS_Clr();

	while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE));
        spi_i2s_data_transmit(SPI0, dat);
	while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE));
        spi_i2s_data_receive(SPI0);

	OLED_CS_Set();
#elif SPI0_CFG == 2
	spi_dma_enable(SPI0, SPI_DMA_TRANSMIT);
#else
	u8 i;
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;
	}	
  OLED_CS_Set();	
#endif
}


/******************************************************************************
      関数説明：   LCDCに8bitデータを送信
      パラメータ： dat = 送信データ
      戻り値：     なし
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	OLED_DC_Set();	// データ
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      関数説明：   LCDCに16bitデータを送信
      パラメータ： dat = 送信データ
      戻り値：     なし
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
	OLED_DC_Set();	// データ
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      関数説明：   LCDCにコマンドを送信
      パラメータ： dat = 送信コマンド
      戻り値：     なし
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	OLED_DC_Clr();	// コマンド
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      関数説明：   描画の範囲を設定する
      パラメータ： x1,x2 = 左端,右端のx座標
                  y1,y2 = 上端,下端のy座標
      戻り値：     なし
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_WR_REG(0x2a);	// column address set
		LCD_WR_DATA(x1+26);
		LCD_WR_DATA(x2+26);
		LCD_WR_REG(0x2b);	// row address set
		LCD_WR_DATA(y1+1);
		LCD_WR_DATA(y2+1);
		LCD_WR_REG(0x2c);	// memory write
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_WR_REG(0x2a);	// column address set
		LCD_WR_DATA(x1+26);
		LCD_WR_DATA(x2+26);
		LCD_WR_REG(0x2b);	// row address set
		LCD_WR_DATA(y1+1);
		LCD_WR_DATA(y2+1);
		LCD_WR_REG(0x2c);	// memory write
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_WR_REG(0x2a);	// column address set
		LCD_WR_DATA(x1+1);
		LCD_WR_DATA(x2+1);
		LCD_WR_REG(0x2b);	// row address set
		LCD_WR_DATA(y1+26);
		LCD_WR_DATA(y2+26);
		LCD_WR_REG(0x2c);	// memory write
	}
	else
	{
		LCD_WR_REG(0x2a);	// column address set
		LCD_WR_DATA(x1+1);
		LCD_WR_DATA(x2+1);
		LCD_WR_REG(0x2b);	// row address set
		LCD_WR_DATA(y1+26);
		LCD_WR_DATA(y2+26);
		LCD_WR_REG(0x2c);	// memory write
	}
}

#if SPI0_CFG == 2
/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
	dma_parameter_struct dma_init_struct;

    /* SPI0 transmit dma config:DMA0,DMA_CH2 */
    dma_deinit(DMA0, DMA_CH2);
    dma_struct_para_init(&dma_init_struct);

    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI0);
    dma_init_struct.memory_addr  = (uint32_t)image;
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority     = DMA_PRIORITY_LOW;
    dma_init_struct.number       = FRAME_SIZE;
    dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_init(DMA0, DMA_CH2, &dma_init_struct);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH2);
    dma_memory_to_memory_disable(DMA0, DMA_CH2);
}
#endif

#if SPI0_CFG == 1
/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_config(void)
{
    spi_parameter_struct spi_init_struct;
    /* deinitilize SPI and the parameters */
    OLED_CS_Set();
    spi_struct_para_init(&spi_init_struct);

    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);

	spi_crc_polynomial_set(SPI0,7);
	spi_enable(SPI0);
}
#endif


/******************************************************************************
      関数説明：   LCD初期化(=Lcd_init)
      パラメータ： なし
      戻り値：     なし
******************************************************************************/
void LCD_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);

#if SPI0_CFG == 1
 	rcu_periph_clock_enable(RCU_AF);
	rcu_periph_clock_enable(RCU_SPI0);
	/* SPI0 GPIO config: NSS/PA4, SCK/PA5, MOSI/PA7 */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 |GPIO_PIN_6| GPIO_PIN_7);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

	spi_config();

#elif SPI0_CFG == 2
    rcu_periph_clock_enable(RCU_DMA0);
    rcu_periph_clock_enable(RCU_SPI0);

	/* SPI0 GPIO config: NSS/PA4, SCK/PA5, MOSI/PA7 */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7);
    /* SPI0 GPIO config: MISO/PA6 */
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

	dma_config();

	dma_channel_enable(DMA0,DMA_CH2);
#elif SPI0_CFG == 3
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_7);
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

	gpio_bit_reset(GPIOA, GPIO_PIN_5 | GPIO_PIN_7);
	gpio_bit_reset(GPIOB, GPIO_PIN_2);
#endif

	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1);
	gpio_bit_reset(GPIOB, GPIO_PIN_0 | GPIO_PIN_1);

	OLED_RST_Clr();
	delay_1ms(200);
	OLED_RST_Set();
	delay_1ms(20);
	OLED_BLK_Set();

	LCD_WR_REG(0x11);	// turn off sleep mode
	delay_1ms(100);

	LCD_WR_REG(0x21);	// display inversion mode

	LCD_WR_REG(0xB1);	// Set the frame frequency of the full colors normal mode
						// Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
						// fosc = 850kHz
	LCD_WR_DATA8(0x05);	// RTNA
	LCD_WR_DATA8(0x3A);	// FPA
	LCD_WR_DATA8(0x3A);	// BPA

	LCD_WR_REG(0xB2);	// Set the frame frequency of the Idle mode
						// Frame rate=fosc/((RTNB x 2 + 40) x (LINE + FPB + BPB +2))
						// fosc = 850kHz
	LCD_WR_DATA8(0x05);	// RTNB
	LCD_WR_DATA8(0x3A);	// FPB
	LCD_WR_DATA8(0x3A);	// BPB

	LCD_WR_REG(0xB3);	// Set the frame frequency of the Partial mode/ full colors
	LCD_WR_DATA8(0x05);  
	LCD_WR_DATA8(0x3A);
	LCD_WR_DATA8(0x3A);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3A);
	LCD_WR_DATA8(0x3A);

	LCD_WR_REG(0xB4);
	LCD_WR_DATA8(0x03);

	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x62);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x04);

	LCD_WR_REG(0xC1);
	LCD_WR_DATA8(0xC0);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x00);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0x6A);   

	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x8D); 
	LCD_WR_DATA8(0xEE); 

	LCD_WR_REG(0xC5);  /*VCOM*/
	LCD_WR_DATA8(0x0E);    

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x0E);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x0E);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x12);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x37);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0E);
	LCD_WR_DATA8(0x10);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x0E);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x36);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0E);
	LCD_WR_DATA8(0x10);

	LCD_WR_REG(0x3A);	// define the format of RGB picture data
	LCD_WR_DATA8(0x05);	// 16-bit/pixel

	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x08);
	else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC8);
	else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x78);
	else LCD_WR_DATA8(0xA8);

	LCD_WR_REG(0x29);	// Display On

	ankfont_width = 8;
	#if USE_FONTX2		// Initialize FONTX2
	fontx2_init();
	#ifdef FONTX2_USELED
	led_init();
	led_on( LED_G );
	#endif
	if ( fontx2_open( 0, LCD_KFONT ) ) {
		#ifdef FONTX2_USELED
		led_on( LED_R );
		#endif
	}
	#if USE_FONTX2 == 2
	if ( fontx2_open( 1, LCD_AFONT ) ) {
		#ifdef FONTX2_USELED
		led_on( LED_R );
		#endif
	}
	ankfont_width = font[1].fontwidth;
	#endif 
	#ifdef FONTX2_USELED
	led_off( LED_G );
	#endif
	#endif
}


/******************************************************************************
      関数説明：   画面を指定色で塗りつぶす
      パラメータ： Color = 描画色 
      戻り値：     なし
******************************************************************************/
void LCD_Clear(u16 Color)
{
	u16 i,j;  	
	LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	  {
			for (j=0;j<LCD_H;j++)
				{
					LCD_WR_DATA(Color);
				}
	  }
}


/******************************************************************************
      関数説明：   漢字を1文字表示する(oledfont.h 内定義のもの)
      パラメータ： x,y = 表示開始(左上)座標
	              index = 文字番号(oledfont.h 内のインデックス番号)
				  size = 文字サイズ(16px or 32px)
				  color = 表示色
      戻り値：     なし
******************************************************************************/
void LCD_ShowChinese(u16 x,u16 y,u8 index,u8 size,u16 color)	
{  
	u8 i,j;
	u8 *temp,size1;
	if(size==16){temp=Hzk16;}	// 文字サイズ選択
	if(size==32){temp=Hzk32;}
  LCD_Address_Set(x,y,x+size-1,y+size-1); // 描画範囲指定
  size1=size*size/8;	// フォントデータサイズ計算
	temp+=index*size1;	// 文字選択
	for(j=0;j<size1;j++)
	{
		for(i=0;i<8;i++)
		{
		 	if((*temp&(1<<i))!=0)	// データの下位ビットから読み取る
			{
				LCD_WR_DATA(color); // pset
			}
			else
			{
				LCD_WR_DATA(BACK_COLOR);	// preset
			}
		}
		temp++;
	 }
}


/******************************************************************************
      関数説明：   点を描画する
      パラメータ： x,y = 描画座標
				  color = 描画色
      戻り値：     なし
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y); // 描画範囲指定
	LCD_WR_DATA(color);
} 


/******************************************************************************
      関数説明：   大きな点を描画する(実際には3x3の矩形を描画する)
      パラメータ： x,y = 描画座標
				  color = 描画色
      戻り値：     なし
******************************************************************************/
void LCD_DrawPoint_big(u16 x,u16 y,u16 color)
{
	LCD_Fill(x-1,y-1,x+1,y+1,color);
} 


/******************************************************************************
      関数説明：   矩形領域を塗りつぶす
      パラメータ： xsta,ysta = 左上座標
				  xend,yend = 右下座標
				  color = 描画色
      戻り値：     なし
******************************************************************************/
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	LCD_Address_Set(xsta,ysta,xend,yend); // 描画範囲指定 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);
	} 					  	    
}


/******************************************************************************
      関数説明：   直線を描画する
      パラメータ： x1,y1 = 始点座標
				  x2,y2 = 終点座標
				  color = 描画色
      戻り値：     なし
******************************************************************************/
void LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1;	// 座標増分の計算
	delta_y=y2-y1;
	uRow=x1;		// 描画の開始点の座標
	uCol=y1;
	if(delta_x>0)incx=1; // 方向指定
	else if (delta_x==0)incx=0;	// 垂直線
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;	// 水平線
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x;	// 基本となる描画軸を指定
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);		// pset
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/******************************************************************************
      関数説明：   矩形を描画する
      パラメータ： x1,y1 = 始点座標
				  x2,y2 = 終点座標
				  color = 描画色
      戻り値：     なし
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      関数説明：   円を描画する(=Draw_Circle)
      パラメータ： x0,y0 = 中心座標
				  r = 半径
				  color = 描画色
      戻り値：     なし
******************************************************************************/
void LCD_DrawCircle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	// int di;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))	// 描画点が遠すぎるかどうか判断
		{
			b--;
		}
	}
}


/******************************************************************************
      関数説明：   1文字表示する
      パラメータ： x,y = 表示開始(左上)座標
                  num = 文字コード(ASCII および Shift-JIS)
                  mode = 描画モード(文字背景を背景色で塗りつぶすかどうか)
				    (0 → 塗りつぶす, 1 → 塗りつぶさない)
				  color = 描画色
      戻り値：	   表示エラーのとき → エラーコード 
	  			    0 → 未定義文字
	  			    254 → 右端がはみ出すため改行が必要
				    255 → 表示範囲外(改行しても表示できない場合)
	  			  正常終了時 → 表示された文字の横幅(スペーシングを含む)
******************************************************************************/
uint8_t LCD_ShowChar(u16 x,u16 y,u16 num,u8 mode,u16 color)
{
    u8 temp;
    u8 pos, t, tw, i;
	u16 x0=x;
	uint8_t wb = 1;				// 横方向のバイト数
	uint8_t fx2 = 0;			// FONTX2を使う場合、フォントNo.+1. 使わない場合 0.
	uint8_t sp = 0;				// スペーシング
	uint8_t font_width = 8;		// フォント表示幅
	uint8_t font_height = 16;	// フォント表示高さ

	#if USE_FONTX2
		#if USE_FONTX2 == 2
			// 1バイト文字もFONTX2を使用する場合
			font_height = font[1].fontheight;
			font_width = ankfont_width;
			fx2 = 2;
			sp = LCD_ASPACE;
		#endif
		if( num > 0xff ) {
			// 2バイト文字にFONTX2を使用する場合
			font_height = font[0].fontheight;
			font_width = font[0].fontwidth;
			fx2 = 1;
			sp = LCD_KSPACE;
		}
	#else
		if( num > 0xff ) {
			return 0;	// エラー(未定義文字)
		}
	#endif

	wb = ( font_width + 7 ) / 8;
	if( fx2 ) {
		// FONTX2のフォント読み込み
		#ifdef FONTX2_USELED
			led_on( LED_G );
		#endif
		if( ( i = fontx2_read( fontdata, fx2 - 1, num ) ) ) {
			#ifdef FONTX2_USELED
				led_off( LED_G );
				led_on( LED_R );
			#endif
			// LCD_ShowNum( 120, 60, i, 4, RED );	// エラーコード表示(デバッグ用)
			return 0;	// エラー(未定義文字)
		}
		#ifdef FONTX2_USELED
			led_off( LED_G );
		#endif
	}
	else {
		num = num - ' ';	// 標準フォントのindex
	}

    if( x > LCD_W - font_width ) {
		if( y > LCD_H - font_height ) {
			return SC_OVER;		// 表示領域外
		}
		else {
			return SC_NL;		// 改行が必要
		}
	}

	LCD_Address_Set( x, y, x + font_width - 1, y + font_height - 1 );	// 表示アドレス設定
	if( !mode ) {
		// 上書きモード
		for( pos = 0; pos < font_height; pos++ ) {
			for( i = 0; i < wb; i++ ) {
				tw = font_width - i * 8;
				tw = ( tw > 8 ) ? 8 : tw;
				if( fx2 ) {
					// FONTX2
					temp = fontdata[pos * wb + i];				// FONTX2のフォントデータ
					for( t = 0; t < tw; t++ ) {
				        if( temp & 0x80 ) { 
							LCD_WR_DATA( color ) ;
						}
						else {
							LCD_WR_DATA( BACK_COLOR );
						}
						temp <<= 1;
						x++;
			    	}
					x = x0;
					y++;
				}
				else {
					// 標準フォント
					temp = asc2_1608[(u16)num * 16 + pos];		// oledfont.h のフォントデータ
					for( t = 0; t < tw; t++ ) {
				        if( temp & 0x01 ) { 
							LCD_WR_DATA( color ) ;
						}
						else {
							LCD_WR_DATA( BACK_COLOR );
						}
						temp >>= 1;
						x++;
			    	}
					x = x0;
					y++;
				}
			}
		}	
	}
	else {
		// ORモード
		for( pos = 0; pos < font_height; pos++ ) {
			for( i = 0; i < wb; i++ ) {
				tw = font_width - i * 8;
				tw = ( tw > 8 ) ? 8 : tw;
				if( fx2 ) {
					// FONTX2
					temp = fontdata[pos * wb + i];				// FONTX2のフォントデータ
					for( t = 0; t < tw; t++ ) {
				        if( temp & 0x01 ) { 
							LCD_DrawPoint( x + t, y + pos, color );
						}
						temp <<= 1;
						x++;
			    	}
					x = x0;
					y++;
				}
				else {
					// 標準フォント
					temp = asc2_1608[(u16)num * 16 + pos];		// oledfont.h のフォントデータ
					for( t = 0; t < tw; t++ ) {
				        if( temp & 0x01 ) { 
							LCD_DrawPoint( x + t, y + pos, color );
						}
						temp >>= 1;
						x++;
			    	}
					x = x0;
					y++;
				}
			}
		}
	}
	return font_width + sp;
}


/******************************************************************************
      関数説明：   文字列を表示する(2バイト文字に対応するには USE_UTF8STR を定義)
      パラメータ： x,y = 表示開始(左上)座標
                  p = 文字列へのポインタ
				  color = 描画色
      戻り値：	   なし
******************************************************************************/
void LCD_ShowString(u16 x,u16 y,const u8 *p,u16 color)
{
	uint8_t s, c;
	uint8_t l = 0;
	uint16_t uc = 0;

    while( ( c = *p ) ) {
		#ifdef USE_UTF8STR
			// UTF8→UTF16 変換
			if( ( c & 0x80 ) == 0 ) {
				uc = c & 0x7f;	l = 0; 
			}
			else if( ( c & 0xe0 ) == 0xc0 ) {
				uc = c & 0x1f;	l = 1; 
			}
			else if( ( c & 0xf0 ) == 0xe0 ) {
				uc = c & 0x0f;	l = 2; 
			}
			else if( ( c & 0xf8 ) == 0xf0 ) {
				uc = c & 0x07;	l = 3; 
			}
			else if( ( c & 0xfc ) == 0xf8 ) {
				uc = c & 0x03;	l = 4; 
			}
			else if( ( c & 0xfe ) == 0xfc ) {
				uc = c & 0x01;	l = 5; 
			}
			while( l-- > 0  &&  ( ( ( c = *( ++p ) ) & 0xc0 ) == 0x80 ) ) {
				uc = ( uc << 6 ) | ( c & 0x3f ) ;
			}
			// UTF16→SJIS 変換
			uc = uni2sjis( uc );
		#else
			uc = c;
		#endif

		// 文字表示
		switch( s = LCD_ShowChar( x, y, uc, 0, color ) ) {
			case SC_UNDEF:
				break;
			case SC_NL:
				// 自動改行
				y += FONT_HEIGHT;
				x = 0;
				x += LCD_ShowChar( x, y, uc, 0, color );
				break;
			case SC_OVER:
				// 表示領域オーバー
				LCD_Clear( RED );
				x = 0;
				y = 0;
				break;
			default:
				x += s;
				break;
		}
        p++;
    }
}


/******************************************************************************
      関数説明：   べき乗の計算
      パラメータ： m = 底
                  n = 指数
      戻り値：     m^n
******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}


/******************************************************************************
      関数説明：   整数値表示
      パラメータ： x,y = 表示開始(左上)座標
                  num = 表示数値
                  len = 表示文字数
				  color = 描画色
      戻り値：     なし
******************************************************************************/
void LCD_ShowNum(u16 x,u16 y,u16 num,u8 len,u16 color)
{         	
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+ankfont_width*t,y,' ',0,color);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+ankfont_width*t,y,temp+48,0,color); 
	}
} 


/******************************************************************************
      関数説明：   16進数表示
      パラメータ： x,y = 表示開始(左上)座標
                  num = 表示数値
                  len = 表示文字数
				  color = 描画色
      戻り値：     なし
******************************************************************************/
void LCD_ShowHex(u16 x,u16 y,u16 num,u8 len,u16 color)
{
	uint8_t t, temp;
	uint8_t enshow = 0;
	for( t = 0; t < len; t++ ) {
		temp = ( num / mypow( 16, len - t - 1 )) % 16;
		if( enshow == 0 && t < ( len - 1 ) ) {
			if( !temp ) {
				LCD_ShowChar( x + ankfont_width * t, y, ' ', 0, color );
				continue;
			}
			else {
				enshow = 1; 
			}
		}
	 	LCD_ShowChar( x + ankfont_width * t, y, temp > 9 ? temp + 55 : temp + 48, 0, color ); 
	}
}


/******************************************************************************
      関数説明：   小数値表示
      パラメータ： x,y = 表示開始(左上)座標
                  num = 表示数値
                  len = 表示文字数
				  color = 描画色
      戻り値：     なし
******************************************************************************/
void LCD_ShowNum1(u16 x,u16 y,float num,u8 len,u16 color)
{         	
	u8 t,temp;
	// u8 enshow=0;
	u16 num1;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+ankfont_width*(len-2),y,'.',0,color);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+ankfont_width*t,y,temp+48,0,color);
	}
}


/******************************************************************************
      関数説明：   40x40画像表示(image[]に表示データをセットする)
      パラメータ： x1,y1 = 左上座標
				  x2,y2 = 右下座標
      戻り値：     なし
******************************************************************************/
void LCD_ShowPicture(u16 x1,u16 y1,u16 x2,u16 y2)
{
	int i;
	LCD_Address_Set(x1,y1,x2,y2);
	for(i=0;i<12800;i++)
	{ 	
		// LCD_WR_DATA8(image[i*2+1]);
		LCD_WR_DATA8(image[i]);
	}			
}


/******************************************************************************
      関数説明：   ロゴマーク表示(bmp.h 内で定義された160x76のイメージを表示)
      パラメータ： なし
      戻り値：     なし
******************************************************************************/
void LCD_ShowLogo(void)
{
	int i;
	LCD_Address_Set(0,0,159,75);
	for(i=0;i<25600;i++)
	{
		LCD_WR_DATA8(logo_bmp[i]);
	}			
}

