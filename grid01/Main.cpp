#define GLOBAL_INSTANCE 
#include "gloval.h"
#include "map.h"
#include "cui.h"
#include "callProc.h"
#include "innerCommand.h"

#define fatalException(reason) exit(failed(reason))
#define Exception(reason) {error(reason); queue|=QUEUE_CONTINUE; return RETURN_FAILED;}

#define maf(type) finalData = ma(void*,1)


int failed(const char* str){
	WORD s = grep_strong_2;
	cui << "\n\n";
	grep(debug_message_line_fatal,"*",s);
	cui << "\n\n\t\t\t\t";
	grep("STOP", "STOP", s);
	cui << "\n\tReason : " << str << "\n\n";
	grep(debug_message_line_fatal,"*",s);
	cui << "\n";
	return -1;
}

int error(const char* str){
	cui << "\n";
	grep(debug_message_line, "-");
	cui << "\n\t\t\t\t";
	grep("STOP", "STOP");
	cui << "\n\tReason : " << str << "\n";
	grep(debug_message_line,"-");
	cui << "\n";
	return -1;
}

int main_routine(long &queue, int argc, char *argv[]){
	debug_message_called("main routine");
	using std::cout;
#ifdef __debug__
	cui << "from location : " << argv[0] << "\n\n";
#endif
	STRLIST unsolved;

	char buf[3][256];
	string command;
	//cui << "debug : comment = tsunami_hazard_map\n\n";
	//cui << debug_message_line << "\n\ncommand :";
	cui << "\nsearchSystem - command > ";
#ifndef __auto_input__
	cui.getline(command);
	//第０段階：マクロ処理
	{
		bool macro_replaced = false;
		string com_backup = command;
		int macro_cnt = 0;
		while(command_macro.a(command)){
			macro_replaced = true;
			if(macro_cnt > macro_continue){
				string err = "solving macro lasted more than ";
				char tmp[256] = {0,};
				Math<int>().itoa(macro_continue, tmp);
				err += tmp;
				err += " \n";
				Exception(err.c_str());
			}
			command = *command_macro[command];
			macro_cnt++;
		}
		if(macro_replaced)
			cui << "macro : " << com_backup << " -> " << command << "\n\n";
	}
#else
	{
		cui << "tsunami_hazard_map\n";
		strcpy(buf[0], "tsunami_hazard_map");
	}
#endif
	//マクロ処理が終わってからコピー
	strcpy(buf[0], command.c_str());
	//cui << "\n" << debug_message_line << "\n";
	//第１段階：コマンド処理(キュー関連)
	{
		if(command == ""){
			queue |= QUEUE_CONTINUE;
			return RETURN_FAILED;
		}
		if(command == "exit"){
			queue |= QUEUE_EXIT;
			return RETURN_SUCCESS;
		}
	}
	//第２段階：コマンド処理
	{
		if(command.find(' ') == string::npos){
			if(command_func.a(command)){
				command_func[command](NULL);
				return RETURN_SUCCESS;
			}
		}else{
			char tmpCmd[256];
			size_t sPos = command.find(' ');
			strcpy(tmpCmd, command.c_str());
			tmpCmd[sPos] = '\0';
			if(command_func.a(tmpCmd)){
				return command_func[tmpCmd](tmpCmd+sPos+1);
			}
		}
	}
	//第３段階：検索
	bool is_first = true, dl_a = false;
	void *finalData;
	int (*showFunc)(void*);
	int dataCnt = 0;
	string type;
	callProc<int, char*, char*, char*, void*> way1;
	way1.set(rtFunc[0].p,rtFunc[0].f);
//	if(way1(buf[0], buf[1], buf[2], buf[3]) == RETURN_FAILED)
//		fatalException("the way to solve this problem not exist.");
//	debug_message_returned("main");
//	/*if(!tmp1)
//		cui << "(!) error occured at searching way.\n";*/
//#ifdef __debug__
//	cui << ">>\t返り値：\n";
//	cui << "\t> type = " << buf[1] << "\n\t> way = " << buf[2] << endl;
//	cui << command << "\n\n" << debug_message_line << "\n";
//#endif
//	//置換
//	commandReplace(command, buf[0], buf[2]);
	//ループ(テスト)
	while((unsolved=unProcessed(command)).a){
		for(size_t i=0;i<unsolved.num;i++){
			strcpy(buf[0], unsolved.s[i].c_str());
			if(way1(buf[0], buf[1], buf[2], NULL) == RETURN_FAILED){
				char tmp[256];
				sprintf(tmp, "the way to solve \"%s\" not found.", command.c_str());
				Exception(tmp);
			}
			if(is_first){
				type = buf[1];
				is_first = false;
			}
			commandReplace(command, buf[0], buf[2]);
			STRLIST tmp2 = processed(command);
#ifdef __debug_searching__
			cui << "\n\n\n";
			if(tmp2.a)
				for(size_t i=0;i<tmp2.num;i++)
					cui << tmp2.s[i] << "\n";
#endif
			debug_message_returned("main routine");
		}
	}
	//コマンド直接入力時の対策
	if(type==""){
		STRLIST tmp = cutCommand(command);
		string tc;
		if(tmp.s[tmp.num] != ""){
			tc = tmp.s[tmp.num];
		}else{
			tc = tmp.s[tmp.num-1];
		}
		STRPAIR tmp2 = sepComPath(tc);
		type = tmp2.type;
	}
	//データリストのアロケート
	if(type == "image"){
		maf(IMG0);
		showFunc = mkGUI;
	}else if(type == "table"){
		maf(TABLE0);
		showFunc = show_table_p;
	}else{
		maf(string);
		showFunc = show_string_p;
	}
#ifdef __debug_calling__
	cui << command << "\n\n\n";	//////////
#endif
	//試験的に。
	/*if(command=="[test.contCUI.dll*setCharSize]"){
		TABLE0 i;
		int n = 3;
		callProc<int, void*, void*, void*, void*> cp;
		cp.set(L"test.contCUI.dll", "setCharSize");
		cp((void*)&i, (void*)&n, NULL, NULL);
		return RETURN_SUCCESS;
	}
	*/
	//ここまで試験的に。
	//ここから試験的に。
	{
		STRLIST com_part;
		void *dataList[100] = {NULL, };
		void **dataSeek = dataList;
		bool final_flag = false;
		bool stackOver = false;

		size_t seekPosS = 0, seekPosC = 0, stackCnt = 0, &num = com_part.num;
		string firstControlChar;
		com_part = cutCommand(command);
#ifdef __debug_calling__
		size_t _start = (com_part.c_first ? 1 : 0);
		if(_start == 1)
			cui << "\t" << com_part.c[0] << endl;
		for(size_t i=0;i<com_part.num;i++)
			cui << com_part.s[i] << "\n\t" << com_part.c[i+_start] << endl;
		cui << com_part.s[com_part.num] << endl;
		cui << debug_message_line << endl;
		cui << "\n\nflag1\n\n";	////////////////
#endif
		
		if(com_part.c_first){
			firstControlChar = com_part.c[0];
			seekPosC = 1;
		}

		while(seekPosS <= num){
			//switch(com_part.c[seekPos];
			if(seekPosS==num)
				final_flag = true;

			if(com_part.s[seekPosS] != ""){
				STRPAIR tmp_path = sepComPath(com_part.s[seekPosS]);
				callProc<int, void*, void*, void*, void*> cp;

				WCHAR file_path[256];
				char tmp[256];
				
				strcpy(tmp, tmp_path.file.c_str());
				mbstowcs(file_path, tmp, sizeof(tmp));
				
				cp.set(file_path, tmp_path.func.c_str());

				auto allocating = [&]{
					string &t = tmp_path.type;
					if(t=="image")
						dataList[stackCnt] = ma(IMG0, 1);
					else if(t=="table")
						dataList[stackCnt] = ma(TABLE0, 1);
					else if(t=="string")
						//dataList[stackCnt] = ma(string, 1);	//クラスなので例外
						dataList[stackCnt] = new string;
					show_value(stackCnt);
					return dataList[stackCnt++];
				};

				auto calling = [&]{
					if(stackOver){
						stackOver = false;
						int ret =  cp(dataSeek[0], dataSeek[1], dataSeek[2], allocating());
						dataSeek = dataList + stackCnt - 1;
						return ret;
						
					}else{
						int t = cp(NULL, NULL, NULL, allocating());
						return t;
						//return cp(NULL, NULL, NULL, allocating());
					}
				};

				bool success = false;

				string err;
				if(!cp.a())
					err = "this dll or function not exist";
				else if(calling() == RETURN_FAILED)
					err = "this function returned \"FAILED\"";
				else
					success = true;

				if(!success){
					string s = "an error occured at \"";
					s += tmp_path.func;
					s += "\" in \"";
					s += tmp_path.file;
					s += "\"";
					s += "\n\t\t ";
					s += err;
					Exception(s.c_str());
				}
				if(!final_flag){
#ifdef __debug_calling__
					cui << com_part.c[seekPosC] << endl;
#endif
					if(com_part.c[seekPosC] == ">")
						stackOver = true;
				}
			}
			seekPosS++;
			seekPosC++;
		}
		//show_value(stackCnt);
		//showFunc(dataList[0]);
		if(type != "")
			showFunc(dataList[stackCnt-1]);
		else
			cui("an error occured at showing\ndata type not defined", "fatal error", MB_OK | MB_ICONERROR);
#ifdef __debug_calling__
		cui << "\n\nflag2\n\n";
#endif
	}
	//ここまで試験的に。
	//ここから下は要らなくなったはずだが、今後のために一応残しておく
	/*
	//wayの分解
	//現時点ではテストのため、最初の１つだけを読む。
	string way = command;
	size_t pos1, pos2;
	char seek[2][256];
	if(way.find('+') == string::npos)
		way += "+";
	if( (pos1 = way.find('*')) != string::npos && (pos2 = way.find('+')) != string::npos){
#ifdef __debug__
		cui << "\n>>\twayの分解...\n";
#endif
		char *seek0, *seek1;
		strcpy(seek[0], way.c_str());
		seek[0][pos1] = seek[0][pos2] = '\0';
		seek[0][pos2-1] = '\0';	//']'の消去
		strcpy(seek[1], seek[0]+(pos1+1));	//'\0'と'['の分
		//調整用
		seek0 = seek[0] +1;
		seek1 = seek[1];
#ifdef __debug__
		cui << "\t> seek[0] = " << seek0 << "\n\t> seek[1] = " << seek1 << "\n";
#endif
		WCHAR file_path[256];
		//setlocale( LC_CTYPE, "jpn" );	//必要か不明なので一応コメントアウト
		mbstowcs(file_path, seek0, sizeof(seek[0]));	//sizeofはseek0ではなくseek[0]
		TABLE0 i;
		callProc<int, TABLE0, int, int, int> cp1;
		cp1.set(file_path,seek1);
		i.height = i.width = 5;
		i.status = 0;
		i.count = 0;
		i.table = (int*)malloc(sizeof(int) * i.width * i.height);
		i.comment="テストデータ";
		i.a = false;
		//呼び出し
		{
			bool success = false;
			string err;
			if(!cp1.a())
				err = "this dll or function not exist";
			else if(cp1(i,0,0,0) == RETURN_FAILED)
				err = "this function returned \"FAILED\"";
			else
				success = true;

			if(!success){
				string s = "an error occured at \"";
				s += seek1;
				s += "\" in \"";
				s += seek0;
				s += "\"";
				s += "\n\t\t ";
				s += err;
				Exception(s.c_str());
			}
		}
        show_table_p(&i);
		if(type=="image"){
			void *_img0 = malloc(sizeof(IMG0));
			vtor(IMG0, img0);
			img0.x = 640;
			img0.y = 480;
			img0.data = ma(COLORREF, img0.x * img0.y);
			for(int i=0;i<img0.y;i++)
				for(int n=0;n<img0.x;n++)
					img0.data[i*img0.x+n]= RGB(i, n, 255);
			return showFunc(_img0);
		}
		debug_message_returned("main routine");
	}
	*/
	//ここまでコメントアウト
	return RETURN_SUCCESS;
}

int main(int argc, char *argv[]){
	ios::sync_with_stdio(false);	//cin速度改善
	command_init();
	long queue = 0;
	while(!(queue&QUEUE_EXIT)){
		debug_message_called("main");
		main_routine(queue, argc, argv);
		if(queue&QUEUE_CONTINUE){
			queue &= ~QUEUE_CONTINUE;
			continue;
		}
	}
}
