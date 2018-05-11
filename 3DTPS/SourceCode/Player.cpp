#include"Player.h"

clsPlayer::clsPlayer()
{
}

clsPlayer::~clsPlayer()
{
}

void clsPlayer::Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
	D3DXVECTOR3& vLight, D3DXVECTOR3& vEye)
{
	if (m_pSkinModel == nullptr && m_pAnimCtrl == nullptr) {
		return;
	}
	//レンダーだけ変更したい場合用.
	D3DXVECTOR3 TmpPos;
	D3DXVECTOR3 TmpRot;

	TmpPos = GetPosition();
	TmpRot = GetRotation();

	//モデルの前後反転.
	AddRotationY(D3DXToRadian(180));

	Update();

	m_pSkinModel->Render(
		mView, mProj, vLight, vEye,
		m_pAnimCtrl);

	SetPosition(TmpPos);
	SetRotation(TmpRot);
}
