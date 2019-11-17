// unicode.c 用ヘッダファイル
//

#ifndef __UNICODE_H
#define __UNICODE_H

#include <stdint.h>
#include "lcd/lcd.h"

#ifdef USE_UTF8STR

typedef unsigned int	UINT;	/* int must be 16-bit or 32-bit */
typedef unsigned char	BYTE;	/* char must be 8-bit */
typedef uint16_t		WORD;	/* 16-bit unsigned integer */
typedef uint16_t		WCHAR;	/* 16-bit unsigned integer */
typedef uint32_t		DWORD;	/* 32-bit unsigned integer */
typedef uint64_t		QWORD;	/* 64-bit unsigned integer */

extern WCHAR uni2sjis( DWORD uni );	/* UTF-16 encoded character to be converted */

#endif
#endif