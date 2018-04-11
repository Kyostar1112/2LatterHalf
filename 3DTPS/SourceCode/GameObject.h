#ifndef _C_GAME_OBJECT_H_
#define _C_GAME_OBJECT_H_

#include "Global.h"

class clsGameObject
{
public:
	clsGameObject();
	virtual ~clsGameObject();//継承目的なら仮想化しておかないと継承先のデストラクタをとおってくれない.

	//位置設定.
	virtual void SetPosition(D3DXVECTOR3 vPos);//SpiaMgeでvirtual.
	void SetPositionX(float fPosX);
	void SetPositionY(float fPosY);
	void SetPositionZ(float fPosZ);
	//取得.
	D3DXVECTOR3 GetPosition();
	float GetPositionX();
	float GetPositionY();
	float GetPositionZ();
	//足しこみ.
	void AddPosition(D3DXVECTOR3 vMove);
	void AddPositionX(float fMoveX);
	void AddPositionY(float fMoveY);
	void AddPositionZ(float fMoveZ);

	//回転設定.
	//設定.
	void SetRotation(D3DXVECTOR3 vRot);
	void SetRotationY(float fRotY);
	void SetRotationZ(float fRotZ);
	//取得.
	D3DXVECTOR3 GetRotation();
	float GetRotationX();
	float GetRotationY();
	float GetRotationZ();
	//足しこみ.
	void AddRotation(D3DXVECTOR3 vMove);
	void AddRotationY(float fMoveY);
	void AddRotationZ(float fMoveZ);

	//拡縮設定.
	void SetScale(float fScale);
	float GetScale();

	D3DXVECTOR3 m_vRay;
	D3DXVECTOR3 m_vAxis;

private:

protected://
	D3DXVECTOR3		m_vPos;
	D3DXVECTOR3		m_vRot;
	float			m_fScale;

	//座標や回転の更新.
	virtual void Update() = 0; //純粋仮想関数.
};

#endif//#ifndef _C_GAME_OBJECT_H_.