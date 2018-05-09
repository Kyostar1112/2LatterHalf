#ifndef _C_CHARACTER_H_
#define _C_CHARACTER_H_

//ｲﾝｸﾙｰﾄﾞﾌｧｲﾙ.
#include "DX9Mesh.h"
#include "GameObject.h"

/***********************************************************************************
*
*	キャラクタクラス.
*
**/
class clsCharacter
	: public clsGameObject
{
public:
	clsCharacter();
	virtual ~clsCharacter();

	//モデルデータ関連付け.
	void AttachModel(clsDX9Mesh* pModel);
	//モデルデータ関連付け解除.
	void DetachModel();

	//描画.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vLight, D3DXVECTOR3& vEye,
		D3DXVECTOR4 vColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f),
		bool alphaFlg = false);

	clsDX9Mesh* GetPointor() {
		return m_pModel;
	}

	LPD3DXMESH GetMesh() {
		return m_pModel->m_pMesh;
	}

	LPD3DXMESH GetMeshForRay() {
		return m_pModel->m_pMeshForRay;
	}

	//位置更新関数.
	void UpdatePos();

	//座標や回転値を更新する.
	void Update();

	enDirection		m_enDir;	//方向.
	float			m_fSpd;
	SPHERE			m_Sphere;	//ｽﾌｨｱ構造体.

private:

	clsDX9Mesh*		m_pModel;
};

#endif //#ifndef _C_CHARACTER_H_
