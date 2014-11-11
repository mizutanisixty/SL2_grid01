#ifndef __grid_mng_Error__
#define __grid_mng_Error__
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include "flag.h"

class errCode{
private:
	unsigned long error;
	//char *_reason;
	std::string _reason;
public:
	errCode(){};
	errCode(char str[],unsigned long code){
		//_reason = (char*)malloc(sizeof(char)*(strlen(str)+1));
		//sprintf(_reason,"%s",str);
		_reason = str;
		error = code;
	};
	~errCode(){
		//free(_reason);
	};
	unsigned long operator = (int){
		return error;
	};
	//エラーコードの呼び出し
	unsigned long call(){
		return error;
	};
	//エラー理由の呼び出し
	/*char* reason(){
		return _reason;
	};*/
	std::string reason(){
		return _reason;
	};
};

class mngError{
private:
	unsigned long error;
public:
	mngError(){
		error=0;
	};
	~mngError(){};
	int showErr();
	void add(errCode code){
		error |= code.call();
#ifdef __debug__
		cui<<"add error code : "<<code.reason()<<"\n";
#endif
	};
	void del(errCode code){
		/*add(code);
		error ^= code;*/
		error &= ~code.call();
	};
	bool chk(errCode code){
		return (error&code.call() ? true : false);
	};
};


errCode E_nInit("root pointer is not initialized.",0x0001);
errCode E_overRange("searching position is out of range.",0x0002);
errCode E_illegalMemAccess("accessed to illegal range of memory adress.",0x0004);
errCode E_failedMemAllocation("malloc returned null pointer",0x0008);
errCode E_nSetNul("nul value of dynamic isn't set",0x0010);

errCode errList[] = {
	E_nInit,
	E_overRange,
	E_illegalMemAccess,
	E_failedMemAllocation,
	E_nSetNul
};

int mngError::showErr(){
		for(int i=0;i<_countof(errList);i++)
			cui << errList[i].reason() << "\n";
		return error;
};

#endif
