#ifndef __grid_map__
#define __grid_map__
#include "includer.h"
#include <iostream>
#include "mngError.h"
#include "dynamic.h"
#include <string>

#ifdef add_one_and_new
#undef add_one_and_new
#endif
#define add_one_and_new(arr, T, pos) arr.add(1), arr[pos] = new T

using namespace std;

template <class T> class map{
private:
	dynamic<string*> tag;
	dynamic<T> data;
	dynamic<bool> isThisAvaible;
public:
	map(){};
	~map(){
		long tmp = tag.getSize();
		for(long i=0;i<tmp;i++)
			delete tag[i];
	};

	//isAvailable
	int pos(string str){
		long tmp = tag.getSize();
		for(long i=0;i<tmp;i++)
		//	cui<<*tag.c(i)<<endl;
			if((*tag[i]) == str && isThisAvaible[i])
				return i;
		return -1;
	};
	bool a(string str){
		return pos(str) != -1;
	};
	//呼び出し(参照渡し)
	T& call(string str){
		long tmp = pos(str);
		if(tmp != -1)
			return data[tmp];

		tmp = tag.getSize();
		//存在しなかったら作る
		for(long i=0;i<tmp;i++)
			if(!isThisAvaible[i]){
				*tag[i] = str;
				isThisAvaible[i] = true;
#ifdef __debug__
		cui << "\n\t\t > (+) added an item to a map\n";
#endif
				//return data[data.getSize()-1];	//謎なので一応コメントアウト
				return data[i];
			}
		/*tag.add(1);
		tag.c(1) = new string;*/
		add_one_and_new(tag, string, tmp);
		//tmp = tag.getSize()-1;
		*tag[tmp] = str;
		//cui << *tag.c(tmp) << endl;
		isThisAvaible.add(1);
		isThisAvaible[tmp] = true;
#ifdef __debug__
		cui << "\n\t\t > (+) added an item to a map [" << str << "]\n";
#endif
		data.add(1);
		return data[tmp];
	};

	//添え字のオーバーロードで呼び出し
	T& operator[](string str){
		return call(str);
	};
	T& operator[](char* str){
		string _str(str);
		return call(_str);
	};
	int operator()(string str){
		return del(str);
	};
	int operator()(char* str){
		string _str(str);
		return del(_str);
	};
	int del(string str){
		long i = pos(str);
		if(i==-1)
			return 0;
		isThisAvaible[i] = false;
#ifdef __debug__
		cui << "\n\t\t > (-) deleted an item from a map [" << str << "]\n";
#endif
		return 1;
	};
	
};

#endif
