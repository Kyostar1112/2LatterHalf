#ifndef _CEFFECTS_H_
#define _CEFFECTS_H_

//警告についてのｺｰﾄﾞ分析を無効にする。4005:再定義.
#pragma warning( disable : 4005 )

//------------------------------------------------
//	先にしておくﾍｯﾀﾞ,ﾗｲﾌﾞﾗﾘの読込.
//------------------------------------------------
#include <stdio.h>

#include <D3D11.h>
#include <XAudio2.h>
#pragma comment( lib, "d3d11.lib" )

//------------------------------------------------
//	ここからEffekseer関係 ﾍｯﾀﾞ,ﾗｲﾌﾞﾗﾘの読込.
//------------------------------------------------
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>

#if _DEBUG
#pragma comment( lib, "VS2013\\Debug\\Effekseer.lib" )
#pragma comment( lib, "VS2013\\Debug\\EffekseerRendererDX11.lib" )
#pragma comment( lib, "VS2013\\Debug\\EffekseerSoundXAudio2.lib" )
#else//#if _DEBUG
#pragma comment( lib, "VS2013\\Release\\Effekseer.lib" )
#pragma comment( lib, "VS2013\\Release\\EffekseerRendererDX11.lib" )
#pragma comment( lib, "VS2013\\Release\\EffekseerSoundXAudio2.lib" )
#endif//#if _DEBUG

//「D3DX～」使用で必須.
//注意:Effekseer関係より後に読み込むこと.
#include <D3DX10.h>
#pragma comment( lib, "d3dx10.lib" )

/*************************************************
*	ﾌﾘｰｿﾌﾄ Effekseerのﾃﾞｰﾀを使うためのｸﾗｽ.
**/
class clsEffects
{
public:
	//ｴﾌｪｸﾄ種類列挙体.
	enum enEfcType
	{
		enEfcType_Laser = 0,
		enEfcType_MagicDark,
		enEfcType_Hihou,
		enEfcType_Max,
	};

	//ｲﾝｽﾀﾝｽ取得(唯一のｱｸｾｽ経路)
	static clsEffects* GetInstance()
	{
		//唯一のｲﾝｽﾀﾝｽを作成する.
		//(staticで作成されたので2回目は無視される)
		static clsEffects s_Instance;
		return &s_Instance;
	}
	//ﾃﾞｽﾄﾗｸﾀ.
	~clsEffects();

	//構築関数.
	HRESULT Create(ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);
	//破棄関数.
	HRESULT Destroy();
	//ﾃﾞｰﾀ読込関数.
	HRESULT LoadData();
	//ﾃﾞｰﾀ解放関数.
	HRESULT ReleaseData();
	//描画関数.
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj,
		D3DXVECTOR3& vEye);
	//ﾋﾞｭｰ行列設定関数.
	void SetViewMatrix(D3DXMATRIX& mView);
	//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列設定関数.
	void SetProjectionMatrix(D3DXMATRIX& mProj);

	//再生関数.
	::Effekseer::Handle Play(enEfcType enType, D3DXVECTOR3 vPos) {
		return m_pManager->Play(
			m_pEffect[enType], vPos.x, vPos.y, vPos.z);
	}
	//一時停止.
	void Paused(::Effekseer::Handle handle, bool bFlag) {
		m_pManager->SetPaused(handle, bFlag);//bFlag:true 一時停止.
	}
	//停止.
	void Stop(::Effekseer::Handle handle) {
		m_pManager->StopEffect(handle);
	}
	//全て停止.
	void StopAll(::Effekseer::Handle handle) {
		m_pManager->StopAllEffects();
	}
	//ｻｲｽﾞを指定する.
	void SetScale(::Effekseer::Handle handle, D3DXVECTOR3 vScale) {
		m_pManager->SetScale(handle, vScale.x, vScale.y, vScale.z);
	}
	//回転を指定する.
	void SetRotation(::Effekseer::Handle handle, D3DXVECTOR3 vRot) {
		m_pManager->SetRotation(handle, vRot.x, vRot.y, vRot.z);
	}
	void SetRotation(::Effekseer::Handle handle,
		D3DXVECTOR3 vAxis, float fAngle) {
		m_pManager->SetRotation(handle,
			::Effekseer::Vector3D(vAxis.x, vAxis.y, vAxis.z),
			fAngle);
	}
	//位置を指定する.
	void SetLocation(::Effekseer::Handle handle, D3DXVECTOR3 vPos) {
		m_pManager->SetLocation(handle,
			::Effekseer::Vector3D(vPos.x, vPos.y, vPos.z));
	}

	//ﾍﾞｸﾀｰ変換関数.
	::Effekseer::Vector3D Vector3DxToEfk(
		const D3DXVECTOR3* pSrcVec3Dx);
	D3DXVECTOR3 Vector3EfkToDx(
		const ::Effekseer::Vector3D* pSrcVec3Efk);
	//行列変換関数.
	::Effekseer::Matrix44 MatrixDxToEfk(
		const D3DXMATRIX* pSrcMatDx);
	D3DXMATRIX MatrixEfkToDx(
		const ::Effekseer::Matrix44* pSrcMatEfk);

private:
	//生成やｺﾋﾟｰを禁止する.
	clsEffects();	//ｺﾝｽﾄﾗｸﾀ.
	clsEffects(const clsEffects& rhs);
	clsEffects& operator = (const clsEffects& rhs);

	//ｴﾌｪｸﾄを動作させるために必要.
	::Effekseer::Manager*			m_pManager;
	::EffekseerRenderer::Renderer*	m_pRender;
	::EffekseerSound::Sound*		m_pSound;
	//ｴﾌｪｸﾄﾃﾞｰﾀに含まれる音再生に必要.
	IXAudio2*						m_pXA2;
	IXAudio2MasteringVoice*			m_pXA2Master;

	//ｴﾌｪｸﾄの種類ごとに必要.
	::Effekseer::Effect*			m_pEffect[enEfcType_Max];
	//ｴﾌｪｸﾄごとに必要.
	//※同じｴﾌｪｸﾄを同時に３つ出すなら３つ必要.
	::Effekseer::Handle				m_Handle;
};

#endif//#ifndef _CEFFECTS_H_