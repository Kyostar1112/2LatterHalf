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

	void ShotHit();		//攻撃を当てられた時の処理.
	void ExpRender();	//爆発の表示処理.

	bool m_bAttakFlg;
	//敵の爆破用.
	vector<unique_ptr<clsExplosion>> m_vsmpExplosion;

private:
	float m_fLen;//長さ.

	bool m_bEnableFlg;//trueが使用している.

	bool m_bArrival;
	int  m_iAttakInterval;

	D3DXVECTOR3 m_vSpawnPos;	//敵を出す為の元の位置.
};

#endif  //#ifdef _C_ENEMY_H_