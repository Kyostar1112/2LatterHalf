#ifndef _C_CHARACTER_SKIN_H_
#define _C_CHARACTER_SKIN_H_

#include"D3DXSKINMESH.h"
#include"GameObject.h"

/*****************************************************************************************************
*	�L�����N�^�N���X�i�X�L�����b�V���N���X�j
*/
class clsCharacterSkin
	: public clsGameObject
{
public:
	clsCharacterSkin();
	~clsCharacterSkin();

	//���f���f�[�^�֘A�t��.
	void AttachModel(clsD3DXSKINMESH* pModel);
	//���f���f�[�^�֘A�t������.
	void DetachModel();

	//�����ݸ�.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vEye);

	//�A�j���[�V�����ő吔���擾����.
	int GetAnimSetMax();
	//�A�j���[�V�����؂�ւ�.
	void ChangeAnimSet(int index, double dStartPos = 0.0f);
	double GetAnimPeriod(int index)
	{
		return m_pSkinModel->GetAnimPeriod(index);
	}

	double m_dAnimSpeed;
	double m_dAnimTime;
	int m_iAnimNo;

	//�ʒu�X�V�֐�.
	void UpdatePos();
	enDirection		m_enDir;	//����.
	float			m_fSpd;

	clsD3DXSKINMESH* GetPointor() {
		return m_pSkinModel;
	}
	//���W���]�̍X�V.
	virtual void Update();//���z�֐�.
private:
	clsD3DXSKINMESH*			m_pSkinModel;
	LPD3DXANIMATIONCONTROLLER	m_pAnimCtrl;
};

#endif //#ifndef _C_CHARACTER_SKIN_H_.
