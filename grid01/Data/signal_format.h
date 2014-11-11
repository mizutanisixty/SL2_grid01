#ifndef __grid_signal_format__
#define __grid_signal_format__
#include <string>

//�֐��̏ꏊ���i�[����\����
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

// �O���t�B�b�N�p�E�B���h�E�̃f�[�^���܂Ƃ߂��\����
typedef struct{
	int					x;			// �\���J�n�ʒu
	int					y;
	COLORREF			*data;		// BMP�̃f�[�^����
} IMG0;

#endif
