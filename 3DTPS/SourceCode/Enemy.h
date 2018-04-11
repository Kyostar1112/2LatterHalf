#ifndef _C_ENEMY_H_
#define _C_ENEMY_H_

#include"CharacterSkin.h"
#include<math.h>
#include <random>
#include "Explosin.h"

class clsEnemy
	:public clsCharacterSkin
{
public:
	clsEnemy();
	~clsEnemy();

	bool GetEnableFlg()
	{
		return m_bEnableFlg;
	}

	void SetEnableFlg(bool EnableFlg)
	{
		m_bEnableFlg = EnableFlg;
	}
	void InitEnemy(bool EnableFlg, D3DXVECTOR3 SpawnPos)
	{
		m_bEnableFlg = EnableFlg;
		m_vSpawnPos = SpawnPos;
		m_fSpd = 0.5f;
	}
	void EnemySpawn();

	void EnemyMove();

	void ShotHit();		//�U���𓖂Ă�ꂽ���̏���.
	void ExpRender();	//�����̕\������.

	bool m_bAttakFlg;
	//�G�̔��j�p.
	vector<unique_ptr<clsExplosion>> m_vsmpExplosion;

private:
	float m_fLen;//����.

	bool m_bEnableFlg;//true���g�p���Ă���.

	bool m_bArrival;
	int  m_iAttakInterval;

	D3DXVECTOR3 m_vSpawnPos;	//�G���o���ׂ̌��̈ʒu.
};

#endif  //#ifdef _C_ENEMY_H_