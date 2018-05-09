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

	enum class State
	{
		Spawn = 0,		//敵召喚.
		Move,			//敵移動.
		ChangeAttack,	//移動から攻撃モードへの変更.
		InitAttack,		//攻撃ごとの初期化.
		Attak,			//攻撃処理.
		IdleAttak,		//攻撃のインターバル.
	};

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
	void Spawn();

	void Loop();

	void Damage();		//攻撃を当てられた時の処理.
	void ExpRender();	//爆発の表示処理.

	bool m_bAttakFlg;

	//敵の爆破用.
	vector<unique_ptr<clsExplosion>> m_vsmpExplosion;

private:

	void Move();

	void ChangeAttack();

	void InitAttack();

	void Attack();

	void IdleAttack();

	State m_EnemyState;

	float m_fLen;//長さ.

	bool m_bEnableFlg;//trueがこのモデルを使用している.

	int  m_iAttakInterval;//攻撃の間隔.

	D3DXVECTOR3 m_vSpawnPos;	//敵を出す為の元の位置.
};

#endif  //#ifdef _C_ENEMY_H_