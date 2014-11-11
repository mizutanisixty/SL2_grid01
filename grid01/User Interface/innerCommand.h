#ifndef __grid_inner_command__
#define __grid_inner_command__
#include "includer.h"
#include "gloval.h"
#include "gui.h"

using namespace std;
typedef int (*command_type_pointer)(char[]);

map<command_type_pointer> command_func;
dynamic<string*> command_help;
map<string*> command_macro;
dynamic<string*> macro_help;

bool setting_a;

int _inner_command_cls(char trush[] = NULL){
	system("cls");
	return RETURN_SUCCESS;
}

int _inner_command_color(char cl[]){
	char tmp[9];
	strcpy(tmp, "color 07");
	if(cl){
		tmp[6] = cl[0];
		tmp[7] = cl[1];
	}
	system(tmp);
	return RETURN_SUCCESS;
}

int _inner_command_help(char _param[]){
	if(!_param){
		cui << "this command shows list of usable command.\n";
		cui << "help -[0-3]\t:\tcommand class [0-3]\n";
		cui << "help -a\t\t:\tall usable command\n";
		return RETURN_SUCCESS;
	}
	string param = _param;
	bool show[4] = {false,};
	if(param == "-a")
		show[0] = show[1] = show[2] = show[3] = true;
	else if(param == "-0")
		show[0] = true;
	else if(param == "-1")
		show[1] = true;
	else if(param == "-2")
		show[2] = true;
	else if(param == "-3")
		show[3] = true;
	//Class 0：マクロ処理
	if(show[0]){
		grep("\tClass 0\n", "Class 0", grep_strong_2);
		dynamic<string*> &mh = macro_help;
		long cnt = 0;
		mh.setPos(0);
		while(mh.a() | aThis && !mh.getError().chk(E_overRange)){
			if(*mh.c() != "")
				cui << "[" << cnt++ << "]\t" << *mh.c() << " = " << *command_macro[*mh.c()] << endl;
			mh++;
		}
		cui << endl;
	}
	//Class 1：コマンド処理(キュー関連)
	if(show[1]){
		grep("\tClass 1\n", "Class 1", grep_strong_2);
		cui << "[0]\texit" << endl;
		cui << endl;
	}
	//Class 2：コマンド処理
	if(show[2]){
		grep("\tClass 2\n", "Class 2", grep_strong_2);
		//dynamicクラスのseek機能がうまく動くか確認。
		dynamic<string*> &ch = command_help;
		ch.setPos(0);
		while(ch.a() | aThis && !ch.getError().chk(E_overRange)){
			cui << "[" << (long)ch << "]\t" <<  *ch.c() << endl;
			ch++;
		}
		cui << endl;
	}
	//Class 3：検索
	if(show[3]){
		grep("\tClass 3\n", "Class 3", grep_strong_2);
		string buf, data_path("../test.provWayData01.txt");
		long cnt=0;
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
			while(std::getline(ifs, buf) != NULL){
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
		//ハンドル生成
		std::ifstream ifs(data_path);
		if(ifs.fail()){
			cui << "failed to read database file.\n";
		}else{
			while(getline(ifs,buf)){
				size_t pos;
				char tmp[256];
				if((pos=buf.find(","))!=string::npos){
					strcpy(tmp, buf.c_str());
					tmp[pos] = '\0';
					cui << "[" << cnt++ << "]\t" << tmp << endl;
				}
			}
		}
	}
	return RETURN_SUCCESS;
}

int _inner_command_echoFile(char path[]){
	if(path){
		string tmp = string("../") + path;
		ifstream ifs(tmp);
		if(!ifs.fail()){
			string buf;
			while(getline(ifs, buf) != NULL){
				if(_kbhit()){
				char k = _getch();	//特殊キーなら0xE0を返すはず
				if(k == 0xE0)
					k = _getch();
				
					if(k == VK_ESCAPE){
						cui << "\n[Esc] aborted\n";
						return RETURN_SUCCESS;
					}
				}
				cui << buf << endl;
			}
			return RETURN_SUCCESS;
		}
	}
	cui << "file not exist\n";
	return RETURN_FAILED;
}

int _inner_command_log(char trush[] = NULL){
	cui << "log file " << (cui.fileWriting() ? "enabled" : "disabled") << endl;
	cui << "use \"togglelog\" for changing\n";
	return RETURN_SUCCESS;
}

int _inner_command_toggleLog(char path[]){
	string tmp, fileName;
	bool append = true;
	if(!cui.fileWriting()){
		if(path){
			fileName = path;
		}else{
			cui << "input file name ( default : output.txt )\t> ";
			cui.getline(fileName);
			if(fileName == "")
				fileName = "output.txt";
		}
		cui << "[A] append\t[O] overwrite\n";
		cui >> tmp;
		if(tmp[0] == 'o' || tmp[0] == 'O' || tmp[0] == '0')
			append = false;
	}
	cui.switchFileBuf(!cui.fileWriting(), fileName, append);
	cui << "file logging " <<  (cui.fileWriting()?"enabled":"disbled") << ".\n";
	return RETURN_SUCCESS;
}

int _inner_command_testOutput(char trush[] = NULL){
	cui << "test output text\n";
	return RETURN_SUCCESS;
}

int setMacro(string,string);	//しょうがないのでプロトタイプ宣言
int _inner_command_makeMacro(char param[]){
	string tmp;
	size_t pos;
	if(param)
		tmp = param;
	else
		cui << "コマンドを入力 ( a=b 形式 )\t> ">>tmp;	//CUIクラスだからできるひねり技
	if((pos=tmp.find("="))!=string::npos){
		char tmp_c[256]={0,}, *seek;
		strcpy(tmp_c, tmp.c_str());
		tmp_c[pos] = '\0';
		seek = tmp_c + pos + 1;
		setMacro(tmp_c, seek);
		cui << "set \"" << tmp_c << "\" to \"" << seek << "\"\n";
		return RETURN_SUCCESS;
	}
	cui << "error\n";
	return RETURN_FAILED;
}

int _inner_command_deleteMacro(char param[]){
	if(param){
		if(command_macro.a(param))
			delete command_macro[param];
		if(command_macro(param)){
			dynamic<string*> &mh = macro_help;
			mh.setPos(0);
			while(mh.a() | aThis && !mh.getError().chk(E_overRange)){
				if(*mh.c() == param)
					*mh.c() = "";
				mh++;
			}
			cui << "accepted\n";
			return RETURN_SUCCESS;
		}
	}
	cui << "error\n";
	return RETURN_FAILED;
}

int setMacro(string name, string func){
	command_macro[name] = new string(func);
	macro_help.add(1);
	macro_help[macro_help.getSize()-1] = new string(name);
	return RETURN_SUCCESS;
}

int command_init(){
	int (*sm)(string,string)  = setMacro;
	sm("scs", "setCharSize");
	sm("thm", "tsunami_hazard_map");
	sm("thm2", "tsunami_hazard_map+getTsunamiData>imgProc");
	sm("thm2_i", "[image?test.procImg01.dll*getMap]+[table?test.provKnowledge01.dll*getTsunamiData]>[image?test.procImg01.dll*procMap]+[table?test.provKnowledge01.dll*getTsunamiData]>[image?test.procImg01.dll*procMap]");
	sm("gibp", "e_gibp");
	auto add = [&](string name, command_type_pointer func){
		command_func[name] = func;
		command_help.add(1);
		command_help[command_help.getSize()-1] = new string(name);
	};
	add("cls", _inner_command_cls);
	add("color", _inner_command_color);
	add("log", _inner_command_log);
	add("togglelog", _inner_command_toggleLog);
	add("testoutput", _inner_command_testOutput);
	add("guitest", GUItest);
    add("echofile", _inner_command_echoFile);
	add("setmacro", _inner_command_makeMacro);
	add("delmacro", _inner_command_deleteMacro);
	add("help", _inner_command_help);
	return RETURN_SUCCESS;
}

#endif
