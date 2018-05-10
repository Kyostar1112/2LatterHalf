#include"Enemy.h"

const float PI = 3.141592f;
namespace ConstantEnemy
{
	const float fSpawnLength = 30.0f;	//敵が出てくる距離.
	const int   iExpImg = 3;			//爆発画像の数.
	const int	iAttakInterval = 180.0f;//攻撃間隔.
}

namespace CE = ConstantEnemy;

clsEnemy::clsEnemy()
{
	m_vsmpExplosion.reserve(CE::iExpImg);
	for (size_t i = 0; i < CE::iExpImg; i++)
	{
		m_vsmpExplosion.push_back(make_unique<clsExplosion>());
		m_vsmpExplosion[i]->Create();
	}
}

clsEnemy::~clsEnemy()
{
}

void clsEnemy::Loop()
{
	//アニメーション速度加算は先に.
	m_dAnimTime += m_dAnimSpeed;

	switch (m_EnemyState)
	{
	case clsEnemy::State::Move:
		Move();
		break;
	case clsEnemy::State::ChangeAttack:
		ChangeAttack();
		break;
	case clsEnemy::State::InitAttack:
		InitAttack();
		break;
	case clsEnemy::State::Attak:
		Attack();
		break;
	case clsEnemy::State::IdleAttak:
		IdleAttack();
		break;
	default:
		break;
	}
	UpdatePos();
}

void clsEnemy::Spawn()
{
	ChangeAnimSet(02);
	m_iAttakInterval = 180.0f;
	float fZ, fX;

	float fDeg = RandamAmong(0.0f, 360.0f);//角度.

	m_fLen = CE::fSpawnLength;//長さ.

	float rad = fDeg * PI / 180.0f;

	fX = m_fLen*cosf(rad);
	fZ = m_fLen*sinf(rad);

	m_dAnimSpeed = 0.01f;
	m_iAnimNo = 0;

	D3DXVECTOR3 vTmp = m_vSpawnPos;

	vTmp.x += fX;
	vTmp.z += fZ;
	SetPosition(vTmp);

	m_bEnableFlg = true;

	//移動状態に変更.
	m_EnemyState = State::Move;
}

void clsEnemy::Move()
{
	D3DXVECTOR3 vTargtPos = m_vSpawnPos;
	D3DXVECTOR3 vTmpPos = { 0.0f, 0.0f, 0.0f };	//ターゲットと自分の変化量.
	float TargetRot = 0.0f;
	vTmpPos = vTargtPos - GetPosition();
	//長さに変換する.
	float Length = D3DXVec3Length(&vTmpPos);

	if (Length > 5.0f)
	{
		TargetRot = atan2f(vTmpPos.x, vTmpPos.z);	//ターゲットへの角度.

		m_enDir = enDirection_Foward;

		SetRotationY(TargetRot);
	}
	else
	{
		//攻撃準備状態に変更.
		ChangeAnimSet(03);
		m_dAnimTime = 0.0f;
		m_EnemyState = State::ChangeAttack;
	}
}

void clsEnemy::ChangeAttack()
{
	//待機状態へ.
	if ( m_dAnimTime + m_dAnimSpeed >= GetAnimPeriod(03))
	{
		//攻撃待機状態に変更.
		m_EnemyState = State::InitAttack;
		m_dAnimSpeed = 0.0f;
	}
}

void clsEnemy::InitAttack()
{
	//アニメーション切り替え.
	ChangeAnimSet(05);
	m_dAnimTime = 0;
	m_dAnimSpeed = 0.01f;
	m_iAttakInterval = CE::iAttakInterval;
	//攻撃状態に変更.
	m_EnemyState = State::Attak;
}

void clsEnemy::Attack()
{
	//攻撃のモーション中か？.
	if ( m_dAnimTime + m_dAnimSpeed >= GetAnimPeriod(05) )
	{
		m_bAttakFlg = true;
		ChangeAnimSet(06);
		m_dAnimTime = 0;
		//攻撃待機状態に変更.
		m_EnemyState = State::IdleAttak;
	}
}

void clsEnemy::IdleAttack()
{
	//攻撃のモーション中か？.
	if (m_dAnimTime + m_dAnimSpeed >= GetAnimPeriod(06))
	{
		m_dAnimSpeed = 0.0f;
		ChangeAnimSet(00);
	}

	if (m_iAttakInterval < 0)
	{
		//攻撃待機状態に変更.
		m_EnemyState = State::InitAttack;
	}
	m_iAttakInterval--;
	m_dAnimSpeed = 0.01f;
}

void clsEnemy::Damage()
{
	for (size_t i = 0; i < m_vsmpExplosion.size(); i++)
	{
		if (!m_vsmpExplosion[i]->m_bDispFlg)
		{
			m_vsmpExplosion[i]->Fire(m_vPos);
			break;
		}
	}
}

void clsEnemy::ExpRender()	//爆発の表示処理.
{
	for (size_t i = 0; i < m_vsmpExplosion.size(); i++)
	{
		if (m_vsmpExplosion[i]->m_bDispFlg)
		{
			m_vsmpExplosion[i]->Render();
		}
	}
}

