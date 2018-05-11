#include"Player.h"

clsPlayer::clsPlayer()
{
	m_dAnimNum = 1;
	ChangeAnimSet(m_dAnimNum);
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

	//アニメーションに合わせて少しずらす.
	float m_fLen = 1.0f;//ずらす長さ.

	float fDeg = GetRotationY();

	float rad =  -fDeg;

	float tmpfX, tmpfZ;

	tmpfX = m_fLen*cosf(rad);
	tmpfZ = m_fLen*sinf(rad);

	D3DXVECTOR3 vTmp = GetPosition();

	vTmp.x += tmpfX;
	vTmp.z += tmpfZ;
	SetPosition(vTmp);

	Update();

	m_pSkinModel->Render(
		mView, mProj, vLight, vEye,
		m_pAnimCtrl);

	SetPosition(TmpPos);
	SetRotation(TmpRot);
}
