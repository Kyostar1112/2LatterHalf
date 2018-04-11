#include "Effects.h"
#include "Global.h"

//描画用ｲﾝｽﾀﾝｽ最大数.
const int g_RenderSpriteMax = 2000;
//ｴﾌｪｸﾄ管理用ｲﾝｽﾀﾝｽ最大数.
const int g_EffectInstanceMax = 1000;

//ｴﾌｪｸﾄﾌｧｲﾙのﾘｽﾄ.
const wchar_t g_strFileNameList
[clsEffects::enEfcType_Max][128] =
{
	L"Data\\Effekseer\\Laser03.efk",
	L"Data\\Effekseer\\MagicDark.efk",
	L"Data\\Effekseer\\miko_hihou_effect.efk"
};

//ｺﾝｽﾄﾗｸﾀ.
clsEffects::clsEffects()
{
	ZeroMemory(this, sizeof(clsEffects));
}

//ﾃﾞｽﾄﾗｸﾀ.
clsEffects::~clsEffects()
{
}

//構築関数.
HRESULT clsEffects::Create(
	ID3D11Device* pDevice,
	ID3D11DeviceContext* pContext)
{
	//XAudio2の初期化を行う.
	if (FAILED(
		XAudio2Create(&m_pXA2)))
	{
		ERR_MSG("XAudio2作成失敗", "CEffects::Create()");
		return E_FAIL;
	}
	if (FAILED(
		m_pXA2->CreateMasteringVoice(&m_pXA2Master)))
	{
		ERR_MSG("MasteringVoice作成失敗", "CEffects::Create()");
		return E_FAIL;
	}

	//描画用ｲﾝｽﾀﾝｽの生成.
	m_pRender
		= ::EffekseerRendererDX11::Renderer::Create(
		pDevice, pContext, g_RenderSpriteMax);

	//ｴﾌｪｸﾄ管理用ｲﾝｽﾀﾝｽの生成.
	m_pManager
		= ::Effekseer::Manager::Create(g_EffectInstanceMax);

	//描画用ｲﾝｽﾀﾝｽから描画機能を設定.
	m_pManager->SetSpriteRenderer(m_pRender->CreateSpriteRenderer());
	m_pManager->SetRibbonRenderer(m_pRender->CreateRibbonRenderer());
	m_pManager->SetRingRenderer(m_pRender->CreateRingRenderer());
	m_pManager->SetModelRenderer(m_pRender->CreateModelRenderer());

	//描画ｲﾝｽﾀﾝｽからﾃｸｽﾁｬの読込機能を設定.
	//独自拡張可能、現在はﾌｧｲﾙから読み込んでいる.
	m_pManager->SetTextureLoader(m_pRender->CreateTextureLoader());
	m_pManager->SetModelLoader(m_pRender->CreateModelLoader());

	//音再生用ｲﾝｽﾀﾝｽの生成.
	m_pSound = ::EffekseerSound::Sound::Create(m_pXA2, 16, 16);

	//音再生用ｲﾝｽﾀﾝｽから再生機能を設定.
	m_pManager->SetSoundPlayer(m_pSound->CreateSoundPlayer());

	//音再生用ｲﾝｽﾀﾝｽからｻｳﾝﾄﾞﾃﾞｰﾀの読込機能を設定.
	//独自拡張可能、現在はﾌｧｲﾙから読み込んでいる.
	m_pManager->SetSoundLoader(m_pSound->CreateSoundLoader());

	return S_OK;
}

//破棄関数.
HRESULT clsEffects::Destroy()
{
	//ｴﾌｪｸﾄﾃﾞｰﾀを解放.
	ReleaseData();

	//先にｴﾌｪｸﾄ管理用ｲﾝｽﾀﾝｽを破棄.
	m_pManager->Destroy();

	//次に音再生用ｲﾝｽﾀﾝｽを破棄.
	m_pSound->Destory();

	//次に描画用ｲﾝｽﾀﾝｽを破棄.
	m_pRender->Destory();

	//XAudio2の解放.
	if (m_pXA2Master != NULL){
		m_pXA2Master->DestroyVoice();
		m_pXA2Master = NULL;
	}
	ES_SAFE_RELEASE(m_pXA2);

	return S_OK;
}

//ﾃﾞｰﾀ読込関数.
HRESULT clsEffects::LoadData()
{
	////ｴﾌｪｸﾄの読込.
	//m_pEffect
	//	= ::Effekseer::Effect::Create(
	//			m_pManager,
	//			(const EFK_CHAR*)L"Data\\Effekseer\\Laser03.efk" );
	//if( m_pEffect == NULL ){
	//	ERR_MSG( "ｴﾌｪｸﾄﾌｧｲﾙ読込失敗", "clsEffects::LoadData()" );
	//	return E_FAIL;
	//}

	//ｴﾌｪｸﾄの読込.
	for (int i = 0; i < enEfcType_Max; i++){
		m_pEffect[i]
			= ::Effekseer::Effect::Create(
			m_pManager,
			(const EFK_CHAR*)g_strFileNameList[i]);
		if (m_pEffect[i] == NULL){
			char strMsg[128];
			wsprintf(strMsg, "clsEffects::LoadData()\n%ls",
				g_strFileNameList[i]);
			ERR_MSG(strMsg, "ｴﾌｪｸﾄﾌｧｲﾙ読込失敗");
			return E_FAIL;
		}
	}

	return S_OK;
}

//ﾃﾞｰﾀ解放関数.
HRESULT clsEffects::ReleaseData()
{
	//ｴﾌｪｸﾄの破棄.
	for (int i = 0; i < enEfcType_Max; i++){
		ES_SAFE_RELEASE(m_pEffect[i]);
	}

	return S_OK;
}

//描画関数.
void clsEffects::Render(
	D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vEye)
{
	//ﾋﾞｭｰ行列を設定.
	SetViewMatrix(mView);

	//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列を設定.
	SetProjectionMatrix(mProj);

	//ｴﾌｪｸﾄの更新処理.
	m_pManager->Update();

	//-------------------------------------
	//	Effekseerﾚﾝﾀﾞﾘﾝｸﾞ.
	//-------------------------------------
	//ｴﾌｪｸﾄの描画開始処理を行う.
	m_pRender->BeginRendering();

	//ｴﾌｪｸﾄの描画を行う.
	m_pManager->Draw();

	//ｴﾌｪｸﾄの描画終了処理を行う.
	m_pRender->EndRendering();
}

//ﾋﾞｭｰ行列設定関数.
void clsEffects::SetViewMatrix(D3DXMATRIX& mView)
{
	//ﾋﾞｭｰ(ｶﾒﾗ)行列.
	::Effekseer::Matrix44 tmpEsCamMat;

	//DirectX Matrix → Effekseer Matrix に変換.
	tmpEsCamMat = MatrixDxToEfk(&mView);

	//ﾋﾞｭｰ行列を設定.
	m_pRender->SetCameraMatrix(tmpEsCamMat);
}

//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列設定関数.
void clsEffects::SetProjectionMatrix(D3DXMATRIX& mProj)
{
	//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列.
	::Effekseer::Matrix44 tmpEsPrjMat;

	//DirectX Matrix → Effekseer Matrix に変換.
	tmpEsPrjMat = MatrixDxToEfk(&mProj);

	//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列を設定.
	m_pRender->SetProjectionMatrix(tmpEsPrjMat);
}

// DirectX Vector3 → Effekseer Vector3 に変換する.
::Effekseer::Vector3D clsEffects::Vector3DxToEfk(
	const D3DXVECTOR3* pSrcVec3Dx)
{
	return ::Effekseer::Vector3D(
		pSrcVec3Dx->x,
		pSrcVec3Dx->y,
		pSrcVec3Dx->z);
}

// Effekseer Vector3 → DirectX Vector3 に変換する.
D3DXVECTOR3 clsEffects::Vector3EfkToDx(
	const ::Effekseer::Vector3D* pSrcVec3Efk)
{
	return D3DXVECTOR3(
		pSrcVec3Efk->X,
		pSrcVec3Efk->Y,
		pSrcVec3Efk->Z);
}

// DirectX Matrix → Effekseer Matrix に変換する.
::Effekseer::Matrix44 clsEffects::MatrixDxToEfk(
	const D3DXMATRIX* pSrcMatDx)
{
	::Effekseer::Matrix44 OutMatEfk;

#if 1
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			OutMatEfk.Values[i][j] = pSrcMatDx->m[i][j];
		}
	}
#else
	OutMatEfk.Values[0][0] = pSrcMatDx->_11;
	OutMatEfk.Values[0][1] = pSrcMatDx->_12;
	OutMatEfk.Values[0][2] = pSrcMatDx->_13;
	OutMatEfk.Values[0][3] = pSrcMatDx->_14;

	OutMatEfk.Values[1][0] = pSrcMatDx->_21;
	OutMatEfk.Values[1][1] = pSrcMatDx->_22;
	OutMatEfk.Values[1][2] = pSrcMatDx->_23;
	OutMatEfk.Values[1][3] = pSrcMatDx->_24;

	OutMatEfk.Values[2][0] = pSrcMatDx->_31;
	OutMatEfk.Values[2][1] = pSrcMatDx->_32;
	OutMatEfk.Values[2][2] = pSrcMatDx->_33;
	OutMatEfk.Values[2][3] = pSrcMatDx->_34;

	OutMatEfk.Values[3][0] = pSrcMatDx->_41;
	OutMatEfk.Values[3][1] = pSrcMatDx->_42;
	OutMatEfk.Values[3][2] = pSrcMatDx->_43;
	OutMatEfk.Values[3][3] = pSrcMatDx->_44;
#endif

	return OutMatEfk;
}

// Effekseer Matrix → DirectX Matrix に変換する.
D3DXMATRIX clsEffects::MatrixEfkToDx(
	const ::Effekseer::Matrix44* pSrcMatEfk)
{
	D3DXMATRIX OutMatDx;

#if 1
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			OutMatDx.m[i][j] = pSrcMatEfk->Values[i][j];
		}
	}
#else
	OutMatDx._11 = pSrcMatEfk->Values[0][0];
	OutMatDx._12 = pSrcMatEfk->Values[0][1];
	OutMatDx._13 = pSrcMatEfk->Values[0][2];
	OutMatDx._14 = pSrcMatEfk->Values[0][3];

	OutMatDx._21 = pSrcMatEfk->Values[1][0];
	OutMatDx._22 = pSrcMatEfk->Values[1][1];
	OutMatDx._23 = pSrcMatEfk->Values[1][2];
	OutMatDx._24 = pSrcMatEfk->Values[1][3];

	OutMatDx._31 = pSrcMatEfk->Values[2][0];
	OutMatDx._32 = pSrcMatEfk->Values[2][1];
	OutMatDx._33 = pSrcMatEfk->Values[2][2];
	OutMatDx._34 = pSrcMatEfk->Values[2][3];

	OutMatDx._41 = pSrcMatEfk->Values[3][0];
	OutMatDx._42 = pSrcMatEfk->Values[3][1];
	OutMatDx._43 = pSrcMatEfk->Values[3][2];
	OutMatDx._44 = pSrcMatEfk->Values[3][3];
#endif

	return OutMatDx;
}