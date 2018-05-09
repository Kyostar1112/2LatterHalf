#ifndef _C_SPHERE_H_
#define _C_SPHERE_H_

#include"Character.h"

class clsSphere
	:public clsCharacter
{
public:
	clsSphere();
	~clsSphere();
	void InitSphere(D3DXVECTOR3 vPos, float fScale)
	{
		//中心と半径を構造体に設定.
		SetPosition(vPos);
		SetPositionY(1.0f);
		SetScale(0.5f);
	}
	SPHERE m_stSphere;
private:
	//スフィア作成.
};

#endif //_C_SPHERE_H_
