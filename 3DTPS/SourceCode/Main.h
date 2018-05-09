//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include "DX9Mesh.h"	//Dx9を使用するｸﾗｽ.
#include "DebugText.h"	//ﾃﾞﾊﾞｯｸﾞﾃｷｽﾄｸﾗｽ.
#include "D3DXSKINMESH.h"	//ｽｷﾝﾒｯｼｭｸﾗｽ.

#include "DxInput.h"	//ｺﾝﾄﾛｰﾗｸﾗｽ.
#include "Ray.h"		//ﾚｲ表示ｸﾗｽ.

#include <thread>

//シーンクラス達.
#include "TitleScene.h"
#include "StageScene.h"
#include "OverScene.h"
#include "ClearScene.h"
#include "ResultScene.h"

//======================================
//	ﾗｲﾌﾞﾗﾘ.
//======================================
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )//「D3DX～」の定義使用時に必要.

//======================================
//	定数.
//======================================
#define WND_TITLE	"ディフェンサー"
#define APR_NAME	"ディフェンサー"

const float WSPACE = 0.8f;	//壁との限界距離.

//======================================
//	構造体.
//======================================
//ｶﾒﾗ情報.
struct CAMERA
{
	D3DXVECTOR3	vEye;	//ｶﾒﾗ位置.
	D3DXVECTOR3	vLook;	//注視位置.
	float		fYaw;	//回転値.
	D3DXMATRIX	mRot;	//回転行列.
};

/*************************************************
*	ﾒｲﾝｸﾗｽ.
**/
class clsMain
{
private:
	HWND	m_hWnd;	//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.

public:
	clsMain();	//ｺﾝｽﾄﾗｸﾀ.
	~clsMain();	//ﾃﾞｽﾄﾗｸﾀ.

	//ｳｨﾝﾄﾞｳ初期化関数.
	HRESULT InitWindow(
		HINSTANCE hInstance,
		INT x, INT y, INT width, INT height,
		LPSTR WindowName);

	//ｳｨﾝﾄﾞｳ関数(ﾒｯｾｰｼﾞ毎の処理)
	LRESULT MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);

	//ﾒｯｾｰｼﾞﾙｰﾌﾟ.
	void Loop();
	//ｱﾌﾟﾘｹｰｼｮﾝﾒｲﾝ処理.
	void AppMain();
	//描画(ﾚﾝﾀﾞﾘﾝｸﾞ)
	void Render();

private:
	//↓ｱﾌﾟﾘにひとつ.
	ID3D11Device*			m_pDevice;	//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext*	m_pDeviceContext;//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
	IDXGISwapChain*			m_pSwapChain;//ｽﾜｯﾌﾟﾁｪｰﾝ.
	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;//ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰ.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;//ﾊﾞｯｸﾊﾞｯﾌｧ.
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;//ﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰ.

	D3DXVECTOR3		m_vLight;		//ﾗｲﾄの方向.

	CAMERA			m_Camera;		//ｶﾒﾗ情報.

	D3DXMATRIX		m_mView;		//ﾋﾞｭｰ(ｶﾒﾗ)行列.
	D3DXMATRIX		m_mProj;		//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列.

	int					m_AnimationNo;//ｱﾆﾒｰｼｮﾝ番号.
	double				m_AnimTime;	//ｱﾆﾒｰｼｮﾝ時間.

	//ﾃﾞﾊﾞｯｸﾞﾃｷｽﾄｸﾗｽ.
	unique_ptr<clsDebugText>	m_smpText;

	//深度(Z)ﾃｽﾄ設定.
	ID3D11DepthStencilState* m_pDepthStencilState;

	//ｺﾝﾄﾛｰﾗｸﾗｽ.
	unique_ptr<clsDxInput>	m_smpDxInput;

	//ﾚｲ表示ｸﾗｽ.
	unique_ptr<clsRay>			m_smpRayV;	//垂直.
	unique_ptr<clsRay>			m_smpRayFB;	//前後.
	unique_ptr<clsRay>			m_smpRayH;	//左右.

	//キャラクタクラス.
	int	m_iAnimModelArrayMax;

	clsResource*				m_pResource;//リソースクラス.

	//今現在のシーン.
	SceneMode m_enScene;
	//シーンが変わった瞬間をとるための変数.
	SceneMode m_enOldScene;
	//シーン名.
	vector<string> m_vsSceneName;

	//シーンクラス達.
	unique_ptr<clsTitleScene>	m_smpTitleScene;
	unique_ptr<clsStageScene>	m_smpStageScene;
	unique_ptr<clsOverScene>	m_smpOverScene;
	unique_ptr<clsClearScene>	m_smpClearScene;
	unique_ptr<clsResultScene>	m_smpResultScene;

	unique_ptr<clsSound>	m_smpSeClick;

	void SceneChange();
	bool m_bTitleFlg;

public:
	//DirectX初期化.
	HRESULT InitD3D();
	//Direct3D終了処理.
	void DestroyD3D();

	//ﾒｯｼｭ読込関数（まとめた）
	HRESULT MeshRead();

	//深度ﾃｽﾄ(Zﾃｽﾄ) ON/OFF切替.
	void SetDepth(bool bFlg);

	//ﾚｲとﾒｯｼｭの当たり判定.
	bool Intersect(
		clsGameObject* pAttacker, clsCharacter* pTarget,
		float* pfDistance, D3DXVECTOR3* pvIntersect);
	//交差位置のﾎﾟﾘｺﾞﾝの頂点を見つける.
	HRESULT FindVerticesOnPoly(
		LPD3DXMESH pTarget, DWORD dwPolyIndex,
		D3DXVECTOR3* pVecVertices);

	//壁の当たり判定関連.
	void WallJudge(clsGameObject* pAttacker, clsCharacter* pWall);
	void dirOverGuard(float* fYaw);//回転値調整.

	//ｶﾒﾗ関数.
	void Camera();
	//ﾌﾟﾛｼﾞｪｸｼｮﾝ関数.
	void Proj();
};