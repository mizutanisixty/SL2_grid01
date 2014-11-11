#ifndef __grid_prove__
#define __grid_prove__

#include "includer.h"
/**
認証部分
後回し。
initでハッシュ生成
ハンドル(^)を使うかどうかはそのうち考える
*/

template <class T>
class prove{
private:
	T id;
	T* id_p;
	long id_size;
	bool isIdPointer;
	unsigned long long hash;
	unsigned long seed;
	unsigned long status;
	mngError e;
	void init();
public:
	prove(T ID){
		isIdPointer = false;
		id = ID;
		init();
	};
	prove(T *ID,long size){
		isIdPointer = true;
		id_p = (T*)malloc(sizeof(T)*size);
		for(long i=0;i<size;i++)
			id_p[i] = ID[p];
		init();
	};
	~prove(){
		if(isIdPointer)
			free(id_p);
	};
	T operator= (int){
		return id;
	}

};

template<class T> void prove<T>::init(){

};

#endif
