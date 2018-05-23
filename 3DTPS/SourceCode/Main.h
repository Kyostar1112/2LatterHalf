//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include "DX9Mesh.h"	//Dx9を使用するクラス.
#include "DebugText.h"	//デバッグテキストクラス.
#include "D3DXSKINMESH.h"	//スキンメッシュクラス.

#include "DxInput.h"	//コントローラクラス.
#include "Ray.h"		//レイ表示クラス.

#include <thread>
#include <mutex>

//シーンクラス達.
#include "TitleScene.h"
#include "StageScene.h"
#include "OverScene.h"
#include "ClearScene.h"
#include "ResultScene.h"

#include<atomic>

//======================================
//	ライブラリ.
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
//カメラ情報.
struct CAMERA
{
	D3DXVECTOR3	vEye;	//カメラ位置.
	D3DXVECTOR3	vLook;	//注視位置.
	float		fYaw;	//回転値.
	D3DXMATRIX	mRot;	//回転行列.
};

/*************************************************
*	メインクラス.
**/
class clsMain
{
private:
	HWND	m_hWnd;	//ウィンドウハンドル.

public:
	clsMain();	//コンストラクタ.
	~clsMain();	//デストラクタ.

	//ウィンドウ初期化関数.
	HRESULT InitWindow(
		HINSTANCE hInstance,
		INT x, INT y, INT width, INT height,
		LPSTR WindowName);

	//ウィンドウ関数(メッセージ毎の処理)
	LRESULT MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);

	//メッセージループ.
	void Loop();
	//アプリケーションメイン処理.
	void AppMain();
	//描画(レンダリング)
	void Render();

private:
	//↓アプリにひとつ.
	ID3D11Device*			m_pDevice;	//デバイスオブジェクト.
	ID3D11DeviceContext*	m_pDeviceContext;//デバイスコンテキスト.
	IDXGISwapChain*			m_pSwapChain;//スワップチェーン.
	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;//レンダーターゲットビュー.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;//バックバッファ.
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;//デプスステンシルビュー.

	D3DXVECTOR3		m_vLight;		//ライトの方向.

	CAMERA			m_Camera;		//カメラ情報.

	D3DXMATRIX		m_mView;		//ビュー(カメラ)行列.
	D3DXMATRIX		m_mProj;		//プロジェクション行列.

	int					m_AnimationNo;//アニメーション番号.
	double				m_AnimTime;	//アニメーション時間.

	//デバッグテキストクラス.
	unique_ptr<clsDebugText>	m_smpText;

	//深度(Z)テスト設定.
	ID3D11DepthStencilState* m_pDepthStencilState;

	//コントローラクラス.
	unique_ptr<clsDxInput>	m_smpDxInput;

	//レイ表示クラス.
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

	//ロード画面用.
	unique_ptr<clsSprite2D> m_smpLoadString;
	unique_ptr<clsSprite2D> m_smpLoadCircle;

	atomic<bool> m_bLoadFlg;

	void SceneChange();
	bool m_bTitleFlg;

public:
	//DirectX初期化.
	HRESULT InitD3D();
	//Direct3D終了処理.
	void DestroyD3D();

	//メッシュ読込関数（まとめた）
	HRESULT MeshRead();

	//深度テスト(Zテスト) ON/OFF切替.
	void SetDepth(bool bFlg);

	//レイとメッシュの当たり判定.
	bool Intersect(
		clsGameObject* pAttacker, clsCharacter* pTarget,
		float* pfDistance, D3DXVECTOR3* pvIntersect);
	//交差位置のポリゴンの頂点を見つける.
	HRESULT FindVerticesOnPoly(
		LPD3DXMESH pTarget, DWORD dwPolyIndex,
		D3DXVECTOR3* pVecVertices);

	//壁の当たり判定関連.
	void WallJudge(clsGameObject* pAttacker, clsCharacter* pWall);
	void dirOverGuard(float* fYaw);//回転値調整.

	//カメラ関数.
	void Camera();
	//プロジェクション関数.
	void Proj();
};