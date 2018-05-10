#ifndef _C_RESURCE_H_
#define _C_RESURCE_H_
#include"Global.h"
#include"DX9Mesh.h"
#include"D3DXSKINMESH.h"
#include"File.h"
#include<memory>

/**************************************************
*	リソースクラス.
*/
struct ModelRenderSet
{
	D3DXMATRIX  mView;
	D3DXMATRIX  mProj;
	D3DXVECTOR3 vLight;
	D3DXVECTOR3 vEye;
};
struct SpriteRenderSet
{
	HWND hWnd;
	ID3D11Device*  pDevice11;
	ID3D11DeviceContext* pContext11;
};
class clsResource
{
public:
	//スタティックモデル種類.
	enum enStaticModel
	{
		enStaticModel_Min = 0,	//ここにモデルを入れないで.

		enStaticModel_Plane,
		enStaticModel_Player,
		enStaticModel_Stage1,
		enStaticModel_Stage2,
		enStaticModel_Shot,
		enStaticModel_Sphere,

		enStaticModel_Max,		//ここにモデルを入れないで.
	};

	//スキンモデル種類.
	enum enSkinModel
	{
		enSkinModel_Min = 0,	//ここにモデルを入れないで.

		enSkinModel_Boss,

		enSkinModel_Max,		//ここにモデルを入れないで.
	};
	//インスタンス取得(唯一のアクセス経路).
	static clsResource* GetInstance()
	{
		//唯一のインスタンスを作成する.
		//(staticで作成されたので2回目は無視される).
		static clsResource s_Instance;
		return &s_Instance;
	}
	//	clsResource();
	~clsResource();

	HRESULT Init(HWND hWnd,
		ID3D11Device* pDevice,
		ID3D11DeviceContext* pContext);

	HRESULT CreateStaticModel(
		LPSTR FileName, enStaticModel enModel);
	HRESULT ReleaseStaticModel(enStaticModel enModel);
	clsDX9Mesh* GetStaticModels(enStaticModel enModel);

	//スキンモデル用.
	HRESULT CreateSkinModel(
		LPSTR FileName, enSkinModel enModel);
	HRESULT ReleaseSkinModel(enSkinModel enModel);
	clsD3DXSKINMESH* GetSkinModels(enSkinModel enModel);

	ModelRenderSet GetModelRenderSet()
	{
		return m_stModelRenderSet;
	}

	SpriteRenderSet GetSpriteRenderSet()
	{
		return m_stSpriteRenderSet;
	}

	void SetModelRender(D3DXMATRIX  mView, D3DXMATRIX  mProj, D3DXVECTOR3 vLight, D3DXVECTOR3 vEye)
	{
		m_stModelRenderSet.mView = mView;
		m_stModelRenderSet.mProj = mProj;
		m_stModelRenderSet.vLight = vLight;
		m_stModelRenderSet.vEye = vEye;
	}

	void SetSpriteRender(HWND hWnd, ID3D11Device*  pDevice11, ID3D11DeviceContext* pContext11)
	{
		m_stSpriteRenderSet.hWnd = hWnd;
		m_stSpriteRenderSet.pDevice11 = pDevice11;
		m_stSpriteRenderSet.pContext11 = pContext11;
	}
	unique_ptr<clsFile> m_smpFile;
	vector<int> m_viScore;
	void ScoreComparison()
	{
		for (size_t i = m_viScore.size() - 1; i >= 0; i--)
		{
			for (size_t j = m_viScore.size() - 1; j >= 0; j--)
			{
				if (m_viScore[i] == m_viScore[j])
				{
					m_viScore.erase(m_viScore.begin() + j);
				}
			}
		}
	}
private:
	//生成やコピーを禁止する.
	clsResource();
	clsResource(const clsResource& rhs);
	clsResource& operator = (const clsResource& rhs);

	ModelRenderSet m_stModelRenderSet;
	SpriteRenderSet m_stSpriteRenderSet;

	CD3DXSKINMESH_INIT m_si;

	vector<unique_ptr<clsDX9Mesh>> 		m_vsmpStaticModels;	//スタティックモデル.
	vector<unique_ptr<clsD3DXSKINMESH>>	m_vsmpSkinModels;	//スキンモデル.
	bool IsRangeStaticModel(enStaticModel enModel);
	bool IsRangeSkinModel(enSkinModel enModel);
};

#endif//#ifndef _C_RESURCE_H_
