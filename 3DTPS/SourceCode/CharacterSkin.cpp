#include "CharacterSkin.h"

//コンストラクタ.
clsCharacterSkin::clsCharacterSkin()
	: m_pSkinModel(NULL)
	, m_pAnimCtrl(NULL)
	, m_dAnimSpeed(0.1f)
{
	m_fSpd = 1.0f;
}
//デストラクタ.
clsCharacterSkin::~clsCharacterSkin()
{
}
//モデルデータ関連付け.
void clsCharacterSkin::AttachModel(clsD3DXSKINMESH* pModel)
{
	if (pModel == nullptr) {
		return;
	}

	//モデルの設定.
	m_pSkinModel = pModel;
	//アニメーション速度.
	m_dAnimSpeed = m_pSkinModel->GetAnimSpeed();
	//スケール設定.
	m_fScale = m_pSkinModel->m_fScale;

	//アニメーションコントローラーのクローン.
	LPD3DXANIMATIONCONTROLLER pAC
		= m_pSkinModel->GetAnimCtrl();

	pAC->CloneAnimationController(
		pAC->GetMaxNumAnimationOutputs(),
		pAC->GetMaxNumAnimationSets(),
		pAC->GetMaxNumTracks(),
		pAC->GetMaxNumEvents(),
		&m_pAnimCtrl);	//(out)アニメーションコントローラー.
}
//モデルデータ関連付け解除.
void clsCharacterSkin::DetachModel()
{
	if (m_pSkinModel != nullptr) {
		m_pSkinModel = NULL;

		if (m_pAnimCtrl != NULL) {
			m_pAnimCtrl->Release();
			m_pAnimCtrl = NULL;
		}
	}
}

//ﾚﾝﾀﾞﾘﾝｸﾞ関数.
void clsCharacterSkin::Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
	D3DXVECTOR3& vLight, D3DXVECTOR3& vEye)
{
	if (m_pSkinModel == nullptr && m_pAnimCtrl == nullptr) {
		return;
	}

	Update();

	m_pSkinModel->Render(
		mView, mProj, vLight, vEye,
		m_pAnimCtrl);
}

//座標や回転の更新.
void clsCharacterSkin::Update()
{
	if (m_pSkinModel == nullptr) {
		return;
	}
	m_pSkinModel->m_vPos = m_vPos;

	m_pSkinModel->m_fYaw = m_vRot.y;
	m_pSkinModel->m_fPitch = m_vRot.x;
	m_pSkinModel->m_fRoll = m_vRot.z;

	m_pSkinModel->m_fScale = m_fScale;

	m_pSkinModel->m_dAnimSpeed = m_dAnimSpeed;
}
//============================================================
//	位置更新関数.
//============================================================
void clsCharacterSkin::UpdatePos()
{
	//ｷｬﾗｸﾀ移動処理(ﾗｼﾞｺﾝ)
	D3DXMATRIX mYaw;
	D3DXMatrixRotationY(&mYaw, m_vRot.y);

	//Z軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3	vecAxisZ(0.0f, 0.0f, 1.0f);

	//Z軸ﾍﾞｸﾄﾙそのものを回転状態により変換する.
	D3DXVec3TransformCoord(
		&vecAxisZ,	//(out)
		&vecAxisZ,
		&mYaw);	//Y軸回転行列.

	//方向によって進行させる値を設定.
	switch (m_enDir)
	{
	case enDirection_Stop:
		break;
	case enDirection_Foward:	//前進.
		//向かう方向*進める値(0.1f).
		m_vPos += vecAxisZ * 0.15f * m_fSpd;
		//		m_fWalkLangth -= 0.15f * m_fSpd;
		break;
	case enDirection_Backward:	//後退.
		m_vPos -= vecAxisZ * 0.15f * m_fSpd;
		//		m_fWalkLangth -= 0.15f * m_fSpd;
		break;
		//case enDirection_LeftTurn:
		//	break;
		//case enDirection_RightTurn:
		//	break;
		//default:
		//	break;
	}
	m_enDir = enDirection_Stop;//停止.
}
//アニメーション最大数を取得する.
int clsCharacterSkin::GetAnimSetMax()
{
	if (m_pAnimCtrl != NULL) {
		return (int)m_pAnimCtrl->GetMaxNumAnimationSets();
	}
	return -1;
}

//アニメーション切り替え.
void clsCharacterSkin::ChangeAnimSet(int index, double dStartPos)
{
	//アニメーションの範囲内かチェック.
	if (index < 0 || index >= GetAnimSetMax()) {
		return;
	}
	m_pSkinModel->ChangeAnimSet_StartPos(
		index, dStartPos, m_pAnimCtrl);
}