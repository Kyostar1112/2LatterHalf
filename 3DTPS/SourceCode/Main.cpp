#include "Main.h"
#include "Global.h"
#include "MyMacro.h"
#include <stdio.h>

//ｸﾞﾛｰﾊﾞﾙ変数.

//定数.

clsMain*	g_pClsMain = NULL;

//======================================
//	ﾒｲﾝ関数.
//======================================
INT WINAPI WinMain(
	HINSTANCE hInstance,	//ｲﾝｽﾀﾝ番号(ｳｨﾝﾄﾞｳの番号)
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	INT nCmdShow)
{
	//g_pClsMain = make_unique<clsMain>();	//初期化&ｸﾗｽの宣言.
	g_pClsMain = new clsMain;	//初期化&ｸﾗｽの宣言.

	//ｸﾗｽが存在してるかﾁｪｯｸ.
	if (g_pClsMain != NULL){
		//ｳｨﾝﾄﾞｳ作成成功.
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
				//ﾒｯｾｰｼﾞﾙｰﾌﾟ.
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
//	ｳｨﾝﾄﾞｳﾌﾟﾛｼｰｼﾞｬ.
//======================================
LRESULT CALLBACK WndProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	//ﾌﾟﾛｼｰｼﾞｬ.
	return g_pClsMain->MsgProc(hWnd, uMsg, wParam, lParam);
}

/*************************************************
*	ﾒｲﾝｸﾗｽ.
**/
//======================================
//	ｺﾝｽﾄﾗｸﾀ.
//======================================
clsMain::clsMain()
{
	ZeroMemory(this, sizeof(clsMain));

	m_hWnd = NULL;

	m_pDevice = NULL;	//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	m_pDeviceContext = NULL;	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
	m_pSwapChain = NULL;	//ｽﾜｯﾌﾟﾁｪｰﾝ.

	m_pBackBuffer_TexRTV = NULL;//ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰ.
	m_pBackBuffer_DSTex = NULL;//ﾊﾞｯｸﾊﾞｯﾌｧ.
	m_pBackBuffer_DSTexDSV = NULL;//ﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰ.

	//ｶﾒﾗ(視点)位置.
	m_Camera.vEye = D3DXVECTOR3(0.0f, 2.0f, -3.0f);
	//注視位置.
	m_Camera.vLook = D3DXVECTOR3(0.0f, 0.0f, 10.0f);

	//ﾗｲﾄ方向.
	m_vLight = D3DXVECTOR3(0.0f, 0.5f, -1.0f);

	m_enScene = Title;

	m_vsSceneName.push_back("GameMode[Title]");
	m_vsSceneName.push_back("GameMode[Stage]");
	m_vsSceneName.push_back("GameMode[Over]");
	m_vsSceneName.push_back("GameMode[Clear]");
	m_vsSceneName.push_back("GameMode[Result]");
}

//======================================
//	ﾃﾞｽﾄﾗｸﾀ.
//======================================
clsMain::~clsMain()
{
}

//======================================
//	ｳｨﾝﾄﾞｳ初期化関数.
//======================================
HRESULT clsMain::InitWindow(
	HINSTANCE hInstance,	//ｲﾝｽﾀﾝｽ.
	INT x, INT y,			//ｳｨﾝﾄﾞｳのx,y座標.
	INT width, INT height,	//ｳｨﾝﾄﾞｳの幅,高さ.
	LPSTR WindowName)		//ｳｨﾝﾄﾞｳ名.
{
	//ｳｨﾝﾄﾞｳの定義.
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

	//ｳｨﾝﾄﾞｳｸﾗｽをWindowsに登録.
	if (!RegisterClassEx(&wc)){
		MessageBox(NULL,
			"ｳｨﾝﾄﾞｳｸﾗｽの登録に失敗", "ｴﾗｰ", MB_OK);
		return E_FAIL;
	}

	//ｳｨﾝﾄﾞｳの作成.
	m_hWnd = CreateWindow(
		APR_NAME,	//ｱﾌﾟﾘ名.
		WindowName,	//ｳｨﾝﾄﾞｳﾀｲﾄﾙ.
		WS_OVERLAPPEDWINDOW,//ｳｨﾝﾄﾞｳ種別(普通)
		0, 0,			//表示位置x,y座標.
		width, height,	//ｳｨﾝﾄﾞｳの幅,高さ.
		NULL,			//親ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
		NULL,			//ﾒﾆｭｰ設定.
		hInstance,		//ｲﾝｽﾀﾝｽ番号.
		NULL);			//ｳｨﾝﾄﾞｳ作成時に発生するｲﾍﾞﾝﾄに渡すﾃﾞｰﾀ.

	if (!m_hWnd){
		MessageBox(NULL,
			"ｳｨﾝﾄﾞｳ作成に失敗", "ｴﾗｰ", MB_OK);
		return E_FAIL;
	}

	//ｳｨﾝﾄﾞｳの表示.
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

//======================================
//	ｳｨﾝﾄﾞｳ関数(ﾒｯｾｰｼﾞ毎の処理)
//======================================
LRESULT clsMain::MsgProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	switch (uMsg){
		case WM_DESTROY://ｳｨﾝﾄﾞｳが破棄された時.
			//ｱﾌﾟﾘｹｰｼｮﾝの終了をWindowsに通知する.
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN://ｷｰﾎﾞｰﾄﾞが押されたとき.

			//ｷｰ別の処理.
			switch ((char)wParam){
				case VK_ESCAPE:	//ESCｷｰ.
					if (MessageBox(NULL,
						"ゲームを終了しますか？", "警告",
						MB_YESNO) == IDYES)
					{
						//ｳｨﾝﾄﾞｳを破棄する.
						DestroyWindow(hWnd);
					}
					break;
			}
			break;
	}

	//ﾒｲﾝに返す情報.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//================================================
//	ﾒｯｾｰｼﾞﾙｰﾌﾟとｱﾌﾟﾘｹｰｼｮﾝ処理の入り口.
//================================================
void clsMain::Loop()
{
	//=== DirectInput初期化処理 ===//
	//DxInputｵﾌﾞｼﾞｪｸﾄの作成.
	m_smpDxInput = make_unique<clsDxInput>();
	m_smpDxInput->initDI(m_hWnd);

	//ﾒｯｼｭ読込まとめたもの.
	MeshRead();

	//ﾃﾞﾊﾞｯｸﾞﾃｷｽﾄの初期化.
	m_smpText = make_unique<clsDebugText>();
	D3DXVECTOR4 vColor(1.0f, 1.0f, 1.0f, 1.0f);
	if (FAILED(m_smpText->Init(
		m_pDeviceContext,
		WND_W, WND_H, 50.0f,
		vColor)))			//文字色.
	{
		MessageBox(NULL,
			"ﾃﾞﾊﾞｯｸﾞﾃｷｽﾄ作成失敗", "error", MB_OK);
	}

	//-------------------------------------
	//	ﾌﾚｰﾑﾚｰﾄ調節準備.
	//-------------------------------------
	float fRate = 0.0f;	//ﾚｰﾄ.
	float fFPS = 60.0f;//FPS値.
	DWORD sync_old = timeGetTime();//過去時間.
	DWORD sync_now;					//現在時間.

	//時間処理の為、最小単位を1ﾐﾘ秒に変更.
	timeBeginPeriod(1);

	//ﾒｯｾｰｼﾞﾙｰﾌﾟ.
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

			//ｱﾌﾟﾘｹｰｼｮﾝの処理はここから飛ぶ.
			AppMain();
		}
	}
	//ｱﾌﾟﾘｹｰｼｮﾝの終了.
	timeEndPeriod(1);	//解除.
}

//================================================
//	ｱﾌﾟﾘｹｰｼｮﾝﾒｲﾝ処理.
//================================================
void clsMain::AppMain()
{
	//ｺﾝﾄﾛｰﾗ入力情報更新.
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
			if (GetAsyncKeyState('I') & 0x8000){
				m_bTitleFlg = false;
			}
			if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000 || m_smpDxInput->IsPressKey(enPKey_Down)){
				m_bTitleFlg = true;
			}
			if (GetAsyncKeyState(VK_SPACE) & 0x0001 || m_smpDxInput->IsPressKey(enPKey_00)){
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
			if (m_smpStageScene->GetMode() != Stage){
				m_smpStageScene->BlackStart(fBlackSpeed);
			}
			if (m_smpStageScene->GetBlackEnd())
			{
				m_enScene = m_smpStageScene->GetMode();
			}
			if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000 || m_smpDxInput->IsPressKey(enPKey_Left)){
				m_smpStageScene->LeftRoll();
			}
			if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000 || m_smpDxInput->IsPressKey(enPKey_Right)){
				m_smpStageScene->RightRoll();
			}
			if (GetAsyncKeyState('Z') & 0x0001 || GetAsyncKeyState(VK_SPACE) & 0x8000 || m_smpDxInput->IsPressKey(enPKey_00)){
				m_smpStageScene->Fire();
			}
		}
		break;
		case Over:
		{
			m_smpOverScene->UpDate();
			if (GetAsyncKeyState(VK_SPACE) & 0x0001 || m_smpDxInput->IsPressKey(enPKey_00)){
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
			if (GetAsyncKeyState(VK_SPACE) & 0x0001 || m_smpDxInput->IsPressKey(enPKey_00)){
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
			if (GetAsyncKeyState(VK_SPACE) & 0x0001 || m_smpDxInput->IsPressKey(enPKey_00)){
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

	//ﾚﾝﾀﾞﾘﾝｸﾞ.
	Render();
}

//================================================
//	ｼｰﾝ(場面)を画面にﾚﾝﾀﾞﾘﾝｸﾞ(描画)
//================================================
void clsMain::Render()
{
	//画面のｸﾘｱ.
	float ClearColor[4] = { 0.0f, 0.0f, 0.5f, 1.0f };//ｸﾘｱ色(RGBA順)
	//ｶﾗｰﾊﾞｯｸﾊﾞｯﾌｧ.
	m_pDeviceContext->ClearRenderTargetView(
		m_pBackBuffer_TexRTV, ClearColor);
	//ﾃﾞﾌﾟｽｽﾃﾝｼﾙﾊﾞｯｸﾊﾞｯﾌｧ.
	m_pDeviceContext->ClearDepthStencilView(
		m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0);

	//ｶﾒﾗ関数.
	Camera();
	//ﾌﾟﾛｼﾞｪｸｼｮﾝ関数.
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
	//ﾃﾞﾊﾞｯｸﾞﾃｷｽﾄ.
	if (m_smpText != NULL)//NULLﾁｪｯｸ.
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

	//ﾚﾝﾀﾞﾘﾝｸﾞされたｲﾒｰｼﾞを表示.
	m_pSwapChain->Present(0, 0);
}

//================================================
//	DirectX初期化.
//================================================
HRESULT clsMain::InitD3D()
{
	//-------------------------------------
	//	ﾃﾞﾊﾞｲｽとｽﾜｯﾌﾟﾁｪｰﾝ関係.
	//-------------------------------------

	//ｽﾜｯﾌﾟﾁｪｰﾝ構造体.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;	//ﾊﾞｯｸﾊﾞｯﾌｧの数.
	sd.BufferDesc.Width = WND_W;//ﾊﾞｯｸﾊﾞｯﾌｧの幅.
	sd.BufferDesc.Height = WND_H;//ﾊﾞｯｸﾊﾞｯﾌｧの高さ.
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//ﾌｫｰﾏｯﾄ(32ﾋﾞｯﾄｶﾗｰ)
	sd.BufferDesc.RefreshRate.Numerator = 60;
	//ﾘﾌﾚｯｼｭﾚｰﾄ(分母) ※FPS:60
	sd.BufferDesc.RefreshRate.Denominator = 1;
	//ﾘﾌﾚｯｼｭﾚｰﾄ(分子)
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//使い方(表示先)
	sd.OutputWindow = m_hWnd;	//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.
	sd.SampleDesc.Count = 1;	//ﾏﾙﾁｻﾝﾌﾟﾙの数.
	sd.SampleDesc.Quality = 0;	//ﾏﾙﾁｻﾝﾌﾟﾙのｸｵﾘﾃｨ.
	sd.Windowed = TRUE;	//ｳｨﾝﾄﾞｳﾓｰﾄﾞ(ﾌﾙｽｸ時はFALSE)

	//作成を試みる機能ﾚﾍﾞﾙの優先を指定.
	//	(GPUがｻﾎﾟｰﾄする機能ｾｯﾄの定義)
	//	D3D_FEATURE_LEVEL列挙体の配列.
	//	D3D_FEATURE_LEVEL_10_1:Direct3D 10.1のGPUﾚﾍﾞﾙ.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;//配列の要素数.

	//ﾃﾞﾊﾞｲｽとｽﾜｯﾌﾟﾁｪｰﾝの作成.
	//	ﾊｰﾄﾞｳｪｱ(GPU)ﾃﾞﾊﾞｲｽで作成.
	if (FAILED(
		D3D11CreateDeviceAndSwapChain(
		NULL,			//ﾋﾞﾃﾞｵｱﾀﾞﾌﾟﾀへのﾎﾟｲﾝﾀ.
		D3D_DRIVER_TYPE_HARDWARE,//作成するﾃﾞﾊﾞｲｽの種類.
		NULL,			//ｿﾌﾄｳｪｱ ﾗｽﾀﾗｲｻﾞｰを実装するDLLのﾊﾝﾄﾞﾙ.
		0,				//有効にするﾗﾝﾀｲﾑﾚｲﾔｰ.
		&pFeatureLevels,//作成を試みる機能ﾚﾍﾞﾙの順序を指定する配列へのﾎﾟｲﾝﾀ.
		1,				//↑の要素数.
		D3D11_SDK_VERSION,//SDKのﾊﾞｰｼﾞｮﾝ.
		&sd,			//ｽﾜｯﾌﾟﾁｪｰﾝの初期化ﾊﾟﾗﾒｰﾀのﾎﾟｲﾝﾀ.
		&m_pSwapChain,	//(out)ﾚﾝﾀﾞﾘﾝｸﾞに使用するｽﾜｯﾌﾟﾁｪｰﾝ.
		&m_pDevice,		//(out)作成されたﾃﾞﾊﾞｲｽ.
		pFeatureLevel,	//機能ﾚﾍﾞﾙの配列にある最初の要素を表すﾎﾟｲﾝﾀ.
		&m_pDeviceContext)))//(out)ﾃﾞﾊﾞｲｽ ｺﾝﾃｷｽﾄ.
	{
		//WARPﾃﾞﾊﾞｲｽの作成.
		//	D3D_FEATURE_LEVEL_9_1 ～ D3D_FEATURE_LEVEL_10_1
		if (FAILED(
			D3D11CreateDeviceAndSwapChain(
			NULL, D3D_DRIVER_TYPE_WARP,
			NULL, 0, &pFeatureLevels, 1,
			D3D11_SDK_VERSION, &sd, &m_pSwapChain,
			&m_pDevice, pFeatureLevel,
			&m_pDeviceContext)))
		{
			//ﾘﾌｧﾚﾝｽﾃﾞﾊﾞｲｽの作成.
			//	DirectX SDKがｲﾝｽﾄｰﾙされていないと使えない.
			if (FAILED(
				D3D11CreateDeviceAndSwapChain(
				NULL, D3D_DRIVER_TYPE_REFERENCE,
				NULL, 0, &pFeatureLevels, 1,
				D3D11_SDK_VERSION, &sd, &m_pSwapChain,
				&m_pDevice, pFeatureLevel,
				&m_pDeviceContext)))
			{
				MessageBox(NULL,
					"ﾃﾞﾊﾞｲｽとｽﾜｯﾌﾟﾁｪｰﾝの作成に失敗",
					"error(main.cpp)", MB_OK);
				return E_FAIL;
			}
		}
	}

	//各種ﾃｸｽﾁｬ-と、それに付帯する各種ﾋﾞｭｰ(画面)を作成.

	//-----------------------------------------------
	//	ﾊﾞｯｸﾊﾞｯﾌｧ準備:ｶﾗｰﾊﾞｯﾌｧ設定.
	//-----------------------------------------------

	//ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬ-を取得(既にあるので作成ではない)
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),	//__uuidof:式に関連付けされたGUIDを取得.
		//         Texture2Dの唯一の物として扱う.
		(LPVOID*)&pBackBuffer_Tex);	//(out)ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬ-.

	//そのﾃｸｽﾁｬに対しﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰ(RTV)を作成.
	m_pDevice->CreateRenderTargetView(
		pBackBuffer_Tex,
		NULL,
		&m_pBackBuffer_TexRTV);//(out)RTV.
	//ﾊﾞｯｸﾊﾞｯﾌｧﾃｸｽﾁｬ-を解放.
	SAFE_RELEASE(pBackBuffer_Tex);

	//-----------------------------------------------
	//	ﾊﾞｯｸﾊﾞｯﾌｧ準備:ﾃﾞﾌﾟｽ(深度)ｽﾃﾝｼﾙ関係.
	//-----------------------------------------------

	//ﾃﾞﾌﾟｽ(深さor深度)ｽﾃﾝｼﾙﾋﾞｭｰ用のﾃｸｽﾁｬ-を作成.
	D3D11_TEXTURE2D_DESC descDepth;

	descDepth.Width = WND_W;//幅.
	descDepth.Height = WND_H;//高さ.
	descDepth.MipLevels = 1;//ﾐｯﾌﾟﾏｯﾌﾟﾚﾍﾞﾙ:1.
	descDepth.ArraySize = 1;//配列数:1.
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;//32ﾋﾞｯﾄﾌｫｰﾏｯﾄ.
	descDepth.SampleDesc.Count = 1;//ﾏﾙﾁｻﾝﾌﾟﾙの数.
	descDepth.SampleDesc.Quality = 0;//ﾏﾙﾁｻﾝﾌﾟﾙのｸｵﾘﾃｨ.
	descDepth.Usage = D3D11_USAGE_DEFAULT;//使用方法:ﾃﾞﾌｫﾙﾄ.
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;//深度(ｽﾃﾝｼﾙとして使用)
	descDepth.CPUAccessFlags = 0;//CPUからはｱｸｾｽしない.
	descDepth.MiscFlags = 0;//その他の設定なし.

	m_pDevice->CreateTexture2D(
		&descDepth,
		NULL,
		&m_pBackBuffer_DSTex);//(out)ﾃﾞﾌﾟｽｽﾃﾝｼﾙ用ﾃｸｽﾁｬ.

	//そのﾃｸｽﾁｬに対しﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰ(DSV)を作成.
	m_pDevice->CreateDepthStencilView(
		m_pBackBuffer_DSTex,
		NULL,
		&m_pBackBuffer_DSTexDSV);//(out)DSV.

	//ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄﾋﾞｭｰとﾃﾞﾌﾟｽｽﾃﾝｼﾙﾋﾞｭｰをﾊﾟｲﾌﾟﾗｲﾝにｾｯﾄ.
	m_pDeviceContext->OMSetRenderTargets(
		1,
		&m_pBackBuffer_TexRTV,
		m_pBackBuffer_DSTexDSV);

	//深度ﾃｽﾄ(Zﾃｽﾄ)を有効にする.
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

	//ﾋﾞｭｰﾎﾟｰﾄの設定.
	D3D11_VIEWPORT vp;
	vp.Width = WND_W;	//幅.
	vp.Height = WND_H;	//高さ.
	vp.MinDepth = 0.0f;		//最小深度(手前)
	vp.MaxDepth = 1.0f;		//最大深度(奥)
	vp.TopLeftX = 0.0f;		//左上位置x.
	vp.TopLeftY = 0.0f;		//左上位置y.
	m_pDeviceContext->RSSetViewports(1, &vp);

	//ﾗｽﾀﾗｲｽﾞ(面の塗りつぶし方)設定.
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode = D3D11_FILL_SOLID;//塗りつぶし(ｿﾘｯﾄﾞ)
	rdc.CullMode = D3D11_CULL_NONE;
	//D3D11_CULL_NONE :ｶﾘﾝｸﾞを切る(正背面を描画する)
	//D3D11_CULL_BACK :背面を描画しない.
	//D3D11_CULL_FRONT:正面を描画しない.
	rdc.FrontCounterClockwise = FALSE;
	//ﾎﾟﾘｺﾞﾝの表裏を決定するﾌﾗｸﾞ.
	//TRUE :左回りなら前向き.右回りなら後ろ向き.
	//FALSE:↑の逆になる.
	rdc.DepthClipEnable = FALSE;	//距離についてのｸﾘｯﾋﾟﾝｸﾞ有効.

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

	//Direct3Dｵﾌﾞｼﾞｪｸﾄを解放.
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

#if 0
//ｽﾌｨｱ作成.
HRESULT clsMain::InitSphere(clsDX9Mesh* pMesh, float fScale)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;	//頂点ﾊﾞｯﾌｧ.
	void*		pVertices = NULL;	//頂点
	D3DXVECTOR3 vCenter;		//中心.
	float		fRadius;			//半径.

	//頂点ﾊﾞｯﾌｧを取得.
	if (FAILED(pMesh->m_pMesh->GetVertexBuffer(&pVB)))
	{
		return E_FAIL;
	}

	//ﾒｯｼｭの頂点ﾊﾞｯﾌｧをﾛｯｸする.
	if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
	{
		SAFE_RELEASE(pVB);
		return E_FAIL;
	}

	//ﾒｯｼｭの外接円の中心と半径を計算する.
	D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)pVertices,
		pMesh->m_pMesh->GetNumVertices(),//頂点の数.
		D3DXGetFVFVertexSize(pMesh->m_pMesh->GetFVF()),//頂点の情報.
		&vCenter,	//(out)中心座標.
		&fRadius);	//(out)半径.

	//ｱﾝﾛｯｸ.
	pVB->Unlock();
	SAFE_RELEASE(pVB);

	//中心と半径を構造体に設定.
	pMesh->m_Sphere.vCenter = vCenter;
	pMesh->m_Sphere.fRadius = fRadius * fScale;

	return S_OK;
}

#endif // 0

//ﾒｯｼｭ読込関数（まとめた）
HRESULT clsMain::MeshRead()
{
	//リソースクラス.
	m_pResource = clsResource::GetInstance();
	m_pResource->Init(m_hWnd, m_pDevice, m_pDeviceContext);

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
	//キャラクタクラス.
	//キャラクタのポインタの配列を作る.
	//for (int i = 0; i < m_iEnemyArrayMax; i++)
	//{
	//	m_vsmpEnemyArray.push_back(make_unique<clsCharacter>());

	//	//モデルデータ関連付け.
	//	//m_vsmpEnemyArray[i]->AttachModel( m_pEnemyModel );
	//	m_vsmpEnemyArray[i]->AttachModel( m_pResource->GetStaticModels( clsResource::enStaticModel_RoboA ) );
	//	m_vsmpEnemyArray[i]->SetScale( 3.0f );

	//	m_vsmpEnemyArray[i]->SetPosition(
	//		D3DXVECTOR3( -5.0f + ( i%10 ) * 1.0f -10.0f
	//					, 1.0f
	//					,-5.0f + ( i/10 ) * 1.0f ) );

	//}
#if 0

	//ｽｷﾝﾒｯｼｭｸﾗｽに必要な情報を渡して初期化.
	CD3DXSKINMESH_INIT si;
	si.hWnd = m_hWnd;
	si.pDevice = m_pDevice;
	si.pDeviceContext = m_pDeviceContext;

	m_pAnimModel = make_unique<clsD3DXSKINMESH>();
	m_pAnimModel->Init(&si);
	m_pAnimModel->CreateFromX("Data\\EXTINGER\\extinger.X");
	//m_pAnimModel->m_vPos.y = 0.0f;	//地面との表示位置調整.
	// ﾗｼﾞｱﾝ = ( 度数 × 円周率 ) ÷ 180.

	m_iAnimModelArrayMax = 10;
	for (int i = 0; i < m_iAnimModelArrayMax; i++)
	{
		m_smpBoss.push_back(make_unique<clsCharacterSkin>());
		m_smpBoss[i]->AttachModel(m_pAnimModel.get());

		m_smpBoss[i]->SetScale(i*0.01f);

		m_smpBoss[i]->SetPosition(
			D3DXVECTOR3(-5.0f + (i % 10) * 1.0f
			, 1.0f
			, -5.0f + (i / 10) * 1.0f));

		m_smpBoss[i]->ChangeAnimSet(i * 2);
		m_smpBoss[i]->m_dAnimSpeed = i * 0.01;
	}

#endif // 0
	m_smpClearScene->MusicStop();
	m_smpTitleScene->MusicStop();
	m_smpStageScene->MusicStop();
	m_smpResultScene->MusicStop();
	m_smpOverScene->MusicStop();

	return S_OK;
}

//深度ﾃｽﾄ(Zﾃｽﾄ) ON/OFF切替.
void clsMain::SetDepth(bool bFlg)
{
	//深度ﾃｽﾄ(Zﾃｽﾄ)を有効にする.
	D3D11_DEPTH_STENCIL_DESC	depthStencilDesc;
	ZeroMemory(&depthStencilDesc,
		sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = bFlg;

	m_pDevice->CreateDepthStencilState(
		&depthStencilDesc, &m_pDepthStencilState);

	m_pDeviceContext->OMSetDepthStencilState(
		m_pDepthStencilState, 1);
}

//ﾚｲとﾒｯｼｭの当たり判定.
bool clsMain::Intersect(
	clsGameObject* pAttacker,	//基準の物体.
	clsCharacter*  pTarget,		//対象の物体.
	float* pfDistance,			//(out)距離.
	D3DXVECTOR3* pvIntersect)	//(out)交差座標.
{
	D3DXMATRIXA16 matRot;	//回転行列.

	//回転行列を計算.
	D3DXMatrixRotationY(&matRot, pAttacker->GetRotationY());

	//軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3 vecAxisZ;
	//Z軸ﾍﾞｸﾄﾙそのものを現在の回転状態により変換する.
	D3DXVec3TransformCoord(
		&vecAxisZ, &pAttacker->m_vAxis, &matRot);

	D3DXVECTOR3 vecStart, vecEnd, vecDistance;
	//ﾚｲの開始終了位置をAttackerと合わせる.
	vecStart = vecEnd = pAttacker->m_vRay;
	//Attackerの座標に回転座標を合成する.
	vecEnd += vecAxisZ * 1.0f;

	//対象が動いている物体でも、対象のworld行列の、
	//逆行列を用いれば、正しくﾚｲが当たる.
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

	BOOL bHit = false;	//命中ﾌﾗｸﾞ.

	DWORD dwIndex = 0;		//ｲﾝﾃﾞｯｸｽ番号.
	D3DXVECTOR3 vVertex[3];	//頂点座標.
	FLOAT U = 0, V = 0;			//(out)重心ﾋｯﾄ座標.

	//ﾒｯｼｭとﾚｲの交差を調べる.
	D3DXIntersect(
		pTarget->GetMesh(),	//対象ﾒｯｼｭ.
		&vecStart,			//開始位置.
		&vecDistance,		//ﾚｲの距離.
		&bHit,				//(out)判定結果.
		&dwIndex,	//(out)bHitがTrue時、ﾚｲの始点に.
		//最も近くの面のｲﾝﾃﾞｯｸｽ値へのﾎﾟｲﾝﾀ.
		&U, &V,				//(out)重心ﾋｯﾄ座標.
		pfDistance,			//ﾀｰｹﾞｯﾄとの距離.
		NULL, NULL);
	if (bHit){
		//命中したとき.
		FindVerticesOnPoly(
			pTarget->GetMeshForRay(), dwIndex, vVertex);
		//重心座標から交差点を算出.
		//ﾛｰｶﾙ交点pは、v0 + U*(v1-v0) + V*(v2-v0)で求まる.
		*pvIntersect =
			vVertex[0]
			+ U * (vVertex[1] - vVertex[0])
			+ V * (vVertex[2] - vVertex[0]);

		return true;//命中している.
	}
	return false;//外れている.
}

//交差位置のﾎﾟﾘｺﾞﾝの頂点を見つける.
//※頂点座標はﾛｰｶﾙ座標.
HRESULT clsMain::FindVerticesOnPoly(
	LPD3DXMESH pTarget, DWORD dwPolyIndex,
	D3DXVECTOR3* pVecVertices)
{
	//頂点ごとのﾊﾞｲﾄ数を取得.
	DWORD dwStride = pTarget->GetNumBytesPerVertex();
	//頂点数を取得.
	DWORD dwVertexAmt = pTarget->GetNumVertices();
	//面数を取得.
	DWORD dwPolyAmt = pTarget->GetNumFaces();

	WORD* pwPoly = NULL;

	//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧをﾛｯｸ(読込ﾓｰﾄﾞ)
	pTarget->LockIndexBuffer(
		D3DLOCK_READONLY, (VOID**)&pwPoly);
	BYTE* pbVertices = NULL;//頂点(ﾊﾞｲﾄ型)
	FLOAT* pfVertices = NULL;//頂点(float型)
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;//頂点ﾊﾞｯﾌｧ.
	pTarget->GetVertexBuffer(&VB);//頂点情報の取得.

	//頂点ﾊﾞｯﾌｧのﾛｯｸ.
	if (SUCCEEDED(
		VB->Lock(0, 0, (VOID**)&pbVertices, 0)))
	{
		//ﾎﾟﾘｺﾞﾝの頂点の１つ目を取得.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3]];
		pVecVertices[0].x = pfVertices[0];
		pVecVertices[0].y = pfVertices[1];
		pVecVertices[0].z = pfVertices[2];

		//ﾎﾟﾘｺﾞﾝの頂点の２つ目を取得.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 1]];
		pVecVertices[1].x = pfVertices[0];
		pVecVertices[1].y = pfVertices[1];
		pVecVertices[1].z = pfVertices[2];

		//ﾎﾟﾘｺﾞﾝの頂点の３つ目を取得.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 2]];
		pVecVertices[2].x = pfVertices[0];
		pVecVertices[2].y = pfVertices[1];
		pVecVertices[2].z = pfVertices[2];

		pTarget->UnlockIndexBuffer();	//ﾛｯｸ解除.
		VB->Unlock();	//ﾛｯｸ解除.
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
	pAttacker->m_vRay.y = 1.0f;	//ﾚｲのy位置.

	//ﾚｲの向きにより当たる壁までの距離を求める.
	//軸ﾍﾞｸﾄﾙ前.
	pAttacker->m_vAxis = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	Intersect(pAttacker, pWall, &fDistance[0], &vIntersect[0]);
	//軸ﾍﾞｸﾄﾙ後ろ.
	pAttacker->m_vAxis = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	Intersect(pAttacker, pWall, &fDistance[1], &vIntersect[1]);
	//軸ﾍﾞｸﾄﾙ右.
	pAttacker->m_vAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	Intersect(pAttacker, pWall, &fDistance[2], &vIntersect[2]);
	//軸ﾍﾞｸﾄﾙ左.
	pAttacker->m_vAxis = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	Intersect(pAttacker, pWall, &fDistance[3], &vIntersect[3]);

	fYaw = fabs(pAttacker->GetRotationY());//fabs:絶対値(float版)
	dirOverGuard(&fYaw);	//0～2πの間に収める.

	//前が壁に接近.
	fDis = fDistance[0];
	if (fDis < WSPACE && fDis > 0.01f){
		if (pAttacker->GetRotationY() < 0.0f){
			//時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->SetPositionX(-WSPACE - fDis);
			}
			else{										//奥から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
		}
		else{
			//反時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else{										//奥から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
		}
	}

	//後ろが壁に接近.
	fDis = fDistance[1];
	if (fDis < WSPACE && fDis > 0.01f){
		if (pAttacker->GetRotationY() < 0.0f){
			//時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->SetPositionX(-WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else{										//奥から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
		}
		else{
			//反時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else{										//奥から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
		}
	}

	//右が壁に接近.
	fDis = fDistance[2];
	if (fDis < WSPACE && fDis > 0.01f){
		if (pAttacker->GetRotationY() < 0.0f){
			//時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
			else{										//奥から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
		}
		else{
			//反時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
			else{										//奥から.
				pAttacker->SetPositionX(-WSPACE - fDis);
			}
		}
	}

	//左が壁に接近.
	fDis = fDistance[3];
	if (fDis < WSPACE && fDis > 0.01f){
		if (pAttacker->GetRotationY() < 0.0f){
			//時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->SetPositionX(-WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
			else{										//奥から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
		}
		else{
			//反時計回り.
			if (fYaw >= 0.785f && fYaw < 2.355f){		//右から.
				pAttacker->SetPositionZ(-WSPACE - fDis);
			}
			else if (fYaw >= 2.355f && fYaw < 3.925f){	//前から.
				pAttacker->SetPositionX(-WSPACE - fDis);
			}
			else if (fYaw >= 3.925f && fYaw < 5.495f){	//左から.
				pAttacker->SetPositionZ(WSPACE - fDis);
			}
			else{										//奥から.
				pAttacker->SetPositionX(WSPACE - fDis);
			}
		}
	}
}

//回転値調整.
void clsMain::dirOverGuard(float* fYaw)
{
	if (*fYaw > D3DX_PI * 2.0f){
		//１周以上している.
		*fYaw -= D3DX_PI * 2.0f;	//2π(360°)分を引く.
	}

	//再帰関数.
	if (*fYaw > D3DX_PI * 2.0f){
		dirOverGuard(fYaw);
	}
}

//ｶﾒﾗ関数.
void clsMain::Camera()
{
	//軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3 vecAxisZ(0.0f, 0.0f, 1.0f);
	switch (m_enScene)
	{
		case Title:
			m_Camera.vEye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_Camera.vLook = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			break;
		case Stage:
			//===============================================
			//ｶﾒﾗ追従処理 ここから.
			//===============================================
			//ｶﾒﾗ位置(自機の背中から)の設定.
			m_Camera.vEye = m_Camera.vLook
				= m_smpStageScene->m_smpPlayer->GetPosition();//自機の位置をｺﾋﾟｰ.
			m_Camera.fYaw = m_smpStageScene->m_smpPlayer->GetRotationY();//回転値をｺﾋﾟｰ.

			//Y軸回転行列の作成.
			D3DXMatrixRotationY(
				&m_Camera.mRot, m_Camera.fYaw);

			//Z軸ﾍﾞｸﾄﾙそのものを回転状態により変換する.
			D3DXVec3TransformCoord(
				&vecAxisZ, &vecAxisZ, &m_Camera.mRot);

			m_Camera.vEye -= vecAxisZ/* * 4.0f*/;	//自機の背中側.
			m_Camera.vLook += vecAxisZ/* * 2.0f*/;	//自機の前側.

			m_Camera.vEye.y += 1.0f;	//ｶﾒﾗ位置(高さ)調整.
			m_Camera.vLook.y += 1.0f;	//注視位置(高さ)調整.

			//===============================================
			//ｶﾒﾗ追従処理 ここまで.
			//===============================================
			break;
		case Over:
			break;
		case Clear:
			break;
	}
	//ﾋﾞｭｰ(ｶﾒﾗ)変換.
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置.
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)ﾋﾞｭｰ計算結果.
		&m_Camera.vEye, &m_Camera.vLook, &vUpVec);
}

//ﾌﾟﾛｼﾞｪｸｼｮﾝ関数.
void clsMain::Proj()
{
	//ﾌﾟﾛｼﾞｪｸｼｮﾝ(射影行列)変換.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,	//(out)ﾌﾟﾛｼﾞｪｸｼｮﾝ計算結果.
		D3DX_PI / 4.0, //y方向の視野(ﾗｼﾞｱﾝ指定)数値を大きくしたら視野が狭くなる.
		(FLOAT)WND_W / (FLOAT)WND_H,//ｱｽﾍﾟｸﾄ比(幅÷高さ)
		0.1f,		//近いﾋﾞｭｰ平面のz値.
		100.0f);	//遠いﾋﾞｭｰ平面のz値.
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