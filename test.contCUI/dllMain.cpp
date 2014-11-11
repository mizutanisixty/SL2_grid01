#include <Windows.h>
#include "../grid01/includer.h"

BOOL WINAPI DllMain( HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved){
	return TRUE;
}

int __stdcall echo(CUI *_cui, void *t1, void *t2, void *t3, void *_ans){
	vtor(CUI, cui);
	vtor(string, ans);
	string s;
	cui << "input\t> " >> s;
	ans = s;
	return RETURN_SUCCESS;
}

int __stdcall setCharSize(CUI *_cui, void *___id, void *t1, void *t2, void *_ans){
	vtor(string, ans);
	ans = "font size changed.\n";
	//void_to_ref(int, _id, id);
	int id;
	if(___id){
	vtor(string, __id);
	int _id = Math<int>().atoi(__id);
	//void_to_ref(CUI, _cui, cui);
	vtor(CUI, cui);
	id = _id;
	}else{
		id = 3;
	}
	cout << "set id to " << id << (id==3?" (default)":"") << endl;
	debug_message_called("setCharSize");
	SHORT size[][2] ={
		{3, 5},
		{5, 8},
		{6, 13},
		{8, 18},
		{10, 22},
		{12, 27}
	};
	if(id<0)
		id=0;
	if(id>_countof(size))
		id = _countof(size);
	const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX info = {sizeof info};
	info.dwFontSize.X = size[id][0];
	info.dwFontSize.Y = size[id][1];
	SetCurrentConsoleFontEx(hConsole, 0, &info);
	return RETURN_SUCCESS;
}

//この関数は、無かったことに！！
//(そんな名前の健康食品か何かあったなあ)
int __stdcall grepStr(char *str, char *find, WORD wAttributes = grep_strong_1, int dump=0){
	grep(str, find, wAttributes);
	return RETURN_SUCCESS;
}
