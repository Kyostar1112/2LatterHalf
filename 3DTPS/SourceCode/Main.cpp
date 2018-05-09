#include "Main.h"
#include "Global.h"
#include "MyMacro.h"
#include <stdio.h>

//グローバル変数.

//定数.

clsMain*	g_pClsMain = NULL;

//======================================
//	メイン関数.
//======================================
INT WINAPI WinMain(
	HINSTANCE hInstance,	//インスタン番号(ウィンドウの番号)
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	INT nCmdShow)
{
	//g_pClsMain = make_unique<clsMain>();	//初期化&クラスの宣言.
	g_pClsMain = new clsMain;	//初期化&クラスの宣言.

	//クラスが存在してるかチェック.
	if (g_pClsMain != NULL) {
		//ウィンドウ作成成功.
		if (SUCCEEDED(
			g_pClsMain->InitWindow(
				hInstance,
				0, 0,
				WND_W, WND_H,
				WND_TITLE)))
		{
			//Dx11用の初期化.
			if (SUCCEEDED(g_pClsMain->InitD3D()))
			{
				//メッセージループ.
				g_pClsMain->Loop();
			}
		}
		//終了.
		g_pClsMain->DestroyD3D();//Direct3Dの解放.

		SAFE_DELETE(g_pClsMain);
	}
	return 0;
}

//======================================
//	ウィンドウプロシージャ.
//======================================
LRESULT CALLBACK WndProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	//プロシージャ.
	return g_pClsMain->MsgProc(hWnd, uMsg, wParam, lParam);
}

/*************************************************
*	メインクラス.
**/
//======================================
//	コンストラクタ.
//======================================
clsMain::clsMain()
{
	ZeroMemory(this, sizeof(clsMain));

	m_hWnd = NULL;

	m_pDevice = NULL;	//デバイスオブジェクト.
	m_pDeviceContext = NULL;	//デバイスコンテキスト.
	m_pSwapChain = NULL;	//スワップチェーン.

	m_pBackBuffer_TexRTV = NULL;//レンダーターゲットビュー.
	m_pBackBuffer_DSTex = NULL;//バックバッファ.
	m_pBackBuffer_DSTexDSV = NULL;//デプスステンシルビュー.

	//カメラ(視点)位置.
	m_Camera.vEye = D3DXVECTOR3(0.0f, 2.0f, -3.0f);
	//注視位置.
	m_Camera.vLook = D3DXVECTOR3(0.0f, 0.0f, 10.0f);

	//ライト方向.
	m_vLight = D3DXVECTOR3(0.0f, 0.5f, -1.0f);

	m_enScene = Title;

	m_vsSceneName.push_back("GameMode[Title]");
	m_vsSceneName.push_back("GameMode[Stage]");
	m_vsSceneName.push_back("GameMode[Over]");
	m_vsSceneName.push_back("GameMode[Clear]");
	m_vsSceneName.push_back("GameMode[Result]");
}

//======================================
//	デストラクタ.
//======================================
clsMain::~clsMain()
{
}

//======================================
//	ウィンドウ初期化関数.
//======================================
HRESULT clsMain::InitWindow(
	HINSTANCE hInstance,	//インスタンス.
	INT x, INT y,			//ウィンドウのx,y座標.
	INT width, INT height,	//ウィンドウの幅,高さ.
	LPSTR WindowName)		//ウィンドウ名.
{
	//ウィンドウの定義.
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));//初期化(0設定)

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = APR_NAME;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//ウィンドウクラスをWindowsに登録.
	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL,
			"ウィンドウクラスの登録に失敗", "エラー", MB_OK);
		return E_FAIL;
	}

	//ウィンドウの作成.
	m_hWnd = CreateWindow(
		APR_NAME,	//アプリ名.
		WindowName,	//ウィンドウタイトル.
		WS_OVERLAPPEDWINDOW,//ウィンドウ種別(普通)
		0, 0,			//表示位置x,y座標.
		width, height,	//ウィンドウの幅,高さ.
		NULL,			//親ウィンドウハンドル.
		NULL,			//メニュー設定.
		hInstance,		//インスタンス番号.
		NULL);			//ウィンドウ作成時に発生するイベントに渡すデータ.

	if (!m_hWnd) {
		MessageBox(NULL,
			"ウィンドウ作成に失敗", "エラー", MB_OK);
		return E_FAIL;
	}

	//ウィンドウの表示.
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

//======================================
//	ウィンドウ関数(メッセージ毎の処理)
//======================================
LRESULT clsMain::MsgProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY://ウィンドウが破棄された時.
		//アプリケーションの終了をWindowsに通知する.
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN://キーボードが押されたとき.

		//キー別の処理.
		switch ((char)wParam) {
		case VK_ESCAPE:	//ESCキー.
			if (MessageBox(NULL,
				"ゲームを終了しますか？", "警告",
				MB_YESNO) == IDYES)
			{
				//ウィンドウを破棄する.
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	}

	//メインに返す情報.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//================================================
//	メッセージループとアプリケーション処理の入り口.
//================================================
void clsMain::Loop()
{
	//=== DirectInput初期化処理 ===//
	//DxInputオブジェクトの作成.
	m_smpDxInput = make_unique<clsDxInput>();
	m_smpDxInput->initDI(m_hWnd);

	//メッシュ読込まとめたもの.
	MeshRead();

	//デバッグテキストの初期化.
	m_smpText = make_unique<clsDebugText>();
	D3DXVECTOR4 vColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (FAILED(m_smpText->Init(
		m_pDeviceContext,
		WND_W, WND_H, 50.0f,
		vColor)))			//文字色.
	{
		MessageBox(NULL,
			"デバッグテキスト作成失敗", "error", MB_OK);
	}

	//-------------------------------------
	//	フレームレート調節準備.
	//-------------------------------------
	float fRate = 0.0f;	//レート.
	float fFPS = 60.0f;//FPS値.
	DWORD sync_old = timeGetTime();//過去時間.
	DWORD sync_now;					//現在時間.

	//時間処理の為、最小単位を1ミリ秒に変更.
	timeBeginPeriod(1);

	//メッセージループ.
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		Sleep(1);
		sync_now = timeGetTime();//現在時間を取得.

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		fRate = 1000.0f / fFPS;//理想時間を算出.
		if (sync_now - sync_old >= fRate)
		{
			sync_old = sync_now;//現在時間に置き換え.

			//アプリケーションの処理はここから飛ぶ.
			AppMain();
		}
	}
	//アプリケーションの終了.
	timeEndPeriod(1);	//解除.
}

//================================================
//	アプリケーションメイン処理.
//================================================
void clsMain::AppMain()
{
	//コントローラ入力情報更新.
	m_smpDxInput->UpdateInputState();
	float fBlackSpeed = 0.01f;
	if (m_smpSeClick->IsPlaying())
	{
		m_smpSeClick->SeekToStart();
	}
	switch (m_enScene)
	{
	case Title:
	{
		m_smpTitleScene->UpDate();
		if (GetAsyncKeyState('I') & 0x8000) {
			m_bTitleFlg = false;
		}
		if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000 || m_smpDxInput->IsPressKey(enPKey_Down)) {
			m_bTitleFlg = true;
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x0001 || m_smpDxInput->IsPressKey(enPKey_00)) {
			m_smpSeClick->Play();
			m_smpTitleScene->BlackStart(fBlackSpeed);
		}
		if (m_smpTitleScene->GetBlackEnd())
		{
			if (m_bTitleFlg)
			{
				m_enScene = Stage;
			}
			else
			{
				m_enScene = Result;
			}
		}
	}
	break;
	case Stage:
	{
		m_smpStageScene->UpDate();
		if (m_smpStageScene->GetMode() != Stage) {
			m_smpStageScene->BlackStart(fBlackSpeed);
		}
		if (m_smpStageScene->GetBlackEnd())
		{
			m_enScene = m_smpStageScene->GetMode();
		}
		if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000 || m_smpDxInput->IsPressKey(enPKey_Left)) {
			m_smpStageScene->LeftRoll();
		}
		if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000 || m_smpDxInput->IsPressKey(enPKey_Right)) {
			m_smpStageScene->RightRoll();
		}
		if (GetAsyncKeyState('Z') & 0x0001 || GetAsyncKeyState(VK_SPACE) & 0x8000 || m_smpDxInput->IsPressKey(enPKey_00)) {
			m_smpStageScene->Fire();
		}
	}
	break;
	case Over:
	{
		m_smpOverScene->UpDate();
		if (GetAsyncKeyState(VK_SPACE) & 0x0001 || m_smpDxInput->IsPressKey(enPKey_00)) {
			m_smpSeClick->Play();
			m_smpOverScene->BlackStart(fBlackSpeed);
		}
		if (m_smpOverScene->GetBlackEnd())
		{
			m_enScene = Title;
		}
	}
	break;
	case Clear:
	{
		m_smpClearScene->UpDate();
		if (GetAsyncKeyState(VK_SPACE) & 0x0001 || m_smpDxInput->IsPressKey(enPKey_00)) {
			m_smpSeClick->Play();
			m_smpClearScene->BlackStart(fBlackSpeed);
		}
		if (m_smpClearScene->GetBlackEnd())
		{
			m_enScene = Title;
		}
	}
	break;
	case Result:
	{
		m_bTitleFlg = true;
		m_smpResultScene->UpDate();
		if (GetAsyncKeyState(VK_SPACE) & 0x0001 || m_smpDxInput->IsPressKey(enPKey_00)) {
			m_smpSeClick->Play();
			m_smpResultScene->BlackStart(fBlackSpeed);
		}
		if (m_smpResultScene->GetBlackEnd())
		{
			m_enScene = Title;
		}
	}
	break;
	}

	SceneChange();

	//レンダリング.
	Render();
}

//================================================
//	シーン(場面)を画面にレンダリング(描画)
//================================================
void clsMain::Render()
{
	//画面のクリア.
	float ClearColor[4] = { 0.0f, 0.0f, 0.5f, 1.0f };//クリア色(RGBA順)
	//カラーバックバッファ.
	m_pDeviceContext->ClearRenderTargetView(
		m_pBackBuffer_TexRTV, ClearColor);
	//デプスステンシルバックバッファ.
	m_pDeviceContext->ClearDepthStencilView(
		m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);

	//カメラ関数.
	Camera();
	//プロジェクション関数.
	Proj();

	m_pResource->SetModelRender(m_mView, m_mProj, m_vLight, m_Camera.vEye);
	m_pResource->SetSpriteRender(m_hWnd, m_pDevice, m_pDeviceContext);

	switch (m_enScene)
	{
	case Title:
		m_smpTitleScene->ModelRender();

		SetDepth(false);
		m_smpTitleScene->SpriteRender();
		SetDepth(true);
		break;
	case Stage:
		m_smpStageScene->ModelRender1();
		SetDepth(false);
		m_smpStageScene->ExpRender();
		SetDepth(true);
		m_smpStageScene->ModelRender2();
		SetDepth(false);
		m_smpStageScene->SpriteRender();
		SetDepth(true);
		break;
	case Over:
		m_smpOverScene->ModelRender();

		SetDepth(false);
		m_smpOverScene->SpriteRender();
		SetDepth(true);
		break;
	case Clear:
		m_smpClearScene->ModelRender();

		SetDepth(false);
		m_smpClearScene->SpriteRender();
		SetDepth(true);
		break;
	case Result:
		m_smpResultScene->ModelRender();

		SetDepth(false);
		m_smpResultScene->SpriteRender();
		SetDepth(true);
		break;
	}

#ifdef _DEBUG
	//デバッグテキスト.
	if (m_smpText != NULL)//NULLチェック.
	{
		char strDbgTxt[256];
		float DbgY = 10.0f;
		const float CDBGY = 25.0f;

		sprintf_s(strDbgTxt, "Ply:x[%f],y[%f],z[%f]",
			m_smpStageScene->m_smpPlayer->GetPositionX(),
			m_smpStageScene->m_smpPlayer->GetPositionY(),
			m_smpStageScene->m_smpPlayer->GetPositionZ());
		m_smpText->Render(strDbgTxt, 0, DbgY);
		DbgY += CDBGY;

		sprintf_s(strDbgTxt, "PlyRot:y[%f]",
			m_smpStageScene->m_smpPlayer->GetRotationY());
		m_smpText->Render(strDbgTxt, 0, DbgY);
		DbgY += CDBGY;

		sprintf_s(strDbgTxt, "Enemy:x[%f],y[%f],z[%f]",
			m_smpStageScene->m_vsmpEnemy[0]->GetPositionX(),
			m_smpStageScene->m_vsmpEnemy[0]->GetPositionY(),
			m_smpStageScene->m_vsmpEnemy[0]->GetPositionZ());
		m_smpText->Render(strDbgTxt, 0, DbgY);
		DbgY += CDBGY;

		sprintf_s(strDbgTxt, m_vsSceneName[static_cast<int>(m_enScene)].c_str(),
			m_smpStageScene->m_smpPlayer->GetRotationY());
		m_smpText->Render(strDbgTxt, 0, DbgY);
		DbgY += CDBGY;
	}
#endif//#ifdef _DEBUG

	//レンダリングされたイメージを表示.
	m_pSwapChain->Present(0, 0);
}

//================================================
//	DirectX初期化.
//================================================
HRESULT clsMain::InitD3D()
{
	//-------------------------------------
	//	デバイスとスワップチェーン関係.
	//-------------------------------------

	//スワップチェーン構造体.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;	//バックバッファの数.
	sd.BufferDesc.Width = WND_W;//バックバッファの幅.
	sd.BufferDesc.Height = WND_H;//バックバッファの高さ.
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//フォーマット(32ビットカラー)
	sd.BufferDesc.RefreshRate.Numerator = 60;
	//リフレッシュレート(分母) ※FPS:60
	sd.BufferDesc.RefreshRate.Denominator = 1;
	//リフレッシュレート(分子)
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//使い方(表示先)
	sd.OutputWindow = m_hWnd;	//ウィンドウハンドル.
	sd.SampleDesc.Count = 1;	//マルチサンプルの数.
	sd.SampleDesc.Quality = 0;	//マルチサンプルのクオリティ.
	sd.Windowed = TRUE;	//ウィンドウモード(フルスク時はFALSE)

	//作成を試みる機能レベルの優先を指定.
	//	(GPUがサポートする機能セットの定義)
	//	D3D_FEATURE_LEVEL列挙体の配列.
	//	D3D_FEATURE_LEVEL_10_1:Direct3D 10.1のGPUレベル.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;//配列の要素数.

	//デバイスとスワップチェーンの作成.
	//	ハードウェア(GPU)デバイスで作成.
	if (FAILED(
		D3D11CreateDeviceAndSwapChain(
			NULL,			//ビデオアダプタへのポインタ.
			D3D_DRIVER_TYPE_HARDWARE,//作成するデバイスの種類.
			NULL,			//ソフトウェア ラスタライザーを実装するDLLのハンドル.
			0,				//有効にするランタイムレイヤー.
			&pFeatureLevels,//作成を試みる機能レベルの順序を指定する配列へのポインタ.
			1,				//↑の要素数.
			D3D11_SDK_VERSION,//SDKのバージョン.
			&sd,			//スワップチェーンの初期化パラメータのポインタ.
			&m_pSwapChain,	//(out)レンダリングに使用するスワップチェーン.
			&m_pDevice,		//(out)作成されたデバイス.
			pFeatureLevel,	//機能レベルの配列にある最初の要素を表すポインタ.
			&m_pDeviceContext)))//(out)デバイス コンテキスト.
	{
		//WARPデバイスの作成.
		//	D3D_FEATURE_LEVEL_9_1 ～ D3D_FEATURE_LEVEL_10_1
		if (FAILED(
			D3D11CreateDeviceAndSwapChain(
				NULL, D3D_DRIVER_TYPE_WARP,
				NULL, 0, &pFeatureLevels, 1,
				D3D11_SDK_VERSION, &sd, &m_pSwapChain,
				&m_pDevice, pFeatureLevel,
				&m_pDeviceContext)))
		{
			//リファレンスデバイスの作成.
			//	DirectX SDKがインストールされていないと使えない.
			if (FAILED(
				D3D11CreateDeviceAndSwapChain(
					NULL, D3D_DRIVER_TYPE_REFERENCE,
					NULL, 0, &pFeatureLevels, 1,
					D3D11_SDK_VERSION, &sd, &m_pSwapChain,
					&m_pDevice, pFeatureLevel,
					&m_pDeviceContext)))
			{
				MessageBox(NULL,
					"デバイスとスワップチェーンの作成に失敗",
					"error(main.cpp)", MB_OK);
				return E_FAIL;
			}
		}
	}

	//各種テクスチャ-と、それに付帯する各種ビュー(画面)を作成.

	//-----------------------------------------------
	//	バックバッファ準備:カラーバッファ設定.
	//-----------------------------------------------

	//バックバッファテクスチャ-を取得(既にあるので作成ではない)
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),	//__uuidof:式に関連付けされたGUIDを取得.
		//         Texture2Dの唯一の物として扱う.
		(LPVOID*)&pBackBuffer_Tex);	//(out)バックバッファテクスチャ-.

	//そのテクスチャに対しレンダーターゲットビュー(RTV)を作成.
	m_pDevice->CreateRenderTargetView(
		pBackBuffer_Tex,
		NULL,
		&m_pBackBuffer_TexRTV);//(out)RTV.
	//バックバッファテクスチャ-を解放.
	SAFE_RELEASE(pBackBuffer_Tex);

	//-----------------------------------------------
	//	バックバッファ準備:デプス(深度)ステンシル関係.
	//-----------------------------------------------

	//デプス(深さor深度)ステンシルビュー用のテクスチャ-を作成.
	D3D11_TEXTURE2D_DESC descDepth;

	descDepth.Width = WND_W;//幅.
	descDepth.Height = WND_H;//高さ.
	descDepth.MipLevels = 1;//ミップマップレベル:1.
	descDepth.ArraySize = 1;//配列数:1.
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;//32ビットフォーマット.
	descDepth.SampleDesc.Count = 1;//マルチサンプルの数.
	descDepth.SampleDesc.Quality = 0;//マルチサンプルのクオリティ.
	descDepth.Usage = D3D11_USAGE_DEFAULT;//使用方法:デフォルト.
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;//深度(ステンシルとして使用)
	descDepth.CPUAccessFlags = 0;//CPUからはアクセスしない.
	descDepth.MiscFlags = 0;//その他の設定なし.

	m_pDevice->CreateTexture2D(
		&descDepth,
		NULL,
		&m_pBackBuffer_DSTex);//(out)デプスステンシル用テクスチャ.

	//そのテクスチャに対しデプスステンシルビュー(DSV)を作成.
	m_pDevice->CreateDepthStencilView(
		m_pBackBuffer_DSTex,
		NULL,
		&m_pBackBuffer_DSTexDSV);//(out)DSV.

	//レンダーターゲットビューとデプスステンシルビューをパイプラインにセット.
	m_pDeviceContext->OMSetRenderTargets(
		1,
		&m_pBackBuffer_TexRTV,
		m_pBackBuffer_DSTexDSV);

	//深度テスト(Zテスト)を有効にする.
	D3D11_DEPTH_STENCIL_DESC	depthStencilDesc;
	ZeroMemory(&depthStencilDesc,
		sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = true;

	if (SUCCEEDED(m_pDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilState)))
	{
		m_pDeviceContext->OMSetDepthStencilState(
			m_pDepthStencilState, 1);
	}

	//ビューポートの設定.
	D3D11_VIEWPORT vp;
	vp.Width = WND_W;	//幅.
	vp.Height = WND_H;	//高さ.
	vp.MinDepth = 0.0f;		//最小深度(手前)
	vp.MaxDepth = 1.0f;		//最大深度(奥)
	vp.TopLeftX = 0.0f;		//左上位置x.
	vp.TopLeftY = 0.0f;		//左上位置y.
	m_pDeviceContext->RSSetViewports(1, &vp);

	//ラスタライズ(面の塗りつぶし方)設定.
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode = D3D11_FILL_SOLID;//塗りつぶし(ソリッド)
	rdc.CullMode = D3D11_CULL_NONE;
	//D3D11_CULL_NONE :カリングを切る(正背面を描画する)
	//D3D11_CULL_BACK :背面を描画しない.
	//D3D11_CULL_FRONT:正面を描画しない.
	rdc.FrontCounterClockwise = FALSE;
	//ポリゴンの表裏を決定するフラグ.
	//TRUE :左回りなら前向き.右回りなら後ろ向き.
	//FALSE:↑の逆になる.
	rdc.DepthClipEnable = FALSE;	//距離についてのクリッピング有効.

	ID3D11RasterizerState* pIr = NULL;
	m_pDevice->CreateRasterizerState(&rdc, &pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);

	return S_OK;
}

//======================================
//	Direct3D終了処理.
//======================================
void clsMain::DestroyD3D()
{
	//シーン.
	SAFE_RELEASE(m_smpTitleScene);
	SAFE_RELEASE(m_smpStageScene);
	SAFE_RELEASE(m_smpOverScene);
	SAFE_RELEASE(m_smpClearScene);

	//Direct3Dオブジェクトを解放.
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

#if 0
//スフィア作成.
HRESULT clsMain::InitSphere(clsDX9Mesh* pMesh, float fScale)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;	//頂点バッファ.
	void*		pVertices = NULL;	//頂点
	D3DXVECTOR3 vCenter;		//中心.
	float		fRadius;			//半径.

	//頂点バッファを取得.
	if (FAILED(pMesh->m_pMesh->GetVertexBuffer(&pVB)))
	{
		return E_FAIL;
	}

	//メッシュの頂点バッファをロックする.
	if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
	{
		SAFE_RELEASE(pVB);
		return E_FAIL;
	}

	//メッシュの外接円の中心と半径を計算する.
	D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)pVertices,
		pMesh->m_pMesh->GetNumVertices(),//頂点の数.
		D3DXGetFVFVertexSize(pMesh->m_pMesh->GetFVF()),//頂点の情報.
		&vCenter,	//(out)中心座標.
		&fRadius);	//(out)半径.

	//アンロック.
	pVB->Unlock();
	SAFE_RELEASE(pVB);

	//中心と半径を構造体に設定.
	pMesh->m_Sphere.vCenter = vCenter;
	pMesh->m_Sphere.fRadius = fRadius * fScale;

	return S_OK;
}

#endif // 0

//メッシュ読込関数（まとめた）
HRESULT clsMain::MeshRead()
{
	//リソースクラス.
	m_pResource = clsResource::GetInstance();
	m_pResource->Init(m_hWnd, m_pDevice, m_pDeviceContext);

	m_bLoadFlg = false;

	m_smpLoadString = make_unique<clsSprite2D>();
	m_smpLoadString->Create(m_pDevice, m_pDeviceContext, "Data\\Load\\LoadBack.png");
	m_smpLoadString->SetPos(D3DXVECTOR3(WND_W / 2 - m_smpLoadString->GetSs().Disp.w / 2,
		WND_H / 2 - m_smpLoadString->GetSs().Disp.h / 2,
		0.0f));
	m_smpLoadString->SetPatarnU(0.0f);
	m_smpLoadString->SetPatarnV(0.0f);
	m_smpLoadString->SetAlpha(1.0f);

	m_smpLoadCircle = make_unique<clsSprite2D>();
	m_smpLoadCircle->Create(m_pDevice, m_pDeviceContext, "Data\\Load\\LoadIcon.png");
	m_smpLoadCircle->SetPos(D3DXVECTOR3(m_smpLoadString->GetPos().x + m_smpLoadString->GetSs().Disp.w,
		m_smpLoadString->GetPos().y,
		0.0f));
	m_smpLoadCircle->SetPatarnU(0.0f);
	m_smpLoadCircle->SetPatarnV(0.0f);
	m_smpLoadCircle->SetAlpha(1.0f);


	thread th1([this] {
		while (true)
		{
			//画面のクリア.
			float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };//クリア色(RGBA順:255の比率で出す)
															 //カラーバックバッファ.
			m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer_TexRTV, ClearColor);
			//デプスステンシルバックバッファ.
			m_pDeviceContext->ClearDepthStencilView(m_pBackBuffer_DSTexDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			SetDepth(false);

			m_smpLoadString->Render();

			m_smpLoadString->Flashing(0.0005f);
			m_smpLoadCircle->Render();

			SetDepth(true);

			//レンダリングされたイメージを表示.
			m_pSwapChain->Present(0, 0);

			if (m_bLoadFlg)
			{
				break;
			}

		}
	});

	th1.detach();

	m_pResource->CreateStaticModel(
		"Data\\teststage\\test_stage_X.X",
		clsResource::enStaticModel_Plane);
	m_pResource->CreateStaticModel(
		"Data\\Player\\Ziki.X",
		clsResource::enStaticModel_Player);
	m_pResource->CreateStaticModel(
		"Data\\Player\\Ziki.X",
		clsResource::enStaticModel_Shot);
	m_pResource->CreateStaticModel(
		"Data\\Collision\\Sphere.X",
		clsResource::enStaticModel_Sphere);
	m_pResource->CreateSkinModel(
		"Data\\EXTINGER\\extinger.X",
		clsResource::enSkinModel_Boss);

	Resource->m_smpFile = make_unique<clsFile>();
	Resource->m_smpFile->Init("Data\\Txt\\ScoreRank.csv");
	Resource->m_smpFile->Read();
	for (int i = 0; i < Resource->m_smpFile->GetDataArrayNumMax(); i++)
	{
		Resource->m_viScore.push_back(Resource->m_smpFile->GetDataArray(i));
	}

	m_bTitleFlg = true;
	m_smpTitleScene = make_unique<clsTitleScene>();
	m_smpStageScene = make_unique<clsStageScene>();
	m_smpOverScene = make_unique<clsOverScene>();
	m_smpClearScene = make_unique<clsClearScene>();
	m_smpResultScene = make_unique<clsResultScene>();
	m_pResource->SetModelRender(m_mView, m_mProj, m_vLight, m_Camera.vEye);
	m_pResource->SetSpriteRender(m_hWnd, m_pDevice, m_pDeviceContext);

	m_smpTitleScene->Create();
	m_smpStageScene->Create();
	m_smpOverScene->Create();
	m_smpClearScene->Create();
	m_smpResultScene->Create();

	m_smpTitleScene->Init();
	m_smpStageScene->Init();
	m_smpOverScene->Init();
	m_smpResultScene->Init();
	m_smpClearScene->Init();

	m_smpSeClick = make_unique<clsSound>();
	m_smpSeClick->Open("Data\\Sound\\BGM\\Click.mp3", "ClickSe00", Resource->GetSpriteRenderSet().hWnd);
	m_smpSeClick->SetVolume(100);

	m_smpClearScene->MusicStop();
	m_smpTitleScene->MusicStop();
	m_smpStageScene->MusicStop();
	m_smpResultScene->MusicStop();
	m_smpOverScene->MusicStop();

	m_bLoadFlg = true;

	return S_OK;
}

//深度テスト(Zテスト) ON/OFF切替.
void clsMain::SetDepth(bool bFlg)
{
	//深度テスト(Zテスト)を有効にする.
	D3D11_DEPTH_STENCIL_DESC	depthStencilDesc;
	ZeroMemory(&depthStencilDesc,
		sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = bFlg;

	m_pDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilState);

	m_pDeviceContext->OMSetDepthStencilState(
		m_pDepthStencilState, 1);
}

//レイとメッシュの当たり判定.
bool clsMain::Intersect(
	clsGameObject* pAttacker,	//基準の物体.
	clsCharacter*  pTarget,		//対象の物体.
	float* pfDistance,			//(out)距離.
	D3DXVECTOR3* pvIntersect)	//(out)交差座標.
{
	D3DXMATRIXA16 matRot;	//回転行列.

	//回転行列を計算.
	D3DXMatrixRotationY(&matRot, pAttacker->GetRotationY());

	//軸ベクトルを用意.
	D3DXVECTOR3 vecAxisZ;
	//Z軸ベクトルそのものを現在の回転状態により変換する.
	D3DXVec3TransformCoord(
		&vecAxisZ, &pAttacker->m_vAxis, &matRot);

	D3DXVECTOR3 vecStart, vecEnd, vecDistance;
	//レイの開始終了位置をAttackerと合わせる.
	vecStart = vecEnd = pAttacker->m_vRay;
	//Attackerの座標に回転座標を合成する.
	vecEnd += vecAxisZ * 1.0f;

	//対象が動いている物体でも、対象のworld行列の、
	//逆行列を用いれば、正しくレイが当たる.
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(
		&matWorld,
		pTarget->GetPositionX(),
		pTarget->GetPositionY(),
		pTarget->GetPositionZ());

	//逆行列を求める.
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformCoord(
		&vecStart, &vecStart, &matWorld);
	D3DXVec3TransformCoord(
		&vecEnd, &vecEnd, &matWorld);

	//距離を求める.
	vecDistance = vecEnd - vecStart;

	BOOL bHit = false;	//命中フラグ.

	DWORD dwIndex = 0;		//インデックス番号.
	D3DXVECTOR3 vVertex[3];	//頂点座標.
	FLOAT U = 0, V = 0;			//(out)重心ヒット座標.

	//メッシュとレイの交差を調べる.
	D3DXIntersect(
		pTarget->GetMesh(),	//対象メッシュ.
		&vecStart,			//開始位置.
		&vecDistance,		//レイの距離.
		&bHit,				//(out)判定結果.
		&dwIndex,	//(out)bHitがTrue時、レイの始点に.
		//最も近くの面のインデックス値へのポインタ.
		&U, &V,				//(out)重心ヒット座標.
		pfDistance,			//ターゲットとの距離.
		NULL, NULL);
	if (bHit) {
		//命中したとき.
		FindVerticesOnPoly(
			pTarget->GetMeshForRay(), dwIndex, vVertex);
		//重心座標から交差点を算出.
		//ローカル交点pは、v0 + U*(v1-v0) + V*(v2-v0)で求まる.
		*pvIntersect =
			vVertex[0]
			+ U * (vVertex[1] - vVertex[0])
			+ V * (vVertex[2] - vVertex[0]);

		return true;//命中している.
	}
	return false;//外れている.
}

//交差位置のポリゴンの頂点を見つける.
//※頂点座標はローカル座標.
HRESULT clsMain::FindVerticesOnPoly(
	LPD3DXMESH pTarget, DWORD dwPolyIndex,
	D3DXVECTOR3* pVecVertices)
{
	//頂点ごとのバイト数を取得.
	DWORD dwStride = pTarget->GetNumBytesPerVertex();
	//頂点数を取得.
	DWORD dwVertexAmt = pTarget->GetNumVertices();
	//面数を取得.
	DWORD dwPolyAmt = pTarget->GetNumFaces();

	WORD* pwPoly = NULL;

	//インデックスバッファをロック(読込モード)
	pTarget->LockIndexBuffer(
		D3DLOCK_READONLY, (VOID**)&pwPoly);
	BYTE* pbVertices = NULL;//頂点(バイト型)
	FLOAT* pfVertices = NULL;//頂点(float型)
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;//頂点バッファ.
	pTarget->GetVertexBuffer(&VB);//頂点情報の取得.

	//頂点バッファのロック.
	if (SUCCEEDED(
		VB->Lock(0, 0, (VOID**)&pbVertices, 0)))
	{
		//ポリゴンの頂点の１つ目を取得.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3]];
		pVecVertices[0].x = pfVertices[0];
		pVecVertices[0].y = pfVertices[1];
		pVecVertices[0].z = pfVertices[2];

		//ポリゴンの頂点の２つ目を取得.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 1]];
		pVecVertices[1].x = pfVertices[0];
		pVecVertices[1].y = pfVertices[1];
		pVecVertices[1].z = pfVertices[2];

		//ポリゴンの頂点の３つ目を取得.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 2]];
		pVecVertices[2].x = pfVertices[0];
		pVecVertices[2].y = pfVertices[1];
		pVecVertices[2].z = pfVertices[2];

		pTarget->UnlockIndexBuffer();	//ロック解除.
		VB->Unlock();	//ロック解除.
	}
	VB->Release();

	return S_OK;
}

//壁の当たり判定関連.
void clsMain::WallJudge(clsGameObject* pAttacker, clsCharacter* pWall)
{
	FLOAT		fDistance[4];	//距離.
	D3DXVECTOR3	vIntersect[4];	//交点座標.
	float		fDis, fYaw;		//距離と回転.

	pAttacker->m_vRay = pAttacker->GetPosition();
	pAttacker->m_vRay.y = 1.0f;	//レイのy位置.

	//レイの向きにより当たる壁までの距離を求める.
	//軸ベクトル前.
	pAttacker->m_vAxis = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	Intersect(pAttacker, pWall, &fDistance[0], &vIntersect[0]);
	//軸ベクトル後ろ.
	pAttacker->m_vAxis = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	Intersect(pAttacker, pWall, &fDistance[1], &vIntersect[1]);
	//軸ベクトル右.
	pAttacker->m_vAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	Intersect(pAttacker, pWall, &fDistance[2], &vIntersect[2]);
	//軸ベクトル左.
	pAttacker->m_vAxis = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	Intersect(pAttacker, pWall, &fDistance[3], &vIntersect[3]);

	fYaw = fabs(pAttacker->GetRotationY());//fabs:絶対値(float版)
	dirOverGuard(&fYaw);	//0～2πの間に収める.

	//前が壁に接近.
	fDis = fDistance[0];
	if (fDis < WSPACE && fDis > 0.01f) {
		if (pAttacker->GetRotationY() < 0.0f) {
			//時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f) {		//右から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f) {	//前から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f) {	//左から.
				pAttacker->SetPositionX(-WSPACE - fDis);
			}
			else {										//奥から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
		}
		else {
			//反時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f) {		//右から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f) {	//前から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f) {	//左から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else {										//奥から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
		}
	}

	//後ろが壁に接近.
	fDis = fDistance[1];
	if (fDis < WSPACE && fDis > 0.01f) {
		if (pAttacker->GetRotationY() < 0.0f) {
			//時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f) {		//右から.
				pAttacker->SetPositionX(-WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f) {	//前から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f) {	//左から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else {										//奥から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
		}
		else {
			//反時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f) {		//右から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f) {	//前から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f) {	//左から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else {										//奥から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
		}
	}

	//右が壁に接近.
	fDis = fDistance[2];
	if (fDis < WSPACE && fDis > 0.01f) {
		if (pAttacker->GetRotationY() < 0.0f) {
			//時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f) {		//右から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f) {	//前から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f) {	//左から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
			else {										//奥から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
		}
		else {
			//反時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f) {		//右から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f) {	//前から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f) {	//左から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
			else {										//奥から.
				pAttacker->SetPositionX(-WSPACE - fDis);
			}
		}
	}

	//左が壁に接近.
	fDis = fDistance[3];
	if (fDis < WSPACE && fDis > 0.01f) {
		if (pAttacker->GetRotationY() < 0.0f) {
			//時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f) {		//右から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f) {	//前から.
				pAttacker->SetPositionX(-WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f) {	//左から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
			else {										//奥から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
		}
		else {
			//反時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f) {		//右から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f) {	//前から.
				pAttacker->SetPositionX(-WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f) {	//左から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
			else {										//奥から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
		}
	}
}

//回転値調整.
void clsMain::dirOverGuard(float* fYaw)
{
	if (*fYaw > D3DX_PI * 2.0f) {
		//１周以上している.
		*fYaw -= D3DX_PI * 2.0f;	//2π(360°)分を引く.
	}

	//再帰関数.
	if (*fYaw > D3DX_PI * 2.0f) {
		dirOverGuard(fYaw);
	}
}

//カメラ関数.
void clsMain::Camera()
{
	//軸ベクトルを用意.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);
	switch (m_enScene)
	{
	case Title:
		m_Camera.vEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Camera.vLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;
	case Stage:
		//===============================================
		//カメラ追従処理 ここから.
		//===============================================
		//カメラ位置(自機の背中から)の設定.
		m_Camera.vEye = m_Camera.vLook
			= m_smpStageScene->m_smpPlayer->GetPosition();//自機の位置をコピー.
		m_Camera.fYaw = m_smpStageScene->m_smpPlayer->GetRotationY();//回転値をコピー.

		//Y軸回転行列の作成.
		D3DXMatrixRotationY(
			&m_Camera.mRot, m_Camera.fYaw);

		//Z軸ベクトルそのものを回転状態により変換する.
		D3DXVec3TransformCoord(
			&vecAxisZ, &vecAxisZ, &m_Camera.mRot);

		m_Camera.vEye -= vecAxisZ/* * 4.0f*/;	//自機の背中側.
		m_Camera.vLook += vecAxisZ/* * 2.0f*/;	//自機の前側.

		m_Camera.vEye.y += 1.0f;	//カメラ位置(高さ)調整.
		m_Camera.vLook.y += 1.0f;	//注視位置(高さ)調整.

		//===============================================
		//カメラ追従処理 ここまで.
		//===============================================
		break;
	case Over:
		break;
	case Clear:
		break;
	}
	//ビュー(カメラ)変換.
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ビュー計算結果.
		&m_Camera.vEye, &m_Camera.vLook, &vUpVec);
}

//プロジェクション関数.
void clsMain::Proj()
{
	//プロジェクション(射影行列)変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,	//(out)プロジェクション計算結果.
		D3DX_PI / 4.0, //y方向の視野(ラジアン指定)数値を大きくしたら視野が狭くなる.
		(FLOAT)WND_W / (FLOAT)WND_H,//アスペクト比(幅÷高さ)
		0.1f,		//近いビュー平面のz値.
		100.0f);	//遠いビュー平面のz値.
}
void clsMain::SceneChange()
{
	if (m_enOldScene != m_enScene)
	{
		switch (m_enScene)
		{
		case Title:
			m_smpClearScene->MusicStop();
			m_smpOverScene->MusicStop();
			m_smpResultScene->MusicStop();
			m_smpTitleScene->Init();
			m_enOldScene = m_enScene;
			break;
		case Stage:
			m_smpTitleScene->MusicStop();
			m_smpStageScene->Init();
			m_enOldScene = m_enScene;
			break;
		case Over:
			m_smpStageScene->MusicStop();
			m_smpOverScene->Init();
			m_enOldScene = m_enScene;
			break;
		case Clear:
			m_smpStageScene->MusicStop();
			m_smpOverScene->MusicStop();
			m_smpClearScene->Init();
			m_enOldScene = m_enScene;
			break;
		case Result:
			m_smpTitleScene->MusicStop();
			m_smpResultScene->Init();
			m_enOldScene = m_enScene;
			break;
		}
	}
}