#ifndef __grid_signal_format__
#define __grid_signal_format__
#include <string>

//関数の場所を格納する構造体
typedef struct{
	WCHAR p[256];	//path
	char f[256];	//function
} RT_FUNC;

typedef struct{
	WCHAR *p;	//path
	char *f;	//function
} RT_FUNC_P;

typedef struct{
	bool a;
	int status;
	int count;
	int width;
	int height;
	int *table;
	char *comment;
} TABLE0;

// グラフィック用ウィンドウのデータをまとめた構造体
typedef struct{
	int					x;			// 表示開始位置
	int					y;
	COLORREF			*data;		// BMPのデータ部分
} IMG0;

#endif
