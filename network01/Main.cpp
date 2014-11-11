//Network Dynamic Link Library depending on Windows API
//#pragma comment(linker,"/export:connect=_connect4")
//_declspec(dllexport) 
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

extern "C" int __stdcall net_Win_basic(char destination[32], int dstSocket, sockaddr_in dstAddr){
	return 1;
};
