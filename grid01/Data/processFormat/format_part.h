#ifndef __grid_format_part__
#define __grid_format_part__
//�l�b�g���[�N�Ԃł̃f�[�^�󂯓n���̍ۂ�
//��M�����̊��N���X�ɂȂ�\��

#include "includer.h"
#include "dynamic.h"
#include "tree.h"


using namespace std;

#ifdef __format_ver_01__
template <class T> class partData{
private:
	string str;
	const char *p;
	long end, cnt, num;
	string header;
	Tree<T> strTree;
	long depth;
	bool setNul;
	
public:
	int mkChild(Tree& t,long length){
		t.add(length);
	};

	int cut(string data){
		bool convFlag = false;
		bool isThisString = false;
		bool success = true;
		depth = 0;
		cnt=0;
		str = data;
		Tree& point = strTree;	//Seeker
		p = str.c_str();
		end = str.length();
		for(long i=0; i<end; i++){
			//--------------------------------------------------
			//
			//	TODO : �����ɋ^������؂蕪���������L�q
			//
			//--------------------------------------------------
			switch(data.c_str[i]){
			case '{':
				depth++;
				break;
			case '}':
				depth--;
				if(depth == 0){
					if(i != end-1)
						success = false;
					i = end;
				}
				break;
			case '[':
				if(data.c_str[i+1] == '~')
					convFlag = true;
				break;
			case ']':
				convFlag = false;
				break;
			case '\"':
				isThisString = !isThisString;
				//isThisString ^= 1;	�Ƃ��ł����Ǝv�������萫�d��
				break;
			case '\'':
				break;
				break;
			case '^':
				break;
			case '|':
				break;
			case ',':
				break;
			default:
				if(depth == 0)
					header += data.c_str[i];
				if(isThisString){
				};
			}	//End of switch
		}	//End of loop
	};	//End of cut(string data)
	partData(){};
	partData(string data){
		strTree.free();
	};
	~partData(){
		
	};
};
#endif
#ifdef __format_ver_02__
template <class T> class partData{
private:
public:
	partData(){};
	~partData(){
		
	};
	
};
#endif

#endif
