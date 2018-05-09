#ifndef _DEBUG_TEXT_H_
#define _DEBUG_TEXT_H_
//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include "MyMacro.h"

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
#define DIMENSION		(12.0f)
#define WDIMENSION		(10.0f)
#define TEX_DIMENSION	(128.0f)

//構造体.
struct TEXT_CONSTANT_BUFFER
{
	D3DXMATRIX	mWVP;
	D3DXVECTOR4	vColor;
	D3DXVECTOR4	fAlpha;
};
//テキストの位置情報.
struct TextVertex
{
	D3DXVECTOR3	Pos;	//位置.
	D3DXVECTOR2	Tex;	//テクスチャ座標.
};

//テキストクラス.
class clsDebugText
{
private:
	//↓アプリにひとつ.
	ID3D11Device*			m_pDevice11;		//デバイスオブジェクト.
	ID3D11DeviceContext*	m_pDeviceContext11;	//デバイスコンテキスト.

	//↓モデルの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;//頂点シェーダ.
	ID3D11InputLayout*		m_pVertexLayout;//頂点レイアウト.
	ID3D11PixelShader*		m_pPixelShader;	//ピクセルシェーダ.
	ID3D11Buffer*			m_pConstantBuffer;//コンスタントバッファ.

	//↓モデルごとに用意.
	ID3D11Buffer*			m_pVertexBuffer[100];//頂点バッファ(100個分).

	ID3D11ShaderResourceView*	m_pAsciiTexture;//テクスチャ.
	ID3D11SamplerState*			m_pSampleLinear;//テクスチャのサンプラ-:テクスチャーに各種フィルタをかける.

	ID3D11BlendState*			m_pBlendState;//ブレンドステート.

	DWORD	m_dwWindowWidth;	//ウィンドウ幅.
	DWORD	m_dwWindowHeight;	//ウィンドウ高さ.

	float	m_fKerning[100];	//カーニング(100個分)
	float	m_fScale;			//拡縮値(25pixelを基準 25pixel=1.0f)
	float	m_fAlpha;			//透過値.
	D3DXVECTOR4	m_vColor;		//色.

	D3DXMATRIX	m_mView;	//ビュー行列.
	D3DXMATRIX	m_mProj;	//プロジェクション行列.

public:
	clsDebugText();	//コンストラクタ.
	~clsDebugText();//デストラクタ.

	HRESULT Init(ID3D11DeviceContext* pContext,
		DWORD dwWidth, DWORD dwHeight,
		float fSize, D3DXVECTOR4 vColor);

	//透過(アルファブレンド)設定の切り替え.
	void SetBlend(bool flg);
	//レンダリング関数.
	void Render(char* text, int x, int y);
	//フォントレンダリング関数.
	void RenderFont(int FontIndex, int x, int y);
};

#endif//#ifndef _DEBUG_TEXT_H_