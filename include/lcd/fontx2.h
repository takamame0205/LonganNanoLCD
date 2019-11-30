// FONTX2ライブラリ Ver 1.0beta2
// 2019/11/30 by Kyoro

#ifndef __FONTX2_H
#define __FONTX2_H

#include <stdint.h>
#include "fatfs/tf_card.h"	// 利用にはFatFsライブラリが必要です

// 記号定数
#define FONTX2_ASCII	0
#define FONTX2_SJIS		1
#define FONTX2_FONTNUM	2	// 同時に利用できるフォントの数(1つにつき23バイトのワークエリアが必要)
#define FONTX2_SJISFNT	1	// 同時に利用できる全角フォントの数(1つにつき追加で1024バイトのワークエリアが必要)
#define FONTX2_FONTSIZE	72	// フォントデータの最大サイズ(24x24→72バイト)

// エラーコード
#define FONTX2_OK			0		// 正常終了(0)
#define FONTX2_ERR_OPEN		1		// フォントファイルオープンエラー(1)
#define FONTX2_ERR_RHD		2		// フォントヘッダリードエラー(2)
#define FONTX2_ERR_RCB		3		// コードブロックリードエラー(3)
#define FONTX2_ERR_FNO		4		// フォント番号不正(4)
#define FONTX2_ERR_UNDEF	5		// 未定義の文字コード(5)
#define FONTX2_ERR_FSIZE	6		// 未対応フォント(1文字当たりのデータサイズがFONTX2_FONTSIZEより大きい)(6)
#define FONTX2_ERR_FSEEK	32		// フォントデータシークエラー(32 + f_lseek 関数の終了コード)
#define FONTX2_ERR_FREAD	64		// フォントデータリードエラー(64 + f_read 関数の終了コード)
#define FONTX2_ERR_TRUNC	128		// フォントデータリードエラー(128 + 読み込んだバイト数)

// 構造体定義
typedef struct {
	uint8_t singature[6];	// シグネチャ
	uint8_t fontname[8];	// フォント名
	uint8_t fontwidth;		// 文字の幅
	uint8_t fontheight;		// 文字の高さ
	uint8_t code;			// 文字コード種別
	uint8_t cbnum;			// コードブロック数
	uint8_t size;			// 1文字当たりのデータサイズ
	uint8_t *cb;			// コードブロックテーブルへのポインタ
} FH;

// 外部参照
extern FH font[FONTX2_FONTNUM];				// FONTX2ヘッダ
extern uint8_t fontdata[FONTX2_FONTSIZE];	// フォントパターン格納バッファ
extern void fontx2_init( void );													// FONTX2ライブラリを初期化する
extern uint8_t fontx2_open(	uint8_t fontnum, char* filename	);						// フォントファイルを開き、ヘッダを読み込む
extern void fontx2_close( uint8_t fontnum );										// フォントファイルを閉じる
extern uint8_t fontx2_read( uint8_t* buffer, uint8_t fontnum, uint16_t charcode );	// フォントデータを取得する

#endif