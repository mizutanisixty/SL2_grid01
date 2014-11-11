#ifndef __grid_global__
#define __grid_global__
#include "includer.h"

#ifdef GLOBAL_INSTANCE
#define GLOBAL
#else
#define GLOBAL extern 
#endif

#include "includer.h"
#include "data.h"
#include "func.h"
#include "dynamic.h"
#include "map.h"
#include "tree.h"
#include "Math.h"

RT_FUNC_P rtFunc[] = {
	{L"test.provKnowledge01.dll", "provWay"}
};	//óvëfêîÇÕ _countof Ç≈éÊìæâ¬î\

IMG0 contGUI;
bool contGUI_a = false;

/*
GLOBAL errCode E_nInit("root pointer is not initialized.",0x0001);
GLOBAL errCode E_overRange("searching position is out of range.",0x0002);
GLOBAL errCode E_illegalMemAccess("accessed to illegal range of memory adress.",0x0004);
*/
#endif
