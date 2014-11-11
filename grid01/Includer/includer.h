#ifndef __grid_includer__
#define __grid_includer__

#define __data_ver_03__	//データ構造ver.01
	//01	:	ツリー構造がdynamic<Tree>を保持、切り分けがぐちゃぐちゃに
	//02	:	dynamic<Tree<T>>(ver.02)を所持、文字列も配列とポインタで処理
	//			(なんだかんだ言ってC言語の処理系が安定する)
	//03	:	連想マップを使ってみる。
#define __dynamic_ver_02__	//メモリ管理クラスver.02
	//01	:	mallocで管理、callはポインタを返却
	//02	:	reallocで管理、callはリファレンスを返却
#define __format_ver_01__	//処理系データフォーマットver.01
	//01	:	Treeとswitchでごり押し
#define __tree_data_format_ver_01__	//データ格納フォーマットver.01
	//01	:	Windowsのファイル属性を参考にjsonみたいな感じで
#define __cui_output_ver_02__	//cui出力制御クラスver.02
	//01	:	basic_streambufの継承
	//02	:	普通に自作クラス

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <memory>
#include <iostream>
#include <functional>	//ラムダ式
//↑function<int()>みたいな使い方
#include <locale.h>	//wchar変換用
#include "flag.h"	//自作ヘッダの最初で定義
#include "cui.h"	//"flag.h"の次に定義
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
