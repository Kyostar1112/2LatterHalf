#include "GameObject.h"

clsGameObject::clsGameObject()
//: m_vPos( 0.0f, 0.0f, 0.0f )
//, m_vRot( 0.0f, 0.0f, 0.0f )
//, m_fScale( 1.0f )
{
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fScale = 1.0f;
}

clsGameObject::~clsGameObject()
{
}

//============================================================
//ˆÊ’uÝ’è.
//============================================================
//Ý’è.
void clsGameObject::SetPosition(D3DXVECTOR3 vPos)
{
	m_vPos = vPos;
}
void clsGameObject::SetPositionX(float fPosX)
{
	m_vPos.x = fPosX;
}
void clsGameObject::SetPositionY(float fPosY)
{
	m_vPos.y = fPosY;
}
void clsGameObject::SetPositionZ(float fPosZ)
{
	m_vPos.z = fPosZ;
}
//Žæ“¾.
D3DXVECTOR3 clsGameObject::GetPosition()
{
	return m_vPos;
}
float clsGameObject::GetPositionX()
{
	return m_vPos.x;
}
float clsGameObject::GetPositionY()
{
	return m_vPos.y;
}
float clsGameObject::GetPositionZ()
{
	return m_vPos.z;
}
//‘«‚µ‚±‚Ý.
void clsGameObject::AddPosition(D3DXVECTOR3 vMove)
{
	SetPosition(GetPosition() + vMove);
}
void clsGameObject::AddPositionX(float fMoveX)
{
	SetPositionX(GetPositionX() + fMoveX);
}
void clsGameObject::AddPositionY(float fMoveY)
{
	SetPositionY(GetPositionY() + fMoveY);
}
void clsGameObject::AddPositionZ(float fMoveZ)
{
	SetPositionZ(GetPositionZ() + fMoveZ);
}

//============================================================
//‰ñ“]Ý’è.
//============================================================
//Ý’è.
void clsGameObject::SetRotation(D3DXVECTOR3 vRot)
{
	m_vRot = vRot;
}
void clsGameObject::SetRotationY(float fRotY)
{
	m_vRot.y = fRotY;
}
void clsGameObject::SetRotationZ(float fRotZ)
{
	m_vRot.z = fRotZ;
}
//Žæ“¾.
D3DXVECTOR3 clsGameObject::GetRotation()
{
	return m_vRot;
}
float clsGameObject::GetRotationX()
{
	return m_vRot.x;
}
float clsGameObject::GetRotationY()
{
	return m_vRot.y;
}
float clsGameObject::GetRotationZ()
{
	return m_vRot.z;
}
//‘«‚µ‚±‚Ý.
void clsGameObject::AddRotation(D3DXVECTOR3 vMove)
{
	D3DXVECTOR3 theta = GetRotation() + vMove;
	//ThetaOverGuard( theta.x );
	//ThetaOverGuard( theta.y );
	//ThetaOverGuard( theta.z );
	SetRotation(theta);
}
void clsGameObject::AddRotationY(float fMoveY)
{
	float theta = GetRotationY() + fMoveY;
	//ThetaOverGuard( theta );
	SetRotationY(theta);
}
void clsGameObject::AddRotationZ(float fMoveZ)
{
	float theta = GetRotationZ() + fMoveZ;
	//ThetaOverGuard( theta );
	SetRotationZ(theta);
}

//============================================================
//ŠgkÝ’è.
//============================================================
void clsGameObject::SetScale(float fScale)
{
	m_fScale = fScale;
}
float clsGameObject::GetScale()
{
	return m_fScale;
}

//============================================================
//	360`0‚É”[‚ß‚é.
//============================================================
//void clsGameObject::ThetaOverGuard( float& theta )
//{
//#define THETA_MAX (float)( M_PI * 2.0 )
//	if( theta > THETA_MAX ){
//		theta -= THETA_MAX;
//	}
//	else if( theta < 0.0f ){
//		theta += THETA_MAX;
//	}
//}
//void clsGameObject::ThetaOverGuard( double& theta )
//{
//#define THETA_MAX ( M_PI * 2.0 )
//	if( theta > THETA_MAX ){
//		theta -= THETA_MAX;
//	}
//	else if( theta < 0.0 ){
//		theta += THETA_MAX;
//	}
//}
//