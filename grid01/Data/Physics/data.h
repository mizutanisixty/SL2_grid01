#ifndef __grid_data__
#define __grid_data__
#include "includer.h"
#include "dynamic.h"
//	(2014/06/18)	data�N���X�̑��݈Ӌ`���悭������Ȃ��Ȃ����̂ŃR�����g�A�E�g
/*
class data{
private:
	unsigned long long id;
	unsigned long long hash;
	unsigned long status;
	unsigned long size;
	dynamic<void*> rt;
	dynamic<bool> isNull;
	mngError e;
public:
	data();
	~data();
	void** call(unsigned long long num){
		return rt.call(num);
	};
	int add(unsigned long long plus);
	int del(unsigned long long minus);
	int noticeInit(unsigned long long pos);
	int noticeDestroy(unsigned long long pos);
	int destroy(unsigned long long pos);
};

data::data(){};
data::~data(){};
//�����ɒǉ�
int data::add(unsigned long long plus){
		rt.add(plus);
		//rt = (void**)malloc(sizeof(void*)*(size+plus));
		//������isNull�̖����ǉ��R�[�h���L��
		return 0;
};
//�������폜
int data::del(unsigned long long pos){
	return 0;
};
//�����ʒm
int data::noticeInit(unsigned long long pos){
	return 0;
};
//�j���ʒm
int data::noticeDestroy(unsigned long long pos){
	*isNull.call(pos) = false;
	return 0;
};
//�j��
int data::destroy(unsigned long long pos){
	try{
		delete rt.call(pos);
		e.del(E_illegalMemAccess);
	}catch(errCode ec){
		e.add(E_illegalMemAccess);
#ifdef __debug__
		cui << ec.reason() << "\n";
#endif
	}
	return 0;
};
*/
#endif
