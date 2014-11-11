#ifndef __grid_process_format__
#define __grid_process_format__
#include "includer.h"
using namespace std;

#define grep_strong_1 (FOREGROUND_RED | FOREGROUND_INTENSITY)	//赤い文字
#define grep_strong_2 (FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY)	//青背景、白い文字
char *typeWord = "?";	//タイプ名とdll名を分ける文字
char *sepWord = "*";	//dll名と関数名を分ける文字
char *cmdWordList[] = {
	"+",	//連結
	">",	//引数
	"{",	//引数ブロック
	"}",
	"(",	//変数リスト番号
	")"
};	//コマンドに使われる記号一覧(文字列)
int cwlCnt = _countof(cmdWordList);	//cmdWordListの要素数

struct STRLIST{
	size_t num;
	bool a;	//isAvailable
	bool c_first;	//is control first
	string *s;	//main char
	string *c;	//control char
	/*STRLIST(){
		a = false;
		num = 0;
	};
	~STRLIST(){
		if(a)
			delete[] s;
	};*/
};

struct STRPAIR{
	string type;
	string file;
	string func;
};

void grep(string str, string find, WORD wAttributes = grep_strong_1){
	if(str.find(find) == string::npos || str=="" || find==""){
		cui << str;
		return ;
	}
	//FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	HANDLE hStdout;
	CONSOLE_SCREEN_BUFFER_INFO csbi;	//構造体
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	//hStdoutのコンソールスクリーンバッファ情報をcsbiに取得
	GetConsoleScreenBufferInfo(hStdout, &csbi);

	string tmp_b, tmp_a;
	tmp_a = tmp_b = str;
	tmp_a.replace(0, tmp_a.find(find)+find.length(),"");
	tmp_b.replace(tmp_b.find(find),tmp_b.length(), "");
	cui << tmp_b;
	SetConsoleTextAttribute(hStdout, wAttributes);
	cui << find;
	SetConsoleTextAttribute(hStdout, csbi.wAttributes);
	//cui << tmp_a;
	grep(tmp_a,find, wAttributes);	//再起定義
}

void commandReplace(string &str, string from, string to){
	auto cntS = [=](string tmp){
		size_t cntS = 0;
		size_t cntE = 0;
		size_t p;
		while((p=tmp.find("[")) != string::npos){
			cntS++;
			tmp[p] = 'a';
		}
		while((p=tmp.find("]")) != string::npos){
			cntE++;
			tmp[p] = 'a';
		}
		return cntS - cntE;
	};
#ifdef __debug_searching__
	if(from != "[" && from != "]"){
		cui << debug_message_line << "\n";
		grep(str, from);
	cui << "\n\t\t\t↓\n";
	}
#endif
	if(str.find(from) != string::npos)
		str.replace(str.find(from),from.length(),to);
#ifdef __debug_searching__
	if(from != "[" && from != "]"){
		grep(str, to);
		cui << "\n" << debug_message_line << "\n";
	}
#endif
}

STRLIST unProcessed(string str){
	STRLIST a;
	size_t seek;
	a.num = 0;
	a.a = false;
	string tmp = str;
	long cntA=0,	//* (sepWord)
		cntO=0;		//other
	while((seek=tmp.find(sepWord)) != string::npos){
		tmp[seek] = 'a';	//ダミーの文字
		cntA++;
	}
	for(int i=0;i<cwlCnt;i++){
		while((seek=tmp.find(cmdWordList[i])) != string::npos){
			tmp[seek] = 'a';	//ダミーの文字
			cntO++;
		}
	}
	a.num = cntO - cntA + 1;
	if(!a.num)	//問題なし
		return a;
	//未処理部分が存在
	a.a = true;
	a.s = new string[a.num];
	size_t proc_ing = 0;
	char *tmp2 = (char*)malloc(sizeof(char)*(str.length()+1)), *tmp2_seek;
	tmp2_seek = tmp2;
	strcpy(tmp2, str.c_str());
	string for_search = tmp2;
	while(proc_ing < a.num){
		size_t oPosMin = for_search.find(cmdWordList[0]), tmp3;
		if((tmp3=for_search.find(cmdWordList[0])) != string::npos)
			oPosMin = tmp3;
		else
			oPosMin = for_search.length();
		for(int i=1;i<cwlCnt;i++)	//不等号の向きが間違っていた記念
			if((tmp3=for_search.find(cmdWordList[i])) != string::npos)
				if(tmp3 < oPosMin)
					oPosMin = tmp3;
		tmp2_seek[oPosMin] = '\0';
#ifdef __debug__
		cui << "\ttmp2_seek = " << tmp2_seek << "\n";
#endif
		//リストに追加
		if(for_search.find(sepWord) > oPosMin)
			a.s[proc_ing++] = tmp2_seek;
		//シーク
		tmp2_seek += oPosMin + 1;		//TODO:ここを実装
		for_search = tmp2_seek;	//書くの忘れてた…
	}
	free(tmp2);
	return a;
}

STRLIST processed(string str){
	STRLIST a;
	size_t seek;
	a.num = 0;
	a.a = false;

	string tmp = str;
	long cntA=0,	//* (sepWord)
		cntO=0;		//other
	while((seek=tmp.find(sepWord)) != string::npos){
		tmp[seek] = 'a';
		cntA++;
	}
	for(int i=0;i<cwlCnt;i++){
		//TODO : 切り分け処理をデバッグ
		while((seek=tmp.find(cmdWordList[i])) != string::npos){
			tmp[seek] = 'a';
			cntO++;
		}
	}
	a.num = cntA;
	if(!a.num)	//呼び出せるところがない
		return a;
	//既処理部分が存在
	a.a = true;
	a.s = new string[a.num];
	size_t proc_ing = 0;
	char *tmp2 = (char*)malloc(sizeof(char)*(str.length()+1)), *tmp2_seek;
	tmp2_seek = tmp2;
	strcpy(tmp2, str.c_str());
	string for_search = tmp2;
	while(proc_ing < a.num){
		size_t oPosMin = for_search.find(cmdWordList[0]), tmp3;
		if((tmp3=for_search.find(cmdWordList[0])) != string::npos)
			oPosMin = tmp3;
		else
			oPosMin = for_search.length();
		for(int i=1;i<cwlCnt;i++)
			if((tmp3=for_search.find(cmdWordList[i])) != string::npos)
				if(tmp3 < oPosMin)
					oPosMin = tmp3;
		tmp2_seek[oPosMin] = '\0';
#ifdef __debug__
		cui << "\ttmp2_seek = " << tmp2_seek << "\n";
#endif
		//リストに追加
		if(for_search.find(sepWord) < oPosMin)
			a.s[proc_ing++] = tmp2_seek;
		//シーク
		tmp2_seek += oPosMin + 1;
		for_search = tmp2_seek;
	}
	free(tmp2);
	return a;
}

STRLIST cutCommand(string str){
	STRLIST a;
	a.num = 0;
	a.a = false;
	typedef struct{bool a; size_t pos;} pos_t;

	/*if(unProcessed(str).a)	//未処理部分があれば空で返す
		return a;
		*/
	string tmp = str;
	
	auto _sep = [&]{
		pos_t min = {0, false};
		size_t pos;
		for(int i=0; i<cwlCnt; i++)
			if((pos=tmp.find(cmdWordList[i])) != string::npos)
				if(!(min.a) || min.pos>pos){
					min.pos = pos;
					min.a = true;
				}
		return min;
	};
	
	auto _cnt = [&]{
		string t = str;
		size_t cnt = 0, pos;
		for(int i=0; i<cwlCnt; i++)
			if((pos=tmp.find(cmdWordList[i])) != string::npos){
				t[pos] = 'a';
				cnt++;
			}
		return cnt;
	};

	//単一コマンドの場合
	if(!_sep().a){
		a.s = new string;
		a.a = true;
		a.c_first = false;
		a.s[0] = str;
		return a;
	}

	pos_t seek;
	size_t proc_ing=0;
	size_t *posList = ma(size_t, a.num);
	char *tmp_c = ma(char, tmp.length()+1);
	tmp_c[tmp.length()] = '\0';
	strcpy(tmp_c, tmp.c_str());

	a.a = true;
	a.c_first = (_sep().pos == 0);
	a.num = _cnt();
	a.s = new string[a.num+1];
	a.c = new string[a.num+1];

	//位置情報を配列に格納
	while((seek=_sep()).a){
		posList[proc_ing] = seek.pos;
		tmp[seek.pos] = 'a';
		proc_ing++;
	}
	//cmdWordの格納
	for(size_t i=0; i<a.num; i++){
		a.c[i] = tmp_c[posList[i]];
		tmp_c[posList[i]] = '\0';
	}
	//制御文の格納
	a.s[0] = tmp_c;
	for(size_t i=0; i<a.num; i++){
		a.s[i+1] = tmp_c + posList[i] + 1;
	}
	a.s[a.num] = tmp_c + posList[a.num-1]+1;
	return a;
}

STRPAIR sepComPath(string str){
	STRPAIR a;
	size_t pos;
	char tmp[256];
	commandReplace(str, "[", "");
	commandReplace(str, "]", "");
	strcpy(tmp, str.c_str());
	if((pos=str.find(typeWord))!=string::npos){
		tmp[pos] = '\0';
		a.type = tmp;
		str = tmp + pos + 1;
		strcpy(tmp, str.c_str());
	}
	if((pos=str.find(sepWord))!=string::npos){
		tmp[pos] = '\0';
		a.func = tmp + pos + 1;
	}
	a.file = tmp;
	return a;
}

void checkMark(char buf[4][256], string& command){
	bool isA = false;
	char *command_initial = buf[0]+1;
	string tmpCmd = command_initial, tmp;
	for(int i=0;i<cwlCnt;i++)
		if(tmpCmd.find(cmdWordList[i]) != string::npos)
			isA = true;
	if(isA){
		STRLIST tmpCmdPart = unProcessed(command_initial);
		if(tmpCmdPart.a)
			for(size_t i=0;i<tmpCmdPart.num;i++){
				size_t tmp2, tmp2Min, tmp2Min2;
				string tmp2Min_str, tmp2Min2_str;
				isA = false;
				for(int n=1;n<cwlCnt;n){
					tmp = cmdWordList[n];
					tmp += tmpCmdPart.s[i];
					tmp2 = tmpCmd.find(tmp);
					if(!isA || tmp2Min > tmp2){
						isA = true;
						tmp2Min_str = cmdWordList[n];
						tmp2Min = tmp2;
					}
				}
					isA = false;
				for(int n=1;n<cwlCnt;n){
					tmp = tmpCmdPart.s[i];
					tmp += cmdWordList[n];
					tmp2 = tmpCmd.find(tmp);
					if(!isA || tmp2Min > tmp2){
						isA = true;
						tmp2Min2_str = cmdWordList[n];
						tmp2Min2 = tmp2;
					}
				}
				if(tmp2Min == tmp2Min2 + 1){
					string tmp = tmp2Min_str + tmpCmdPart.s[i] + tmp2Min2_str;
					string tmp2 = tmp2Min_str + '[' + tmpCmdPart.s[i] + ']' + tmp2Min2_str;
					tmpCmd.replace(tmpCmd.find(tmp),tmp.length(),tmp2);	//置換
				}
			}
	}else if(tmpCmd.find(sepWord) != string::npos){
		command = buf[0];
		strcpy(buf[0], command.c_str());
	}else{
		buf[0][0] = '[';
		size_t i=strlen(buf[0]);
		buf[0][i++]=']';
		buf[0][i] = '\0';
	}
}

#endif
