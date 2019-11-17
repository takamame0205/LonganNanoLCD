// FONTX2ライブラリ Ver 1.0
// 2019/11/17 by Kyoro

#ifndef __FONTX2_H
#define __FONTX2_H

#include <stdint.h>
#include "fatfs/tf_card.h"	// 利用にはFatFsライブラリが必要です

// 記号定数
#define FONTX2_ASCII 0
#define FONTX2_SJIS 1
#define FONTX2_FONTNUM 2	// 同時に利用できるフォントの数
#define FONTX2_FONTSIZE 72	// フォントデータの最大サイズ(24x24→72バイト)

// 構造体定義
typedef struct {
	uint8_t singature[6];	// シグネチャ
	uint8_t fontname[8];	// フォント名
	uint8_t fontwidth;		// 文字の幅
	uint8_t fontheight;		// 文字の高さ
	uint8_t code;			// 文字コード種別
	uint8_t cbnum;			// コードブロック数
	uint8_t size;			// 1文字当たりのデータサイズ
	uint8_t cb[512];		// コードブロック格納バッファ
} FH;

// 外部参照
extern FH font[FONTX2_FONTNUM];				// FONTX2ヘッダ
extern uint8_t fontdata[FONTX2_FONTSIZE];	// フォントパターン格納バッファ
extern void fontx2_init( void );													// フォントファイルを開いて初期化する
extern uint8_t fontx2_open(	uint8_t fontnum, char* filename	);						// フォントファイルを開き、ヘッダを読み込む
extern uint8_t fontx2_read( uint8_t* buffer, uint8_t fontnum, uint16_t charcode );	// フォントデータを取得する

#endif