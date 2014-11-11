#ifndef __grid_gui__
#define __grid_gui__
/*
#include <windows.h>
#include <Vfw.h>
#include <dshow.h>
#include <cstdlib>
*/
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h>
#include <wingdi.h>
#include "gloval.h"

LRESULT CALLBACK eMainWindowProc(
      HWND   hwnd   // handle to window
    , UINT   uMsg   // message identifier
    , WPARAM wParam // first message parameter
    , LPARAM lParam // second message parameter
);

BOOL reDraw(HWND hWnd, BOOL bErase = FALSE){
	return InvalidateRect(hWnd , NULL, bErase);
}

int GUIinit(WNDCLASSEX &tWndClass,HINSTANCE hInstance, TCHAR* cpClassName, TCHAR* cpMenu){
	setlocale( LC_ALL, "Japanese" );
	// ウインドウクラスパラメータセット
	tWndClass.cbSize        = sizeof( WNDCLASSEX );
	tWndClass.style         = CS_HREDRAW | CS_VREDRAW;
	tWndClass.lpfnWndProc   = eMainWindowProc;
	tWndClass.cbClsExtra    = 0;    // GetClassLong で取得可能なメモリ
	tWndClass.cbWndExtra    = 0;    // GetWindowLong で取得可能なメモリ
	tWndClass.hInstance     = hInstance;
	tWndClass.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
	tWndClass.hCursor       = LoadCursor( NULL, IDC_CROSS );	//標準：IDC_ARROW
	tWndClass.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
	tWndClass.lpszMenuName  = cpMenu;
	tWndClass.lpszClassName = cpClassName;
	tWndClass.hIconSm       = NULL;

	static bool called = false;
	if(!called){
		//初回のみクラス登録
		if(!RegisterClassEx( &tWndClass)){
			string e = "creating window class failed";
			grep(e + "\n", e, grep_strong_2);
			return RETURN_FAILED;
		}
	}
	called = true;
	return RETURN_SUCCESS;
}

int mkGUI(void *_img0){
	vtor(IMG0, img0);	//img0の変換
	contGUI = img0;
	contGUI_a = true;
	HINSTANCE	hInstance;
	TCHAR*		cpClassName;
	TCHAR*		cpWindowName;
	TCHAR*		cpMenu;
	MSG			tMsg;
	HWND		hWnd;
	WNDCLASSEX	tWndClass;
	bool menuFlag = false;
	RECT wSize = {0, 0, img0.x, img0.y};

	hInstance    = GetModuleHandle( NULL );
	cpClassName  = _T("GRID");
	cpMenu       = MAKEINTRESOURCE( NULL );
	cpWindowName  = _T("output window @grid01");

	if(GUIinit(tWndClass, hInstance, cpClassName, cpMenu)==RETURN_FAILED)
		return RETURN_FAILED;
	
	DWORD wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME | WS_VISIBLE;

	//クライアント領域のサイズの調整
	AdjustWindowRect(&wSize, wndStyle, FALSE);

	hWnd = CreateWindowEx (
			0						// extended window style
		, tWndClass.lpszClassName	// pointer to registered class name
		, cpWindowName				// pointer to window name
		, wndStyle					// window style
		, CW_USEDEFAULT				// horizontal position of window
		, CW_USEDEFAULT				// vertical position of window
		, wSize.right - wSize.left	// window width
		, wSize.bottom - wSize.top	// window height
		, NULL						// handle to parent or owner window
		, NULL						// handle to menu, or child-window identifier
		, hInstance					// handle to application instance
		, (VOID*)0x12345678			// pointer to window-creation data
	);
	 
	//message loop
	SetTimer(hWnd,1,50,NULL);	//今回はタイマーを使わないが、メッセージループを回すために使う。
	cui << "Esc - メニューの表示\n";
	while( 0 != GetMessage( &tMsg, NULL, 0, 0 ) ) {
		//reDraw(hWnd);	//再描画	//タイマーで何かする時になってから考える。
		if(_kbhit()){
			char k = _getch();	//特殊キーなら0xE0を返すはず
			if(k == 0xE0)
				k = _getch();
			if(menuFlag){	//メニューが表示されている
				if(k == 'q'){
					DestroyWindow(hWnd);
					cui << endl;	//改行する
				}else{
					menuFlag = false;
					cui << "\r\t\t\t\t\t\t\r";
				}
			}else{
				if(k == VK_ESCAPE){
					cui << "q : 終了、その他のキー : キャンセル\t> ";
					menuFlag = true;
				}
			}
		}
		TranslateMessage ( &tMsg );
		DispatchMessage ( &tMsg );
	}
 
	// WM_QUITの終了コードを返却する
	//return( tMsg.wParam );
	return RETURN_SUCCESS;
}

int GUItest(char trush[] = NULL){
	IMG0 i;
	i.x = 640;
	i.y = 480;
	i.data = ma(COLORREF, i.x * i.y);
	auto cutHead = [](int num){
		num = num % (255*2);
		if(num > 255)
			num = 255-num;
		return num;
	};
	//memset(i.data, 0, sizeof(COLORREF)*i.x*i.y);
	//ダミーの画像データ
	for(int t=0;t<i.y;t++)
		for(int n=0;n<i.x;n++)
			i.data[t*i.x+n]= RGB(cutHead(t), cutHead(n), 255);
	//枠の追加
	for(int x=0; x<i.x; x++)
		i.data[i.x*(i.y-1)+x] = RGB(0,0,0);
	for(int y=0; y<i.y; y++)
		i.data[i.x*y-1] = RGB(0,0,0);
	return mkGUI(&i);
}

LRESULT CALLBACK eMainWindowProc(HWND   hWnd, UINT   uMsg, WPARAM wParam, LPARAM lParam){
	//ローカル変数
	static int xb = 0, yb = 0;
	static HBITMAP hBitmap;
	static HDC hMemDC;	//off screen
	static int mx = -1, my = -1;

	switch( uMsg ) {
	case WM_CREATE:
		{
			HDC hdc;
			CREATESTRUCT* tpCreateSt = (CREATESTRUCT*)lParam;
			//DCコンパチブルの作成
			hdc = GetDC(hWnd);
			hMemDC = CreateCompatibleDC(hdc);
			hBitmap = CreateCompatibleBitmap(hdc, contGUI.x, contGUI.y);
			SelectObject(hMemDC, hBitmap);
			SelectObject(hMemDC, GetStockObject(DC_PEN));
			SelectObject(hMemDC, GetStockObject(DC_BRUSH));
			ReleaseDC(hWnd, hdc);
#ifdef __debug__
			//パラメータ表示
			wprintf(
				L"CREATESTRUCT\n"
				L"\tlpCreateParams = 0x%08x\n"
				L"\thInstance      = 0x%08x\n"
				L"\thMenu          = 0x%08x\n"
				L"\thwndParent     = 0x%08x\n"
				L"\tcy             = %d\n"
				L"\tcx             = %d\n"
				L"\ty              = %d\n"
				L"\tx              = %d\n"
				L"\tstyle          = 0x%08x\n"
				L"\tlpszName       = \"%s\"\n"
				L"\tlpszClass      = \"%s\"\n"
				L"\tdwExStyle      = 0x%08x\n"
				, tpCreateSt->lpCreateParams
				, tpCreateSt->hInstance
				, tpCreateSt->hMenu
				, tpCreateSt->hwndParent
				, tpCreateSt->cy
				, tpCreateSt->cx
				, tpCreateSt->y
				, tpCreateSt->x
				, tpCreateSt->style
				, tpCreateSt->lpszName
				, tpCreateSt->lpszClass
				, tpCreateSt->dwExStyle
			);
#endif
			//↓タイマーを使用するときになったら移動する
			for(int x=0; x<contGUI.x; x++){
				for(int y=0; y<contGUI.y; y++){
					SetPixel(hMemDC, x, y, contGUI.data[x+y*contGUI.x]);
				}
			}
			// ウインドウを表示する
			ShowWindow( hWnd, SW_SHOW );
		}
		break;
 
	case WM_ERASEBKGND:
		// 何も処理しない⇒塗り潰した
		return 1;
		break;

	case WM_TIMER:
		//タイマー処理を行う時のguitestの中身
		/*for(int x=0;x<255;x++)
				for(int y=0;y<255;y++)
					SetPixel(hMemDC, x, y, RGB((x+xb)%256, (y+yb)%256, 255));
		xb++;
		yb++;
		xb %= 256;
		yb %= 256;*/
		break;

	case WM_CLOSE:
		// DCコンパチブルの破棄
		DeleteDC(hMemDC);
		DeleteObject(hBitmap);
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		// 終了する (引数はそのまま終了コード)
		PostQuitMessage(0);
		break;

	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			COLORREF col = RGB(127, 255, 127);
			hdc = BeginPaint(hWnd , &ps);
			//裏画面処理
			BitBlt(hdc, 0, 0, contGUI.x, contGUI.y, hMemDC, 0, 0, SRCCOPY);
			EndPaint(hWnd , &ps);
		}
		break;
#ifdef __debug_pointing__
	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		reDraw(hWnd);
		//cui << "(" << mx << ", " << my << ")\n";
		break ;
	case WM_LBUTTONDOWN:
		cui << "\r\t\t\t\rx = " << mx << ", y = " << my << "\t";
		SetPixel(hMemDC, mx, my, RGB(255,0,0));
		break;
#endif
	}
 
	// デフォルト処理呼び出し
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

#endif
