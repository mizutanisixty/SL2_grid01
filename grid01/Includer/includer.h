#ifndef __grid_includer__
#define __grid_includer__

#define __data_ver_03__	//�f�[�^�\��ver.01
	//01	:	�c���[�\����dynamic<Tree>��ێ��A�؂蕪���������Ⴎ�����
	//02	:	dynamic<Tree<T>>(ver.02)�������A��������z��ƃ|�C���^�ŏ���
	//			(�Ȃ񂾂��񂾌�����C����̏����n�����肷��)
	//03	:	�A�z�}�b�v���g���Ă݂�B
#define __dynamic_ver_02__	//�������Ǘ��N���Xver.02
	//01	:	malloc�ŊǗ��Acall�̓|�C���^��ԋp
	//02	:	realloc�ŊǗ��Acall�̓��t�@�����X��ԋp
#define __format_ver_01__	//�����n�f�[�^�t�H�[�}�b�gver.01
	//01	:	Tree��switch�ł��艟��
#define __tree_data_format_ver_01__	//�f�[�^�i�[�t�H�[�}�b�gver.01
	//01	:	Windows�̃t�@�C���������Q�l��json�݂����Ȋ�����
#define __cui_output_ver_02__	//cui�o�͐���N���Xver.02
	//01	:	basic_streambuf�̌p��
	//02	:	���ʂɎ���N���X

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <memory>
#include <iostream>
#include <functional>	//�����_��
//��function<int()>�݂����Ȏg����
#include <locale.h>	//wchar�ϊ��p
#include "flag.h"	//����w�b�_�̍ŏ��Œ�`
#include "cui.h"	//"flag.h"�̎��ɒ�`
#include "mngError.h"
#include "dynamic.h"
#include "includer_algorithm.h"
#include "includer_advancedData.h"
#include "debug_message.h"
#include "signal_format.h"
#include "processFormat.h"
#include "block.h"

#ifdef show_value
#undef show_value
#endif
#ifdef __debug__
#define clear_and_kbhit(time) while(_kbhit())getch();_sleep((time));while(!_kbhit())
#define show_value(x) cui<< # x ,cui<< " = " << (x) << endl
#else
#define clear_and_kbhit(this_is_disabled) NULL
#define show_value(x) NULL
#endif

#define errorMessage(file, func) cui << "fatal exception occured at \"" << (func) << " in " << (file) << "\"\n"


#endif
