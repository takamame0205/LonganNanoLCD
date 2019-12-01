// FONTX2ライブラリ Ver 1.0beta3
// 2019/12/1 by Kyoro

#include "lcd/fontx2.h"

// グローバル変数
FH font[FONTX2_FONTNUM];			// FONTX2フォントヘッダ
uint8_t cb[FONTX2_SJISFNT][1024];	// コードブロックテーブル
uint8_t fontdata[FONTX2_FONTSIZE];	// フォントパターン格納バッファ
FATFS fs;							// ファイルシステムオブジェクト
FIL fontfile[FONTX2_FONTNUM];		// ファイルオブジェクト
uint8_t sjisnum = 0;				// 開いている全角フォントの数

// プロトタイプ
uint32_t get_font( uint8_t fontnum, uint16_t charcode );

// ライブラリ関数
void fontx2_init(	// FONTX2ライブラリを初期化する
	void
)					//  戻り値：なし
{
    f_mount( &fs, "", 1 );
}

uint8_t fontx2_open(	// フォントファイルを開き、ヘッダを読み込む
	uint8_t fontnum,	//  フォントNo.
	char* filename		//  フォントファイル名へのポインタ
)						//  戻り値：正常終了で0, エラーのときエラーコード
{
	FRESULT fr;
	UINT b;

	// パラメータチェック
	if( fontnum >= FONTX2_FONTNUM ) {
		return 4;	// フォントNo.不正
	}

	// フォントファイルを開く
    fr = f_open( &fontfile[fontnum], filename, FA_READ );	// フォントファイルを開く
	if( fr ) {
		return 1;	// ファイルオープンエラー
	}
    f_lseek( &fontfile[fontnum], 0 );	

	// ヘッダを読み込む
	fr = f_read( &fontfile[fontnum], &font[fontnum], 18, &b );
	if( fr || b < 18 ) {
		fontx2_close( fontnum );
		return 2;	// フォントヘッダ読み込みエラー
	}
	else {
		if( font[fontnum].code != FONTX2_ASCII ) {
			// コードブロックを読み込む
			if( sjisnum >= FONTX2_SJISFNT ) {
				fontx2_close( fontnum );
				return 3;	// コードブロック読み込みエラー
			}
			font[fontnum].cb = cb[sjisnum++];
			fr = f_read( &fontfile[fontnum], font[fontnum].cb, font[fontnum].cbnum * 4, &b );
			if( fr ) {
				fontx2_close( fontnum );
				return 3;	// コードブロック読み込みエラー
			}
		}
    	font[fontnum].size = ( font[fontnum].fontwidth + 7 ) / 8 * font[fontnum].fontheight;
		if( font[fontnum].size > FONTX2_FONTSIZE ) {
			fontx2_close( fontnum );
			return 6;	// 未対応のフォント(サイズが大きすぎる)
		} 
		return 0;
	}
}

void fontx2_close(		// フォントファイルを閉じる
	uint8_t fontnum		//  フォントNo.
)						//  戻り値：なし
{
	if( font[fontnum].code == FONTX2_SJIS ) {
		sjisnum--;
	}
	f_close( &fontfile[fontnum] );
}

uint8_t fontx2_read(	// 1文字分のフォントの読み込み
	uint8_t* buffer,	//  データを読み込みバッファへのポインタ
	uint8_t fontnum,	//  フォントNo.
	uint16_t charcode	//  文字コード(ASCII/SJIS)
)						//  戻り値：正常終了で0, エラーのときエラーコード
{
	FRESULT fr;
	UINT b;
	uint32_t p;

	// パラメータチェック
	if( fontnum >= FONTX2_FONTNUM ) {
		return 4;	// フォントNo.不正(4)
	}

	// フォントデータへのポインタを得る
	p = get_font( fontnum, charcode );
	if( !p ) {
		return 5;	// 未定義の文字コード(5)
	}

	// ファイルから読み出す
	if( !( fr = f_lseek( &fontfile[fontnum], p ) ) ) {
		fr = f_read( &fontfile[fontnum], buffer, font[fontnum].size, &b );
		if( fr ) {
			return 64 + fr;		// フォントデータリードエラー(65～83)
		}
		else if( b < font[fontnum].size ) {
			return 128 + b;		// フォントデータリードエラー(128～255)
		}
		else {
			return 0;	// 正常終了
		}
	}
	else {
		return 32 + fr;	// フォントデータシークエラー(33～51)
	}
}

uint32_t get_font(		// FONTX2のフォントデータを取得する
	uint8_t fontnum,	//  フォントNo.
    uint16_t charcode	//	文字コード(Shift JIS)
)						//  戻り値：データへの相対ポインタ, エラーのとき0) 
{
	// 以下はChaN様作のFONTX2用関数を改変して利用させていただきました
    uint16_t nc, bc, sb, eb;
	uint8_t *cblk;

    if ( font[fontnum].code == 0 ) {	/* Single byte code font */
        if ( charcode < 0x100 ) {
			 return ( 17 + charcode * font[fontnum].size );
		}
    }
	else {				/* Double byte code font */
        cblk = font[fontnum].cb;
		nc = 0;  			/* Code block table */
        bc = font[fontnum].cbnum;
        while ( bc-- ) {
            sb = cblk[0] + cblk[1] * 0x100;  /* Get range of the code block */
            eb = cblk[2] + cblk[3] * 0x100;
            if ( charcode >= sb && charcode <= eb ) {  /* Check if in the code block */
                nc += charcode - sb;             /* Number of codes from top of the block */
                return ( 18 + 4 * font[fontnum].cbnum + nc * font[fontnum].size );
            }
            nc += eb - sb + 1;     /* Number of codes in the previous blocks */
            cblk += 4;             /* Next code block */
        }
    }

    return 0;   /* Invalid code */
}