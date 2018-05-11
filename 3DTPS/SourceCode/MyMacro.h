#ifndef _MY_MACRO_H_
#define _MY_MACRO_H_
//インクルード.
#include <Windows.h>
#include <random>

//スフィア構造体.
struct SPHERE
{
	D3DXVECTOR3 vCenter;	//中心.
	float		fRadius;	//半径.
};
//方向列挙体.
enum enDirection
{
	enDirection_Stop = 0,	//停止.
	enDirection_Foward,		//前進.
	enDirection_Backward,	//後退.
	enDirection_LeftTurn,	//左回転.
	enDirection_RightTurn,	//右回転.
};

//================================================
//
//	マクロ.
//
//================================================
//エラーメッセージ.
#define ERR_MSG(str,title)	{ MessageBox(NULL,str,title,MB_OK); }

//解放.
#define SAFE_RELEASE(x)		if(x){(x)->Release();(x)=NULL;}
#define SAFE_DELETE(p)		{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p);   (p)=NULL; } }

//ランダム.
template <typename T>
inline T Randam()
{
	random_device rd;

	mt19937 mt(rd());

	uniform_int_distribution<int> rdDeg();

	return static_cast<T>(rdDeg(mt));
}

template <typename T>
inline T RandamAmong(T x, T y)
{
	random_device rd;

	mt19937 mt(rd());

	uniform_int_distribution<int> rdDeg(static_cast<int>(x), static_cast<int>(y));

	return static_cast<T>(rdDeg(mt));
}

#endif//#ifndef _MY_MACRO_H_