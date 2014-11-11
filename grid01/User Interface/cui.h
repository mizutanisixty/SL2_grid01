#ifndef __grid_cui__
#define __grid_cui__

#ifdef __cui_output_ver_01__

#include <iostream>
#include <cstdio>
#include <streambuf>
#include <windows.h>

//自作出力関数(basic_streambufの継承)
template <class Ch,class Tr=std::char_traits<Ch> >
class basic_debug_streambuf : public std::basic_streambuf<Ch,Tr> {
public:
	basic_debug_streambuf(void){
		setbuf(0,0);
		AllocConsole();
	};

	~basic_debug_streambuf(void){
		FreeConsole();
	};

protected:
	std::streampos seekoff(std::streamoff off, std::ios::seek_dir dir, int nMode = std::ios::in | std::ios::out ){
		return EOF;
	};

	std::streampos seekpos(std::streampos pos, int nMode = std::ios::in | std::ios::out ){
			return EOF;
	};

	int overflow( int nCh = EOF ){
		char buffer[2];
		buffer[0]=nCh;
		DWORD size;
		WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),buffer,1,&size,NULL);
		return 0;
	};

	int underflow(void){
		return EOF;
	};

};

template <class Ch,class Tr=std::char_traits<Ch> >
class basic_debug_stream : public std::basic_iostream<Ch,Tr> {
public:
	basic_debug_stream(void) : std::basic_iostream<Ch,Tr>(new basic_debug_streambuf<Ch,Tr>()){
    };

    ~basic_debug_stream(void){
    };
};

typedef basic_debug_streambuf<char> debug_streambuf;

typedef basic_debug_stream<char> debug_stream;

#endif

#ifdef __cui_output_ver_02__

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <Windows.h>
#include <ImageHlp.h>
#pragma comment(lib, "imagehlp.lib")	//Win32API
#include "flag.h"
#include "signal_format.h"
#include "Math.h"

#ifdef add_one_and_new
#undef add_one_and_new
#endif
#define add_one_and_new(arr, T, pos) arr.add(1), arr[pos] = new T

using namespace std;

class CUI{
private:
	bool useBuf, useFile;
	ofstream ofs;
	string log_path;

public:	
	string str;

	void output(string tmp){
		if(useBuf)
			str += tmp;
		else
			cout << tmp;

		if(useFile && ofs.is_open())
			ofs << tmp;
	};

	template<typename U> U input(){
		U tmp;
		cin >> tmp;
		if(useFile && ofs.is_open())
			ofs << tmp << endl;
		return tmp;
	}

	CUI(){
		setlocale( LC_ALL, "Japanese" );	//Unicode文字の設定
		useBuf = false;
		useFile = false;
		log_path = "../logging/";
		[&]{
			string buf;
			bool found = false;
			std::ifstream ifs(setting_path);
			auto setDef = [&]{
				std::ofstream ofs(setting_path, ios::app);
				if(ofs.fail())
					return ;
				//"デフォルトの書き込み"
				ofs << "log_path=" << log_path << endl;
				ofs.close();
			};
			if(ifs.fail()){
				setDef();
				ifs.close();
				ifs.open(setting_path);
			}
			size_t pos1;
			char tmp[256], *seek;
			while(std::getline(ifs, buf) != NULL){
				strcpy(tmp, buf.c_str());
				if( (pos1 = buf.find('=')) != string::npos){
					tmp[pos1] = '\0';
					seek = tmp + pos1 + 1;
					if(!strcmp(tmp, "log_path")){
						log_path = seek;
						found = true;
						return ;
					}
				}
			}
			if(!found)
				setDef();
			ifs.close();
		}();
#ifdef __debug__
		output("log_path = ");
		output(log_path);
		output("\n");
#endif
	};

	~CUI(){
		if(ofs.is_open())
			ofs.close();
	};

	void switchBuf(bool useBuffer){
		useBuf = useBuffer;
	};

	void switchFileBuf(bool useFileBuffer, string _path = "output.txt", bool append = true){
		string path = log_path + _path;
		//"/"を"\\"に置換
		size_t sPos;
		while((sPos=path.find("/")) != string::npos)
			path.replace(sPos,1,"\\");
		//フォルダが無ければ作る
		MakeSureDirectoryPathExists(path.c_str());	//Win32API
		useFile = useFileBuffer;
		if(useFile){
			if(ofs.is_open())
				ofs.close();
			if(append)
				ofs.open(path, ios::app);	//app = 追記
			else
				ofs.open(path);
			if(ofs.fail()){
				output("failed to open log file.\n");
			}
		}
	};

	bool fileWriting(){
		return (useFile && ofs.is_open());
	};

	//マニピュレータ
	CUI& operator <<(CUI& (*p)(CUI&)){
		return (*p)(*this);
	};

	//一行入力
	void getline(string &str){
		std::getline(cin, str);
		if(useFile && ofs.is_open())
			ofs << str << endl;
	};

	//メッセージボックス
	CUI& operator ()(string str, string title, UINT uType = MB_OK){
		WCHAR mes[257], head[257];
		auto ctowc = [&](WCHAR t2[], string str){
			char t[257];
			strcpy_s(t,256,str.c_str());
			t[256] = '\0';
			mbstowcs(t2, t, 256);
		};
		ctowc(mes, str);
		ctowc(head, title);
		MessageBox(NULL, mes, head, uType);
		return *this;
	};
	
	//出力
	CUI& operator << (short num){
		char tmp[32]={0,};
		Math<short>().itoa(num,tmp);
		output(tmp);
		return *this;
	};

	CUI& operator << (int num){
		char tmp[32]={0,};
		Math<int>().itoa(num,tmp);
		output(tmp);
		return *this;
	};

	CUI& operator << (long num){
		char tmp[32]={0,};
		Math<long>().itoa(num,tmp);
		output(tmp);
		return *this;
	};

	CUI& operator << (long long num){
		char tmp[32]={0,};
		Math<long long>().itoa(num,tmp);
		output(tmp);
		return *this;
	};

	CUI& operator << (unsigned short num){
		char tmp[32]={0,};
		Math<unsigned short>().itoa(num,tmp);
		output(tmp);
		return *this;
	};

	CUI& operator << (unsigned int num){
		char tmp[32]={0,};
		Math<unsigned int>().itoa(num,tmp);
		output(tmp);
		return *this;
	};

	CUI& operator << (unsigned long num){
		char tmp[32]={0,};
		Math<unsigned long>().itoa(num,tmp);
		output(tmp);
		return *this;
	};

	CUI& operator << (unsigned long long num){
		char tmp[32]={0,};
		Math<unsigned long long>().itoa(num,tmp);
		output(tmp);
		return *this;
	};

	CUI& operator << (char s){
		char tmp[2] = {s, '\0'};
		output(tmp);
		return *this;
	};

	
	CUI& operator << (string s){
		output(s);
		return *this;
	};
	
	CUI& operator << (char s[]){
		output(s);
		return *this;
	};

	//入力
	CUI& operator >> (short &num){
		num = input<short>();
		return *this;
	};

	CUI& operator >> (int &num){
		num = input<int>();
		return *this;
	};

	CUI& operator >> (long &num){
		num = input<long>();
		return *this;
	};

	CUI& operator >> (long long &num){
		num = input<long long>();
		return *this;
	};

	CUI& operator >> (unsigned short &num){
		num = input<unsigned short>();
		return *this;
	};

	CUI& operator >> (unsigned int &num){
		num = input<unsigned int>();
		return *this;
	};

	CUI& operator >> (unsigned long &num){
		num = input<unsigned long>();
		return *this;
	};

	CUI& operator >> (unsigned long long &num){
		num = input<unsigned long long>();
		return *this;
	};

	CUI& operator >> (char &s){
		s = input<char>();
		return *this;
	};

	CUI& operator >> (string &s){
		s = input<string>();
		return *this;
	};
	
	CUI& operator >> (char s[]){
		s = input<char*>();
		return *this;
	};

	
};

CUI& endl(CUI& manip){
	manip.output("\n");
	return manip;
}

CUI& cls(CUI& manip){
	system("cls");
	return manip;
}

CUI& clear(CUI& manip){
	manip.str = "";
	return manip;
}

CUI& show(CUI& manip){
	manip.output(manip.str);
	return manip;
}

CUI& bs(CUI& manip){
	manip.output("\b");
	manip.output(" ");
	manip.output("\b");
	return manip;
}

CUI& beep(CUI& manip){
	putchar('\a');
	return manip;
}

//インスタンス生成
CUI cui;
//ここまでインスタンスの条件式

//voidポインタをstringに変換して表示
int show_string_p(void *_s){
	vtor(string, s);
	cui << s;
	return RETURN_SUCCESS;
}

int show_table_p(void *_i){
	vtor(TABLE0, i);
	if(i.a){
		string s;
		char buf[10] = {0,};
		for(int n=0;n<i.height;n++){
			for(int t=0;t<i.width;t++){
				////ここの型はTABLE0依存
				Math<int>().itoa(i.table[n*i.width+t], buf);
				s = buf;
				for(size_t n=3; n>s.length(); n--)
					cui << " ";
				cui << s;
				//printf("%3d ", i.table[n*i.width+t]);
			}
			cui << endl;
		}	
		return RETURN_SUCCESS;
	}
	return RETURN_FAILED;
}

#endif

#endif;
