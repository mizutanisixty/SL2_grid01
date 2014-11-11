#ifndef __grid_algorithm_min__
#define __grid_alcorithm_min__

template <class T_value>

struct Min{
	T_value m_tv;
	Min(const T_value& tv) { m_tv = tv; }
	Min& operator()(const T_value& tv) {
		m_tv = m_tv < tv ? m_tv : tv; 
		return *this;
	}
	operator T_value() { return m_tv; }
};

//使い方
//int iMin = Min<int>(3)(4)(5)(-2)(1);
//みたいな感じ。
//最初の()はコンストラクタ、二番目からは関数呼び出し。

#endif
