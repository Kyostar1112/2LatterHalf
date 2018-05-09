#ifndef _MY_MACRO_H_
#define _MY_MACRO_H_
//�ݸٰ��.
#include <Windows.h>
#include <random>

//�̨��\����.
struct SPHERE
{
	D3DXVECTOR3 vCenter;	//���S.
	float		fRadius;	//���a.
};
//�����񋓑�.
enum enDirection
{
	enDirection_Stop = 0,	//��~.
	enDirection_Foward,		//�O�i.
	enDirection_Backward,	//���.
	enDirection_LeftTurn,	//����].
	enDirection_RightTurn,	//�E��].
};

//================================================
//
//	ϸ�.
//
//================================================
//�װү����.
#define ERR_MSG(str,title)	{ MessageBox(NULL,str,title,MB_OK); }

//���.
#define SAFE_RELEASE(x)		if(x){(x)->Release();(x)=NULL;}
#define SAFE_DELETE(p)		{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p);   (p)=NULL; } }

//�����_��.
template <typename T>
inline T Randam()
{
	random_device rd;

	mt19937 mt(rd());

	uniform_int_distribution<int> rdDeg();

	return rdDeg(mt);
}

template <typename T>
inline T RandamAmong(T x, T y )
{
	random_device rd;

	mt19937 mt(rd());

	uniform_int_distribution<int> rdDeg(x, y);

	return rdDeg(mt);
}

#endif//#ifndef _MY_MACRO_H_