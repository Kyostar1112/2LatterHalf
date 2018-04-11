#include"Explosin.h"

clsExplosion::clsExplosion()
{
}

clsExplosion::~clsExplosion()
{
}

void clsExplosion::Create()
{
	Init();
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bDispFlg = false;
}
void clsExplosion::Fire(D3DXVECTOR3 Pos)
{
	Pos.y += 1.0f;
	m_vPos = Pos;
	m_texUV = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	m_bDispFlg = true;
}