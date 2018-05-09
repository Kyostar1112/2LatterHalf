#include"Resource.h"

clsResource::clsResource()
	: m_vsmpStaticModels(NULL)
{
	m_stSpriteRenderSet.hWnd = NULL;
	m_stSpriteRenderSet.pDevice11 = NULL;
	m_stSpriteRenderSet.pContext11 = NULL;
}

clsResource::~clsResource()
{
	//スタティックメッシュのポインタ領域を解放する.
	if (m_vsmpStaticModels.empty()) {
		for (size_t i = 0; i < m_vsmpStaticModels.size(); i++)
		{
			ReleaseStaticModel((enStaticModel)i);
		}
		m_vsmpStaticModels.clear();
		m_vsmpStaticModels.shrink_to_fit();
	}
	//スキンメッシュのポインタ領域を解放する.
	if (m_vsmpSkinModels.empty()) {
		for (size_t i = 0; i < m_vsmpSkinModels.size(); i++)
		{
			ReleaseSkinModel((enSkinModel)i);
		}
		m_vsmpSkinModels.clear();
		m_vsmpSkinModels.shrink_to_fit();
	}
	m_stSpriteRenderSet.hWnd = NULL;
	m_stSpriteRenderSet.pDevice11 = NULL;
	m_stSpriteRenderSet.pContext11 = NULL;
}

HRESULT clsResource::Init(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_stSpriteRenderSet.hWnd = hWnd;
	m_stSpriteRenderSet.pDevice11 = pDevice;
	m_stSpriteRenderSet.pContext11 = pContext;

	m_si.hWnd = m_stSpriteRenderSet.hWnd;
	m_si.pDevice = m_stSpriteRenderSet.pDevice11;
	m_si.pDeviceContext = m_stSpriteRenderSet.pContext11;

	//スタティックメッシュのポインタ領域を確保.
	for (int i = 0; i < enStaticModel_Max; i++)
	{
		m_vsmpStaticModels.push_back(make_unique<clsDX9Mesh>());
		m_vsmpStaticModels[i] = NULL;	//NULL初期化.
	}

	//スキンメッシュのポインタ領域を確保.
	for (int i = 0; i < enSkinModel_Max; i++)
	{
		m_vsmpSkinModels.push_back(make_unique<clsD3DXSKINMESH>());
		m_vsmpStaticModels[i] = NULL;	//NULL初期化.
	}

	return S_OK;
}

HRESULT clsResource::CreateStaticModel(
	LPSTR FileName, enStaticModel enModel)
{
	//モデル読込.
	m_vsmpStaticModels[enModel] = make_unique<clsDX9Mesh>();
	m_vsmpStaticModels[enModel]->Init
	(m_stSpriteRenderSet.hWnd, m_stSpriteRenderSet.pDevice11, m_stSpriteRenderSet.pContext11, FileName);

	if (!IsRangeStaticModel(enModel)) {
		return E_FAIL;
	}
	//モデルがすでにない？.
	if (m_vsmpStaticModels[enModel] == NULL) {
		return E_FAIL;
	}

	return S_OK;
}
HRESULT clsResource::ReleaseStaticModel(enStaticModel enModel)
{
	if (!IsRangeStaticModel(enModel)) {
		return E_FAIL;
	}
	//モデルがすでにない？.
	if (m_vsmpStaticModels[enModel] == NULL) {
		return E_FAIL;
	}

	//削除してNULLで埋める.
	m_vsmpStaticModels[enModel].reset();
	m_vsmpStaticModels[enModel] = NULL;

	return S_OK;
}
clsDX9Mesh* clsResource::GetStaticModels(enStaticModel enModel)
{
	//範囲内チェック.
	if (!IsRangeStaticModel(enModel)) {
		return NULL;
	}

	//モデルあるかチェック.
	if (m_vsmpStaticModels[enModel] == NULL) {
		return NULL;
	}

	//モデルのポインタを返す.
	return m_vsmpStaticModels[enModel].get();
}

//範囲内かチェックする関数.
//	範囲内に収まっているなら　trueを返す.
bool clsResource::IsRangeStaticModel(enStaticModel enModel)
{
	//列挙体の範囲外か？
	if (enStaticModel_Min < enModel  &&  enModel < enStaticModel_Max) {
		return true;
	}

	return false;
}

HRESULT clsResource::CreateSkinModel(
	LPSTR FileName, enSkinModel enModel)
{
	//モデル読込.
	m_vsmpSkinModels[enModel] = make_unique<clsD3DXSKINMESH>();

	m_vsmpSkinModels[enModel]->Init(&m_si);

	m_vsmpSkinModels[enModel]->CreateFromX(FileName);

	if (!m_vsmpSkinModels[enModel]) {
		return E_FAIL;
	}
	//モデルがすでにない？.
	if (m_vsmpSkinModels[enModel] == NULL) {
		return E_FAIL;
	}

	return S_OK;
}

HRESULT clsResource::ReleaseSkinModel(enSkinModel enModel)
{
	if (!IsRangeSkinModel(enModel)) {
		return E_FAIL;
	}
	//モデルがすでにない？.
	if (m_vsmpSkinModels[enModel] == NULL) {
		return E_FAIL;
	}

	//削除してNULLで埋める.
	m_vsmpSkinModels[enModel].reset();
	m_vsmpSkinModels[enModel] = NULL;

	return S_OK;
}
clsD3DXSKINMESH* clsResource::GetSkinModels(enSkinModel enModel)
{
	//範囲内チェック.
	if (!IsRangeSkinModel(enModel)) {
		return NULL;
	}

	//モデルあるかチェック.
	if (m_vsmpSkinModels[enModel] == NULL) {
		return NULL;
	}

	//モデルのポインタを返す.
	return m_vsmpSkinModels[enModel].get();
}

//範囲内かチェックする関数.
//	範囲内に収まっているなら　trueを返す.
bool clsResource::IsRangeSkinModel(enSkinModel enModel)
{
	//列挙体の範囲外か？
	if (enSkinModel_Min < enModel  &&  enModel < enSkinModel_Max) {
		return true;
	}

	return false;
}