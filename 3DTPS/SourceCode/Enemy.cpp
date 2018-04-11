#include"Enemy.h"

const float PI = 3.141592f;
namespace ConstantEnemy
{
	const float SpawnLength = 20.0f;
	const int   ExpImg = 3;//爆発画像の数.
}

clsEnemy::clsEnemy()
{
	for (size_t i = 0; i < ConstantEnemy::ExpImg; i++)
	{
		m_vsmpExplosion.push_back(make_unique<clsExplosion>());
		m_vsmpExplosion[i]->Create();
	}
}

clsEnemy::~clsEnemy()
{
}

void clsEnemy::EnemySpawn()
{
	m_bArrival = false;
	m_bAttakFlg = false;

	ChangeAnimSet(02);
	m_iAttakInterval = 180.0f;
	float fZ, fX;

	random_device rd;

	mt19937 mt(rd());

	uniform_int_distribution<int> rdDeg(0.0f, 360.0f);

	float fDeg = static_cast<float>(rdDeg(mt));//角度.

	m_fLen = ConstantEnemy::SpawnLength;//長さ.

	float rad = fDeg * PI / 180.0;

	fX = m_fLen*cosf(rad);
	fZ = m_fLen*sinf(rad);

	m_dAnimSpeed = 0.01f;
	m_iAnimNo = 0;

	D3DXVECTOR3 vTmp = m_vSpawnPos;

	vTmp.x += fX;
	vTmp.z += fZ;
	SetPosition(vTmp);

	m_bEnableFlg = true;
}

void clsEnemy::EnemyMove()
{
	int AnimNum = 5;

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

		UpdatePos();

#if 0
		if (m_fLen > 5.0f)
		{
			m_fLen -= 0.05f;
		}

#endif // 0

		//SetPositionX(m_fLen*cosf(TargetRot));
		//SetPositionZ(m_fLen*sinf(TargetRot));

		SetRotationY(TargetRot);
	}
	else if (!m_bArrival)
	{
		//アニメーション切り替え.
		m_iAnimNo = AnimNum;
		ChangeAnimSet(m_iAnimNo);
		m_dAnimTime = 0;
		m_bArrival = true;
	}

	m_dAnimTime += m_dAnimSpeed;

	if (m_iAnimNo == AnimNum)
	{
		if (m_dAnimTime >= GetAnimPeriod(AnimNum))
		{
			m_dAnimTime = 0.0f;
			m_bAttakFlg = true;
			m_dAnimSpeed = 0;
			//ChangeAnimSet(0);
			////アニメーション切り替え.
			//m_iAnimNo = 4;
			//ChangeAnimSet(m_iAnimNo);
		}
		if (m_iAttakInterval < 0)
		{
			m_iAttakInterval = 180.0f;
		}

		if (m_iAttakInterval >= 180)
		{
			m_dAnimTime = 0.0f;
			m_dAnimSpeed = 0.01f;
		}
		m_iAttakInterval--;
	}
}

void clsEnemy::ShotHit()
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