#include <Windows.h>
#include <iostream>
#include "../grid01/includer.h"
#include "../grid01/gui.h"

#ifndef LR_LOADREALSIZE
#define LR_LOADREALSIZE 128
#endif

BOOL WINAPI DllMain( HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved){
	return TRUE;
}

using namespace std;

#define show(x) # x << " = " << (x)

int __stdcall getMap(CUI *_cui, void *t1, void *t2, void *t3, void *_i){
	vtor(IMG0, i);
	vtor(CUI, cui);
	debug_message_called("test.procImg01 > getMap");

	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, L"../test.provMapData.bmp", IMAGE_BITMAP, 0, 0, LR_LOADREALSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	BITMAP bm;
	if(!hBitmap){
		cui << "load error!\n";
		return RETURN_FAILED;
	}
	GetObject(hBitmap, sizeof(BITMAP), &bm);
#ifdef __debug_calling__
	cui << "width = " << (i.x = bm.bmWidth) << ", height = " << (i.y = bm.bmHeight) << endl;
#endif
	i.data = ma(COLORREF, i.x * i.y);
	HDC hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, hBitmap);
	for(int x=0; x<i.x; x++)
		for(int y=0; y<i.y; y++)
			i.data[y*i.x+x] = GetPixel(hdc, x, y);
	DeleteObject(hdc);

	return RETURN_SUCCESS;
}

int __stdcall getImgByPath(CUI *_cui, void *_path, void *t1, void *t2, void *_ans){
	vtor(CUI, cui);
	vtor(string, path);
	vtor(IMG0, ans);
	debug_message_called("test.procImg01 > procImgByPath");

	TCHAR pathW[257];
	mbstowcs(pathW, path.c_str(), 256);
	

	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, pathW, IMAGE_BITMAP, 0, 0, LR_LOADREALSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	BITMAP bm;
	if(!hBitmap){
		cui << "load error!\n";
		return RETURN_FAILED;
	}
	GetObject(hBitmap, sizeof(BITMAP), &bm);
#ifdef __debug_calling__
	cui << "width = " << (ans.x = bm.bmWidth) << ", height = " << (ans.y = bm.bmHeight) << endl;
#endif
	ans.data = ma(COLORREF, ans.x * ans.y);
	HDC hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, hBitmap);
	for(int x=0; x<ans.x; x++)
		for(int y=0; y<ans.y; y++)
			ans.data[y*ans.x+x] = GetPixel(hdc, x, y);
	DeleteObject(hdc);

	return RETURN_SUCCESS;
}

int __stdcall procMap(CUI *_cui, void *_map, void *_i, void *t1, void *_ans){
	int _rad = 7;
	COLORREF pColor = RGB(255, 0, 0);
	vtor(CUI, cui);
	vtor(IMG0, map);
	vtor(TABLE0, i);
	vtor(IMG0, ans);
	ans.x = map.x;
	ans.y = map.y;
	ans.data = ma(COLORREF, map.x*map.y);
	memcpy(ans.data, map.data, sizeof(COLORREF)*map.x*map.y);
	if(!i.a)
		return RETURN_FAILED;
	int rad = _rad;
	int sqRad = advInt<int>(rad)^2;
	auto radAbs = [=](int i){return (i<rad ? rad-i : i-rad/*+1*/);};	//‰~‚ÌØ‚è‘Ö‚¦‚µiHjˆ—
	TABLE0 cirTable;
	//‰~‚ÌŒvŽZ
	{
		cirTable.a = true;
		int side = rad*2+1;
		cirTable.height = side;
		cirTable.width = side;
		cirTable.table = ma(int, advInt<int>(side)^2);
		int cnt = 0;
		for(int x=0; x<cirTable.width; x++)
			for(int y=0; y<cirTable.height; y++){
				advInt<int> px = radAbs(x), py = radAbs(y);
				cirTable.table[cnt++] = ((px^2)+(py^2)<=sqRad ? 1 : 0);
			}
	}
	int _nul_i = NULL;
	COLORREF _nul_c = NULL;
	auto cirPos = [&](int x, int y) -> int&{
		if(0<=x && x<cirTable.width && 0<=y && y<cirTable.height)
			return cirTable.table[y*cirTable.width+x];
		return _nul_i;
	};
	auto cirPosO = [&](int x, int y) -> int&{
		return cirPos(x+rad, y+rad);
	};
	auto ansPos = [&](int x, int y) -> COLORREF&{
		if(0<=x && x<ans.x && 0<=y && y<ans.y)
			return ans.data[y*ans.x + x];
		return _nul_c;
	};
	auto tData = [&](TABLE0 t, int x, int y) -> int&{
		return t.table[y*t.width+x];
	};
	for(int cnt=0; cnt<i.height; cnt++){
		int x = tData(i, 0, cnt), y = tData(i, 1, cnt);
		for(int cx=-rad; cx<=rad; cx++)
			for(int cy=-rad; cy<=rad; cy++)
				if(cirPosO(cx, cy))
					ansPos(x+cx, y+cy) = pColor;
	}
	free(cirTable.table);	//–Y‚ê‚¸‰ð•úB–Y‚ê‚½‚Æ‚±‚ë‚Å‚Ç‚¤‚Æ‚¢‚¤‚±‚Æ‚È‚¢‚ªB
	return RETURN_SUCCESS;
}
