#ifndef __grid_algorithm_power__
#define __grid_algorithm_power__

unsigned long long power(long long n,long long k){
	unsigned long long tmp = n, ans = 1;
	while(k){
		if(k&1)
			ans *= tmp;
		tmp *= tmp;
		k>>=1;
	}
	return ans;
}

template <class T> T power(T n,T k){
	T tmp = n, ans = 1;
	while(k){
		if(k&1)
			ans *= tmp;
		tmp *= tmp;
		k>>=1;
	}
	return ans;
}

/*
unsigned long long power_old(long long n,long long k){
	unsigned long long tmp = n;
	int p = 0,i = 0;
	while(1<<++i<k)
		p++;
	k -= 1<<p;
	while(p--)
		tmp *= tmp;
	while(k--)
		tmp *= n;
	return tmp;
}
*/
#endif
