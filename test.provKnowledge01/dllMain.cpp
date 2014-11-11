#include <Windows.h>
#include <iostream>
#include <fileapi.h>
#include <fstream>
#include <string>
#include "../grid01/map.h"
#include "../grid01/includer.h"


BOOL WINAPI DllMain( HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved){
	return TRUE;
}

using namespace std;

#define show(x) # x << " = " << (x)

//return : 0 = 失敗、 1 = 成功
int __stdcall provWay(CUI *_cui, char* str, char* type, char* way, void* trush1){
	CUI &cui = *_cui;
	debug_message_called("test.provKonwledge01 > provWay");
	//FILE *f;
	//char buf[3][256]={{0,},};
	string buf, data_path("../test.provWayData01.txt");
	[&]{
		string buf;
		bool found = false;
		std::ifstream ifs(setting_path);
		auto setDef = [&]{
			std::ofstream ofs(setting_path, ios::app);
			if(ofs.fail())
				return ;
			//"デフォルトの書き込み"
			ofs << "data_path=" << data_path << endl;
			ofs.close();
		};
		if(ifs.fail()){
			setDef();
			ifs.close();
			ifs.open(setting_path);
		}
		size_t pos1;
		char tmp[256], *seek;
		while(std::getline(ifs, buf)!=NULL){
			strcpy(tmp, buf.c_str());
			if( (pos1 = buf.find('=')) != string::npos){
				tmp[pos1] = '\0';
				seek = tmp + pos1 + 1;
				if(!strcmp(tmp, "data_path")){
					data_path = seek;
					found = true;
					return ;
				}
			}
		}
		if(!found)
			setDef();
		ifs.close();
	}();

	//f = fopen("test.provWayData01.txt","r");

	std::ifstream ifs(data_path);
	//if(!f){
	if(ifs.fail()){
#ifdef __debug__
		cui << "fatal exception occured at \"provWay(test.provKnowledge01.dll)\"\n\t> file \"../text.provWayData01.txt\" not exist.\n";
#endif
		sprintf(type, "error");
		sprintf(way, "error");
		return RETURN_FAILED;
	}
	/*fscanf(f,"%s %s %s", buf[0], buf[1], buf[2]);
	if(strcmp(buf[0], "header") * strcmp(buf[1], "data") * strcmp(buf[2], dataType))	//片方が0ならエラー
		return 0;	//header異常
	while(!feof(f)){
		fscanf(f,"%s %s %s", buf[0], buf[1], buf[2]);
		if(!strcmp(buf[0], str)){
			sprintf(type, buf[1]);
			sprintf(way, buf[2]);
			return 1;
		}
	}*/
	size_t pos1, pos2;
	char tmp[256], *seek, *seek2;
	bool flg_seek2 = false;
	while(getline(ifs, buf)){
		if(buf[0] == '/' || buf[1] == '/')
			continue;	//コメントアウト
#ifdef __debug__
		cui << "\n[" << buf << "]";
#endif
		strcpy(tmp, buf.c_str());
		if( (pos1 = buf.find(',')) != string::npos){
			tmp[pos1] = '\0';
			buf[pos1] = '#';
#ifdef __debug__
			cui << "\n\t > " << tmp  << "\t>> ";
#endif
			if(strcmp(str,tmp))
				continue;
#ifdef __debug__
			cui << "OK\n";
#endif
			seek = tmp + pos1 + 1;
			if( (pos2 = buf.find(',')) != string::npos){
				tmp[pos2] = '\0';
				seek2 = tmp + pos2 + 1;
				flg_seek2 = true;
			}else{
				continue;	//エラー
			}
			strcpy(type, seek);
			if(flg_seek2)
				strcpy(way, seek2);
			return RETURN_SUCCESS;
		}else{
			continue;	//エラー
		}
		
	}
	return RETURN_FAILED;
}

//return : 0 = 失敗、 1 = 成功
int __stdcall getTsunamiData(CUI *_cui, void* t1, void *t2, void *t3, void *_ans){
	debug_message_called("test.provKonwledge01 > getTsunamiData");
	vtor(TABLE0, ans);
	vtor(CUI, cui);
	//データ読み込みテスト
	{
		char *funcName = "getTsunamiData";
		char *fileName = "test.provKnowledge01.dll";
		char *dataPath = "../test.provTsunamiData01.txt";
		ans.a = false;
		ans.width = 2;
		size_t tSizeUnit = sizeof(int)*ans.width, tSize = tSizeUnit;
		auto ra = [&]{
			if(!ans.a){
				ans.height = 1;
				ans.a = true;
				return ma(int, ans.width);
			}
			tSize += tSizeUnit;
			ans.height++;
			return (int*)realloc(ans.table, tSize);
		};
		auto ansX = [&]()->int&{
			return ans.table[(tSize/tSizeUnit-1)*2];
		};
		auto ansY = [&]()->int&{
			return ans.table[(tSize/tSizeUnit-1)*2+1];
		};
		string buf;
		ifstream ifs(dataPath);
		char tmp[256];
		size_t pos;
		if(ifs.fail()){
			errorMessage(fileName, funcName) << "\"" <<  dataPath << "\" not exist.\n";
			return RETURN_FAILED;
		}
		while(getline(ifs, buf)){
			if((pos=buf.find(","))!=string::npos){
				strcpy(tmp, buf.c_str());
				tmp[pos] = '\0';
#ifdef __debug__
				cui << "[" << tmp << "]" << endl;
#endif
				if(string(tmp) == "header")
					continue;	//ヘッダなのでデータではない
				if(tmp[0] == '/' || tmp[1] == '/')
					continue;	//コメントアウト
				buf = tmp + pos + 1;
				if((pos=buf.find(","))!=string::npos){
					strcpy(tmp, buf.c_str());
					tmp[pos] = '\0';
					if(!(ans.table = ra())){
						errorMessage(fileName, funcName) << "realloc return NULL\n";
						return RETURN_FAILED;
					}
					Math<int> hMath;
					ansX() = hMath.atoi(tmp);
					ansY() = hMath.atoi(tmp + pos + 1);
#ifdef __debug__
					cui << "(" << ansX() << ", " << ansY() << ")\n";
#endif
				}else{
					continue;	//エラー
				}
			}else{
				continue;	//エラー
			}
		}
	}
#ifdef __desabled__
	//仮のデータ
	ans.width = 2;	//座標なので固定
	ans.height = 10;
	ans.count = 0;
	ans.table = ma(int, ans.width*ans.height);
	for(int n=0;n<ans.height;n++)
		for(int t=0;t<ans.width;t++)
			ans.table[n*ans.width+t] = (++ans.count)*10,\
	ans.a = true;
#endif
	return RETURN_SUCCESS;
}
