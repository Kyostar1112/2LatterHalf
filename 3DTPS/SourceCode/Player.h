#ifndef _C_PLAYER_H_
#define _C_PLAYER_H_

#include"CharacterSkin.h"

class clsPlayer
	:public clsCharacterSkin
{
public:
	clsPlayer();
	~clsPlayer();

	void Render(D3DXMATRIX & mView, D3DXMATRIX & mProj, D3DXVECTOR3 & vLight, D3DXVECTOR3 & vEye);

	double m_dAnimNum;

private:
};

#endif  //#ifdef _C_PLAYER_H_