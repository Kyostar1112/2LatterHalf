#ifndef _MY_MACRO_H_
#define _MY_MACRO_H_
//²Ý¸Ù°ÄÞ.
#include <Windows.h>

//½Ì¨±\‘¢‘Ì.
struct SPHERE
{
	D3DXVECTOR3 vCenter;	//’†S.
	float		fRadius;	//”¼Œa.
};
//•ûŒü—ñ‹“‘Ì.
enum enDirection
{
	enDirection_Stop = 0,//’âŽ~.
	enDirection_Foward,		//‘Oi.
	enDirection_Backward,	//Œã‘Þ.
	enDirection_LeftTurn,	//¶‰ñ“].
	enDirection_RightTurn,	//‰E‰ñ“].
};

//================================================
//
//	Ï¸Û.
//
//================================================
//´×°Ò¯¾°¼Þ.
#define ERR_MSG(str,title)	{ MessageBox(NULL,str,title,MB_OK); }

//‰ð•ú.
#define SAFE_RELEASE(x)		if(x){(x)->Release();(x)=NULL;}
#define SAFE_DELETE(p)		{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p);   (p)=NULL; } }

#endif//#ifndef _MY_MACRO_H_