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
		Spawn = 0,		//“G¢Š«.
		Move,			//“GˆÚ“®.
		ChangeAttack,	//ˆÚ“®‚©‚çUŒ‚ƒ‚[ƒh‚Ö‚Ì•ÏX.
		InitAttack,		//UŒ‚‚²‚Æ‚Ì‰Šú‰».
		Attak,			//UŒ‚ˆ—.
		IdleAttak,		//UŒ‚‚ÌƒCƒ“ƒ^[ƒoƒ‹.
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

	void Damage();		//UŒ‚‚ğ“–‚Ä‚ç‚ê‚½‚Ìˆ—.
	void ExpRender();	//”š”­‚Ì•\¦ˆ—.

	bool m_bAttakFlg;

	//“G‚Ì”š”j—p.
	vector<unique_ptr<clsExplosion>> m_vsmpExplosion;

private:

	void Move();

	void ChangeAttack();

	void InitAttack();

	void Attack();

	void IdleAttack();

	State m_EnemyState;

	float m_fLen;//’·‚³.

	bool m_bEnableFlg;//true‚ª‚±‚Ìƒ‚ƒfƒ‹‚ğg—p‚µ‚Ä‚¢‚é.

	int  m_iAttakInterval;//UŒ‚‚ÌŠÔŠu.

	D3DXVECTOR3 m_vSpawnPos;	//“G‚ğo‚·ˆ×‚ÌŒ³‚ÌˆÊ’u.
};

#endif  //#ifdef _C_ENEMY_H_