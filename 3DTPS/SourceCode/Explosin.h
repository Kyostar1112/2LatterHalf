#ifndef _C_EXPLOSIN_H_
#define _C_EXPLOSIN_H_

#include"Sprite.h"

class clsExplosion
	:public clsSprite
{
public:
	clsExplosion();
	~clsExplosion();
	void Create();
	void Fire(D3DXVECTOR3 Pos);

private:
};

#endif //_C_EXPLOSIN_H_
