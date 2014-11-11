#ifndef __grid_advanced_integer__
#define __grid_advanced_integer__
#include "includer_algorithm.h"

template <class T>
class advInt{
private:
	T i, temp;
public:
	advInt(){
		i = temp = 0;
	};
	advInt(T val){
		i = val;
		temp = 0;
	};
	~advInt(){};

	//四則演算
	T operator+ (advInt param){
		return param.getNum() + i;
	};
	T operator+ (T param){
		return param + i;
	};
	T operator- (advInt param){
		return param.getNum() - i;
	};
	T operator- (T param){
		return param - i;
	};
	T operator* (advInt param){
		return param.getNum() * i;
	};
	T operator* (T param){
		return param * i;
	};
	T operator/ (advInt param){
		return param.getNum() / i;
	};
	T operator/ (T param){
		return param / i;
	};
	T operator% (advInt param){
		return param.getNum() % i;
	};
	T operator% (T param){
		return param % i;
	};

	//累乗
	T operator^ (advInt param){
		return power<T>(i,param.getNum());
	};
	T operator^ (T param){
		return power<T>(i,param);
	};
	T operator^= (advInt param){
		return (i = power<T>(i,param.getNum()));
	};
	T operator^= (T param){
		return (i = power<T>(i,param));
	};

	//代入
	advInt* operator= (advInt param){
		i = param.getNum();
		return *this;
	};

	advInt* operator= (T param){
		i = param;
		return *this;
	};

	//インクリメント
	T operator++ (){
		return ++i;
	};

	T operator++ (int){
		return i++;
	};

	//デクリメント
	T operator--(){
		return --i;
	};

	T operator-- (int){
		return i--;
	};

	//数値呼び出し(参照渡し)
	T& operator() (){
		return i;
	};

	//数値呼び出し(キャスト)
	operator T(){
		return i;
	};

	//スワップ
	T operator<<= (advInt& param){
		temp = param.getNum();
		param = i;
		i = temp;
		return temp;
	}
	T operator>>= (advInt& param){
		temp = param.getNum();
		param = i;
		i = temp;
		return param.getNum();
	}
	
	//比較演算子
	bool operator>(T param){
		return (i > param);
	};
	bool operator>(advInt param){
		return (i > param.getNum());
	};
	bool operator< (T param){
		return (i < param);
	};
	bool operator< (advInt param){
		return (i < param.getNum());
	};
	bool operator>= (T param){
		return (i >= param);
	};
	bool operator>= (advInt param){
		return (i >= param.getNum());
	};
	bool operator<= (T param){
		return (i <= param);
	};
	bool operator<= (advInt param){
		return (i <= param.getNum());
	};

	//数値呼び出し(実体渡し)
	T getNum(){
		return i;
	};
};

#endif
