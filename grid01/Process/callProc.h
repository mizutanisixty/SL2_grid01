#ifndef __grid_callProc__
#define __grid_callProc__
#include "includer.h"
#include "signal_format.h"

using namespace std;

template <class T, class U, class X, class Y, class Z> class callProc{
private:
	HINSTANCE hDll;
	bool isA;
	T (__stdcall *proc)(CUI*, U, X, Y, Z);
public:
	callProc(){
		isA = false;
	};
	~callProc(){
		FreeLibrary(hDll);
#ifdef __debug__
		cui << "dll handle deleted.\n";
#endif
	};

	//¬Œ÷F1AŽ¸”sF0
	int set(LPCWSTR path, LPCSTR funcName){
		isA = false;
		hDll = ::LoadLibraryEx(path, NULL, 0);
		if(!hDll){
#ifdef __debug__
			cui << "open err"<<endl;
#endif
			return 0;
		}
#ifdef __debug__
		cui << "dll file opened.\n";
#endif
		proc = (T(__stdcall*)(CUI*, U, X, Y, Z)) ::GetProcAddress(hDll, funcName);
		if(!proc){
			FreeLibrary( hDll );
#ifdef __debug__
			cui << "read err"<<endl;
#endif
			return 0;
		}
#ifdef __debug__
		cui << "dll handle created.\n";
#endif
		isA = true;
		return 1;
	};

	bool a(){
		return isA;
	};

	T operator() (U i, X x, Y y, Z z){
		return proc(&cui, i, x, y, z);
	};
};

#endif
