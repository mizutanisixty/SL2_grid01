#ifndef __grid_algorithm_max__
#define __grid_alcorithm_max__

template <class T_value>

struct Max {
	T_value m_tv;
	Max(const T_value& tv) { m_tv = tv; }
	Max& operator()(const T_value& tv) {
		m_tv = m_tv > tv ? m_tv : tv; 
		return *this;
	}
	operator T_value() { return m_tv; }
};


//使い方
//int iMax = Max<int>(3)(4)(5)(-2)(1);
//みたいな感じ。
//最初の()はコンストラクタ、二番目からは関数呼び出し。

#endif
