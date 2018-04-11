#ifndef _DEBUG_TEXT_H_
#define _DEBUG_TEXT_H_
//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>//「D3DX～」の定義使用時に必要.
#include <D3D10.h>

#include "MyMacro.h"

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
//ﾃｷｽﾄの位置情報.
struct TextVertex
{
	D3DXVECTOR3	Pos;	//位置.
	D3DXVECTOR2	Tex;	//ﾃｸｽﾁｬ座標.
};

//ﾃｷｽﾄｸﾗｽ.
class clsDebugText
{
private:
	//↓ｱﾌﾟﾘにひとつ.
	ID3D11Device*			m_pDevice11;		//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext*	m_pDeviceContext11;	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.

	//↓ﾓﾃﾞﾙの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;	//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pConstantBuffer;//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

	//↓ﾓﾃﾞﾙごとに用意.
	ID3D11Buffer*			m_pVertexBuffer[100];//頂点ﾊﾞｯﾌｧ(100個分).

	ID3D11ShaderResourceView*	m_pAsciiTexture;//ﾃｸｽﾁｬ.
	ID3D11SamplerState*			m_pSampleLinear;//ﾃｸｽﾁｬのｻﾝﾌﾟﾗ-:ﾃｸｽﾁｬーに各種ﾌｨﾙﾀをかける.

	ID3D11BlendState*			m_pBlendState;//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ.

	DWORD	m_dwWindowWidth;	//ｳｨﾝﾄﾞｳ幅.
	DWORD	m_dwWindowHeight;	//ｳｨﾝﾄﾞｳ高さ.

	float	m_fKerning[100];	//ｶｰﾆﾝｸﾞ(100個分)
	float	m_fScale;			//拡縮値(25pixelを基準 25pixel=1.0f)
	float	m_fAlpha;			//透過値.
	D3DXVECTOR4	m_vColor;		//色.

	D3DXMATRIX	m_mView;	//ﾋﾞｭｰ行列.
	D3DXMATRIX	m_mProj;	//ﾌﾟﾛｼﾞｪｸｼｮﾝ行列.

public:
	clsDebugText();	//ｺﾝｽﾄﾗｸﾀ.
	~clsDebugText();//ﾃﾞｽﾄﾗｸﾀ.

	HRESULT Init(ID3D11DeviceContext* pContext,
		DWORD dwWidth, DWORD dwHeight,
		float fSize, D3DXVECTOR4 vColor);

	//透過(ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ)設定の切り替え.
	void SetBlend(bool flg);
	//ﾚﾝﾀﾞﾘﾝｸﾞ関数.
	void Render(char* text, int x, int y);
	//ﾌｫﾝﾄﾚﾝﾀﾞﾘﾝｸﾞ関数.
	void RenderFont(int FontIndex, int x, int y);
};

#endif//#ifndef _DEBUG_TEXT_H_