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
		//’†S‚Æ”¼Œa‚ğ\‘¢‘Ì‚Éİ’è.
		SetPosition(vPos);
		SetPositionY(1.0f);
		SetScale(0.5f);
	}
	SPHERE m_stSphere;
private:
	//½Ì¨±ì¬.
};

#endif //_C_SPHERE_H_
