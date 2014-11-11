#ifndef __grid_flag__
#define __grid_flag__

//#define __debug__	//�f�o�b�O�̎���`
#define __debug_calling__	//�Ăяo���֘A�̃f�o�b�O���ɒ�`
#define __debug_searching__	//�����֘A�̃f�o�b�O���ɒ�`
//#define __debug_pointing__	//��ʍ��W�擾���[�h���ɒ�`
//#define __auto_input__

#define setting_path "../setting.ini"	//�ݒ�t�@�C���̃p�X

#define macro_continue 10	//�}�N���̏z��`�h�~


#define RETURN_SUCCESS	1
#define RETURN_FAILED	0

#define QUEUE_EXIT 1
#define QUEUE_CONTINUE 2


#ifdef __debug_mem_leak__
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define calloc(c, s) _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define realloc(p, s) _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _recalloc(p, c, s) _recalloc_dbg(p, c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _expand(p, s) _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)

#define memLeakChkInit _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#endif

#ifdef __debug__
#define show_value(x) std::cout<< # x << " = " << (x) << endl
#else
#define show_value(x) NULL
#endif


#define void_to_ref(type, source, dest) type &dest = *(type*)source;
//if(!_target) -> return FAIL
#define vtor(type, target) if(!##_##target)return RETURN_FAILED;type &target = *(type*)##_##target

#define ma(type, size) (type*)malloc(sizeof(type)*(size))

#endif
