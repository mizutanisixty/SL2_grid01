#ifndef __grid_tree_data_format__
#define __grid_tree_data_format__
#include "includer.h"
#include "map.h"

#ifdef __tree_data_format_ver_01__
#define readOnly	0x01
#define isTemp		0x02
#define isSystem	0x04
class treeData{
private:
	bool folder;
	long attrib;
	//1 :	readOnly
	//2	:	isTemp
	//4	:	isSystem
	treeData *dir;
	dynamic<char> data;		//�����̐��䕔�����ォ����
public:
	treeData(){
		attrib = 0;
		folder = false;
	};
	treeData(bool isFolder, long attr){
		attrib = attr;
		folder = isFolder;
		if(folder)
			dir = new Tree<treeData>;
	};
	//�����ǉ�
	int addAttrib(long attr){
		if(attr & isFolder)
#ifdef __debug__
			cout << "(!) attribute \"isFolder\" can't be changed.\n",
#endif
			return 0;
		attrib |= attr;
		return 1;
	};
	//�����폜
	int delAttrib(long attr){
		if(attr & isFolder)
#ifdef __debug__
			cout << "(!) attribute \"isFolder\" can't be changed.\n",
#endif
			return 0;
		attrib &= ~attr;
		return 1;
	};
	//�����ݒ�
	setAttrib(long attr){
		attrib = attr;
	};

	~treeData(){
		if(folder)
			delete dir;
	};
	
}
#endif

#endif
