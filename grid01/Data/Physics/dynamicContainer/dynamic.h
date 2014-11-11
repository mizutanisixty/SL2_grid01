#ifndef __grid_data_dynamic__
#define __grid_data_dynamic__
#include <cstdio>
#include <cstdlib>
#include <string>
#include <memory>
#include <iostream>
#include "mngError.h"

#ifdef __dynamic_ver_01__
template <class T> class dynamic{
private:
	unsigned long status;
	mngError e;
	long size;
	T *rt;
	long point;
	void chSize(long ch);	//サイズ変更(add,delで呼び出し)
	//friend int operator+(dynamic&);
public:
	operator long(){return point;};
	long operator++(int){
		if(point!=size-1)
			return point++;
		e.add(E_overRange);
		return -1;
	};
	long operator-- (int){
		if(point)
			return point--;
		e.add(E_overRange);
		return -1;
	};
	void init();		//初期化
	dynamic();		//コンストラクタ
	dynamic(long plus);
	~dynamic();	//デストラクタ
	int add(long plus);		//末尾に追加
	T* call(long next);	//呼び出し
	T* c();				//現在位置を呼び出し
	int del(long minus);	//削除
	int setPos(long p);		//positionの指定(絶対位置)
	int movePos(long p);	//positionの指定(相対位置)
	long getPos();			//positionの取得
	long getSize();			//sizeの取得
	int set(long p,T s);				//callの条件分岐付きver
	T c(long next);		//T呼び出し簡易版
};


//初期化
template<class T> void dynamic<T>::init(){
	status = 0;
	point = 0;
	size = 0;
#ifdef __debug__
	cui<<"sizeof(T) = "<<sizeof(T)<<"\n";
#endif
};

//コンストラクタ(初期化なし)
template<class T> dynamic<T>::dynamic(){
	init();
	rt=(T*)malloc(sizeof(T));
	e.add(E_nInit);
};

//コンストラクタ(初期化あり)
template<class T> dynamic<T>::dynamic(long s){
	init();
	if(s>0){
		rt = (T*)malloc(sizeof(T)*s);
		size=s;
	}else{
		rt=(T*)malloc(sizeof(T));
		e.add(E_nInit);
	}
};

//デストラクタ
template<class T> dynamic<T>::~dynamic(){
	try{
		free(rt);
		e.~mngError();
	}catch(mngError e){
		
	}
}

//呼び出し
template<class T> T* dynamic<T>::call(long next){
	if(!e.chk(E_nInit))
		if(next<size)
			return rt + next;
#ifdef __debug__
		else
			cui << "(!) called with wrong size\n";
	else
		cui << "(!) called without initialization\n";
#endif
	return NULL;
};

//callの条件分岐付きver
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//char sをT sに変更。トレース無しにつき
//危険性未知。
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
template<class T> int dynamic<T>::set(long p,T s){
	if(call(p)){
		*call(p)=s;
		return 1;
	}else{
		return NULL;
	}
};

//現在位置を呼び出し
template<class T> T* dynamic<T>::c(){
	return call(point);
};

//T呼び出し簡易版
template<class T> T dynamic<T>::c(long next){
	T *p;
	p = call(next);
	if(p){
		return *p;
	}
	e.add(E_overRange);
	return NULL;
};

//サイズ変更
template<class T> void dynamic<T>::chSize(long plus){
	T *temp;
	
	//初期化なしエラーを解除
	e.del(E_nInit);
	//新たに確保
	temp = (T*)malloc(sizeof(T)*(size+plus));
	if(!temp){
		e.add(E_failedMemAllocation);
		return ;
	}
	/*
#ifdef __debug__
	cui<<"sizeof(T)*(size+plus) = "<<sizeof(T)*(size+plus)<<"\n";
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//メモリリセット処理をコメントアウト
//クラス呼び出しの際の条件分岐を考える。
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	cui<<"Zero-Memoring\n";
#endif
	for(long i=0;i<size+plus;i++)
		temp[i]=NULL;
	//memset(*t,0,sizeof(T)*(size+plus));
#ifdef __debug__
	cui<<"Zero-Memoried\n";
#endif
	*/
	
	//移し替え
///	for(long i=0;i<(Min<int>(size)(size+plus));i++)
	for(long i=0;i<(plus>0 ? size : size+plus);i++)
		temp[i] = rt[i];
	//古いほうを解放
	free(rt);
	//サイズ更新
	size += plus;
	rt = temp;
};

//末尾に追加
template<class T> int dynamic<T>::add(long plus){
	if(plus<1)
		return NULL;

	chSize(plus);

	if(e.chk(E_failedMemAllocation))
		return NULL;
	//成功
	return 1;
};

//末尾を削除
template<class T> int dynamic<T>::del(long minus){
	if(minus>size)
		return NULL;

	chSize(-minus);

	if(e.chk(E_failedMemAllocation))
		return NULL;
	//成功
	return 1;
};

//positionの指定(絶対位置)
template<class T> int dynamic<T>::setPos(long p){
	if(0<=p && p<size){
		point = p;
		return 1;
	}else{
		return NULL;
	}
};

//positionの指定(相対位置)
template<class T> int dynamic<T>::movePos(long p){
	return setPos(point + p);
};

//positionの取得
template<class T> long dynamic<T>::getPos(){
	return point;
};

//sizeの取得
template<class T> long dynamic<T>::getSize(){
	return size;
};
#endif
#ifdef __dynamic_ver_02__

#define aBefore	0x04
#define aNext	0x02
#define aThis	0x01

template <class T> class dynamic{
private:
	unsigned long status;
	mngError e;
	long size;
	T *rt;
	T nul;
	long point;
	void chSize(long ch);	//サイズ変更(add,delで呼び出し)
	//friend int operator+(dynamic&);
public:
	operator long(){return point;};
	long operator++(int){
		if(point!=size-1)
			return point++;
		e.add(E_overRange);
		return -1;
	};
	long operator-- (int){
		if(point)
			return point--;
		e.add(E_overRange);
		return -1;
	};
	T& operator[] (long next){return c(next);};
	void init();		//初期化
	void init(T _nul);
	dynamic();		//コンストラクタ
	dynamic(T _nul);
	dynamic(T _nul, long plus);
	~dynamic();	//デストラクタ
	int add(long plus);		//末尾に追加
	T* call(long next);	//呼び出し
	T& c();				//現在位置を呼び出し
	T& c(long next);
	int del(long minus);	//削除
	int setPos(long p);		//positionの指定(絶対位置)
	int movePos(long p);	//positionの指定(相対位置)
	long getPos();			//positionの取得
	long getSize();			//sizeの取得
	T set(T s);	//callの条件分岐付きver
	T set(T s,long p);	//callの条件分岐付きver
	//int newing(long from, long to);
	int a();			//is available(before,next,this)
	int a(long pos);
	mngError getError();	//エラーの出力
};


//初期化
template<class T> void dynamic<T>::init(){
	status = 0;
	point = 0;
	size = 0;
#ifdef __debug__
	cui<<"sizeof(T) = "<<sizeof(T)<<"\n";
#endif
};
template<class T> void dynamic<T>::init(T _nul){
	nul = _nul;
	status = 0;
	point = 0;
	size = 0;
#ifdef __debug__
	cui<<"sizeof(T) = "<<sizeof(T)<<"\n";
#endif
};

//コンストラクタ(初期化なし)
template<class T> dynamic<T>::dynamic(){
	init();
	e.add(E_nInit);
	e.add(E_nSetNul);
};

//コンストラクタ(初期化あり)
template<class T> dynamic<T>::dynamic(T _nul){
	init(_nul);
	e.add(E_nInit);
};

template<class T> dynamic<T>::dynamic(T _nul,long s){
	init(_nul);
	if(s>0){
		rt = (T*)malloc(sizeof(T)*s);
		size=s;
	}else{
		e.add(E_nInit);
	}
};

//デストラクタ
template<class T> dynamic<T>::~dynamic(){
		free(rt);
		//e.~mngError();
}

//is available
template<class T> int dynamic<T>::a(){
	return (point?aBefore:NULL) | (point<size-1?aNext:NULL) | (0<=point&&point<size?aThis:NULL);
};

template<class T> int dynamic<T>::a(long pos){
	return (pos?aBefore:NULL) | (pos<size-1?aNext:NULL) | (0<=pos&&pos<size?aThis:NULL);
};

//呼び出し
template<class T> T* dynamic<T>::call(long next){
	if(!e.chk(E_nInit))
		if(next<size)
			return rt + next;
#ifdef __debug__
		else
			cui << "(!) called with wrong size\n";
	else
		cui << "(!) called without initialization\n";
#endif
	return NULL;
};

//callの条件分岐付きver
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//char sをT sに変更。トレース無しにつき
//危険性未知。
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
template<class T> T dynamic<T>::set(T s){
	if(call(point)){
		*call(point)=s;
		return s;
	}else{
#ifdef __debug__
		cui << "(!) error in dynamic<T>::set\n";
#endif
		return nul;
	}
};
template<class T> T dynamic<T>::set(T s, long p){
	if(call(p)){
		*call(p)=s;
		return s;
	}else{
		return nul;
	}
};

//現在位置を呼び出し
template<class T> T& dynamic<T>::c(){
	return *call(point);
};

//T呼び出し簡易版
template<class T> T& dynamic<T>::c(long next){
	T *p;
	p = call(next);
	if(p){
		return *p;
	}
	e.add(E_overRange);
	//return NULL;
	return nul;
};

/*//コンストラクタ
template<class T> int dynamic<T>::newing(long from, long to){
	T* tmp = rt+from;
	for(;from <= to; from++)
		*(tmp++) = new T;
};*/

//サイズ変更
template<class T> void dynamic<T>::chSize(long plus){
	T *temp;
	//新たに確保
	if(size<=0 || e.chk(E_nInit))
		temp = (T*)malloc(sizeof(T)*(size+plus));
	else
		temp = (T*)realloc(rt,sizeof(T)*(size+plus));
	if(!temp){
		e.add(E_failedMemAllocation);
#ifdef __debug__
		cui << "(!) failed to change memory size\n";
#endif
		return ;
	}else{
		//初期化なしエラーを解除
		e.del(E_nInit);
	}
	//初期化
	//******************************
	//------------------------------
	//
	//重要：一時的にコメントアウト
	//
	//------------------------------
	//******************************
	/*if(!e.chk(E_nSetNul))
		for(long i=size-1;i<size+plus-1;i++)
			rt[i] = nul;*/
	//サイズ更新
	size += plus;
	rt = temp;
};

//末尾に追加
template<class T> int dynamic<T>::add(long plus){
	if(plus<1)
		return NULL;
	chSize(plus);

	if(e.chk(E_failedMemAllocation))
		return NULL;
	//成功
	return 1;
};

//末尾を削除
template<class T> int dynamic<T>::del(long minus){
	if(minus>=size)
		return NULL;

	chSize(-minus);

	if(e.chk(E_failedMemAllocation))
		return NULL;
	//成功
	return 1;
};

//positionの指定(絶対位置)
template<class T> int dynamic<T>::setPos(long p){
	if(0<=p && p<size){
		point = p;
		e.del(E_overRange);
		return 1;
	}else{
		return NULL;
	}
};

//positionの指定(相対位置)
template<class T> int dynamic<T>::movePos(long p){
	return setPos(point + p);
};

//positionの取得
template<class T> long dynamic<T>::getPos(){
	return point;
};

//sizeの取得
template<class T> long dynamic<T>::getSize(){
	return size;
};

//エラーの取得
template<class T> mngError dynamic<T>::getError(){
	return e;
};

#endif


#endif
