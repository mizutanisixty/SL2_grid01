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
	void chSize(long ch);	//�T�C�Y�ύX(add,del�ŌĂяo��)
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
	void init();		//������
	dynamic();		//�R���X�g���N�^
	dynamic(long plus);
	~dynamic();	//�f�X�g���N�^
	int add(long plus);		//�����ɒǉ�
	T* call(long next);	//�Ăяo��
	T* c();				//���݈ʒu���Ăяo��
	int del(long minus);	//�폜
	int setPos(long p);		//position�̎w��(��Έʒu)
	int movePos(long p);	//position�̎w��(���Έʒu)
	long getPos();			//position�̎擾
	long getSize();			//size�̎擾
	int set(long p,T s);				//call�̏�������t��ver
	T c(long next);		//T�Ăяo���ȈՔ�
};


//������
template<class T> void dynamic<T>::init(){
	status = 0;
	point = 0;
	size = 0;
#ifdef __debug__
	cui<<"sizeof(T) = "<<sizeof(T)<<"\n";
#endif
};

//�R���X�g���N�^(�������Ȃ�)
template<class T> dynamic<T>::dynamic(){
	init();
	rt=(T*)malloc(sizeof(T));
	e.add(E_nInit);
};

//�R���X�g���N�^(����������)
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

//�f�X�g���N�^
template<class T> dynamic<T>::~dynamic(){
	try{
		free(rt);
		e.~mngError();
	}catch(mngError e){
		
	}
}

//�Ăяo��
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

//call�̏�������t��ver
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//char s��T s�ɕύX�B�g���[�X�����ɂ�
//�댯�����m�B
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

//���݈ʒu���Ăяo��
template<class T> T* dynamic<T>::c(){
	return call(point);
};

//T�Ăяo���ȈՔ�
template<class T> T dynamic<T>::c(long next){
	T *p;
	p = call(next);
	if(p){
		return *p;
	}
	e.add(E_overRange);
	return NULL;
};

//�T�C�Y�ύX
template<class T> void dynamic<T>::chSize(long plus){
	T *temp;
	
	//�������Ȃ��G���[������
	e.del(E_nInit);
	//�V���Ɋm��
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
//���������Z�b�g�������R�����g�A�E�g
//�N���X�Ăяo���̍ۂ̏���������l����B
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
	
	//�ڂ��ւ�
///	for(long i=0;i<(Min<int>(size)(size+plus));i++)
	for(long i=0;i<(plus>0 ? size : size+plus);i++)
		temp[i] = rt[i];
	//�Â��ق������
	free(rt);
	//�T�C�Y�X�V
	size += plus;
	rt = temp;
};

//�����ɒǉ�
template<class T> int dynamic<T>::add(long plus){
	if(plus<1)
		return NULL;

	chSize(plus);

	if(e.chk(E_failedMemAllocation))
		return NULL;
	//����
	return 1;
};

//�������폜
template<class T> int dynamic<T>::del(long minus){
	if(minus>size)
		return NULL;

	chSize(-minus);

	if(e.chk(E_failedMemAllocation))
		return NULL;
	//����
	return 1;
};

//position�̎w��(��Έʒu)
template<class T> int dynamic<T>::setPos(long p){
	if(0<=p && p<size){
		point = p;
		return 1;
	}else{
		return NULL;
	}
};

//position�̎w��(���Έʒu)
template<class T> int dynamic<T>::movePos(long p){
	return setPos(point + p);
};

//position�̎擾
template<class T> long dynamic<T>::getPos(){
	return point;
};

//size�̎擾
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
	void chSize(long ch);	//�T�C�Y�ύX(add,del�ŌĂяo��)
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
	void init();		//������
	void init(T _nul);
	dynamic();		//�R���X�g���N�^
	dynamic(T _nul);
	dynamic(T _nul, long plus);
	~dynamic();	//�f�X�g���N�^
	int add(long plus);		//�����ɒǉ�
	T* call(long next);	//�Ăяo��
	T& c();				//���݈ʒu���Ăяo��
	T& c(long next);
	int del(long minus);	//�폜
	int setPos(long p);		//position�̎w��(��Έʒu)
	int movePos(long p);	//position�̎w��(���Έʒu)
	long getPos();			//position�̎擾
	long getSize();			//size�̎擾
	T set(T s);	//call�̏�������t��ver
	T set(T s,long p);	//call�̏�������t��ver
	//int newing(long from, long to);
	int a();			//is available(before,next,this)
	int a(long pos);
	mngError getError();	//�G���[�̏o��
};


//������
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

//�R���X�g���N�^(�������Ȃ�)
template<class T> dynamic<T>::dynamic(){
	init();
	e.add(E_nInit);
	e.add(E_nSetNul);
};

//�R���X�g���N�^(����������)
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

//�f�X�g���N�^
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

//�Ăяo��
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

//call�̏�������t��ver
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//char s��T s�ɕύX�B�g���[�X�����ɂ�
//�댯�����m�B
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

//���݈ʒu���Ăяo��
template<class T> T& dynamic<T>::c(){
	return *call(point);
};

//T�Ăяo���ȈՔ�
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

/*//�R���X�g���N�^
template<class T> int dynamic<T>::newing(long from, long to){
	T* tmp = rt+from;
	for(;from <= to; from++)
		*(tmp++) = new T;
};*/

//�T�C�Y�ύX
template<class T> void dynamic<T>::chSize(long plus){
	T *temp;
	//�V���Ɋm��
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
		//�������Ȃ��G���[������
		e.del(E_nInit);
	}
	//������
	//******************************
	//------------------------------
	//
	//�d�v�F�ꎞ�I�ɃR�����g�A�E�g
	//
	//------------------------------
	//******************************
	/*if(!e.chk(E_nSetNul))
		for(long i=size-1;i<size+plus-1;i++)
			rt[i] = nul;*/
	//�T�C�Y�X�V
	size += plus;
	rt = temp;
};

//�����ɒǉ�
template<class T> int dynamic<T>::add(long plus){
	if(plus<1)
		return NULL;
	chSize(plus);

	if(e.chk(E_failedMemAllocation))
		return NULL;
	//����
	return 1;
};

//�������폜
template<class T> int dynamic<T>::del(long minus){
	if(minus>=size)
		return NULL;

	chSize(-minus);

	if(e.chk(E_failedMemAllocation))
		return NULL;
	//����
	return 1;
};

//position�̎w��(��Έʒu)
template<class T> int dynamic<T>::setPos(long p){
	if(0<=p && p<size){
		point = p;
		e.del(E_overRange);
		return 1;
	}else{
		return NULL;
	}
};

//position�̎w��(���Έʒu)
template<class T> int dynamic<T>::movePos(long p){
	return setPos(point + p);
};

//position�̎擾
template<class T> long dynamic<T>::getPos(){
	return point;
};

//size�̎擾
template<class T> long dynamic<T>::getSize(){
	return size;
};

//�G���[�̎擾
template<class T> mngError dynamic<T>::getError(){
	return e;
};

#endif


#endif
