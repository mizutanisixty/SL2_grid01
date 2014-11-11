#ifndef __grid_tree__
#define __grid_tree__
#include "includer.h"
#include "dynamic.h"
#include "map.h"

#ifdef __data_ver_01__
template <class T> struct Tree{
	T data;
	dynamic<Tree> child;
	bool isThereChild;
	int free(){
		if(isThereChild){
			for(long i=child.getSize();i--;){
				*(child.call(i)).free();
			}
			child.~dynamic();
		}
		return 1;
	};
	
};
#endif
#ifdef __data_ver_02__
template <class T> class Tree{
private:
	dynamic<Tree<T>> child;
	string tag;
	long depth;
	Tree& parent;
	bool parent_init;

public:
	Tree(){
		depth = 0;
		parent_init = false;
	};
	set(long _depth, Tree& _parent){
		depth = _depth;
		parent = _parent;
		parent_init = true;
	};
	void set(long _depth){
		depth = _depth;
		if(depth==0)
			parent = (*this),
			parent_init = true;
		else
			parent_init = false;
	};
	long operator++ (int){
		return child++;
	};
	long operator-- (int){
		return child--;
	};
	int add(long plus){
		child.add(plus);
	};
	int del(long minus){
		child.del(minus);
	};
	void free(){
		if(child.getSize == 0)
			return ;
		for(long i=0;i<child.getSize();i++)
			child.c(i).free();
		child.~dynamic();
	}	
};
#endif
#ifdef __data_ver_03__
template<class T> class Tree{
private:
	map<Tree<T>> child;	//デストラクタ呼び出し処理を後で追加
	//vv-before-vv
	string tag;
	long depth;
	Tree& parent;
	bool parent_init;
	//^^-before-^^
public:
};
#endif
#endif
