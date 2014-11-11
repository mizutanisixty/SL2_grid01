#ifndef __grid_math__
#define __grid_math__
#include <string>
#include "flag.h"
using namespace std;
template <class T> struct Math{
	Math(){};
	~Math(){};
	int itoa(T i, char ret[]){
		char _ret[64]={0,};
		int cnt=1,cnt2=0;
		bool isMinus = false;
		if(i<0)
			isMinus = true,
			i = 0 - i;
		do{
			_ret[cnt++] = (i%10) + '0';
		}while(i/=10,i);
		if(isMinus)
			_ret[cnt++] = '-';
		while(cnt2,--cnt)
			ret[cnt2++] = _ret[cnt];
		return 1;
	};
	T atoi(string str){
		T tmp=0, k=1;
		int len;
		char _s[256];
		strcpy(_s, str.c_str());
		char *p = _s;
		bool isMinus = (p[0]=='-');
		if(isMinus) p++;
		len = strlen(p);
		while(len--)
			tmp+=k*(p[len]-'0'),k*=10;
		if(isMinus)
			tmp = 0 - tmp;
		return tmp;
	};
};
#endif
