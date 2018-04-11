#ifndef _SPRITE_2D_H_
#define _SPRITE_2D_H_

//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

using namespace std;

//============================================================
//	ｲﾝｸﾙｰﾄﾞ.
//============================================================
#include "Common.h"//共通ｸﾗｽ.
#include <memory>
#include <string>
#include <d3dx9tex.h>
#define ALIGN16 _declspec( align( 16 ) )

//============================================================
//	構造体.
//============================================================
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義(Sprite2D.hlsl).
//ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
struct SPRITE2D_CONSTANT_BUFFER
{
	ALIGN16 D3DXMATRIX	mW;				//ﾜｰﾙﾄﾞ行列.
	ALIGN16 float		fViewPortWidth;	//ﾋﾞｭｰﾎﾟｰﾄ幅.
	ALIGN16 float		fViewPortHeight;//ﾋﾞｭｰﾎﾟｰﾄ高さ.
	ALIGN16 float		fAlpha;			//ｱﾙﾌｧ値(透過).
	ALIGN16 D3DXVECTOR2 vUV;			//UV座標.
};
//頂点の構造体.
struct Sprite2DVertex
{
	D3DXVECTOR3	vPos;	//位置.
	D3DXVECTOR2	vTex;	//ﾃｸｽﾁｬ座標.
};
//////ｽﾌﾟﾗｲﾄ構造体.
//struct SPRITE_STATE
//{
//	WHSIZE_FLOAT	Disp;	//表示幅高さ.
//	WHSIZE_FLOAT	Base;	//元画像高さ.
//	WHSIZE_FLOAT	Stride;	//一コマあたりの高さ.
//};//m_SState.
//ｽﾌﾟﾗｲﾄ構造体.
struct SPRITE_STATE
{
	WHSIZE_FLOAT	Disp;	//表示幅高さ.
	WHSIZE_FLOAT	Base;	//元画像高さ.
	//WHSIZE_FLOAT	Stride;	//一コマあたりの高さ.
	WHSIZE_FLOAT	Stride;	//コマの数.
};//m_SState.
enum en_BlackMode
{
	Idle = 0,
	In,
	Change,
	Out,
};

//============================================================
//	ｽﾌﾟﾗｲﾄ2Dｸﾗｽ.
//============================================================
class clsSprite2D
	: public clsCommon
{
public:
	clsSprite2D();	//ｺﾝｽﾄﾗｸﾀ.
	virtual ~clsSprite2D();	//ﾃﾞｽﾄﾗｸﾀ.

	//初期化.
	HRESULT Create(ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		LPSTR fileName);

	////スプライトを画面外へ.
	//void InitSetPos()
	//{
	//	m_vPos = D3DXVECTOR3( WND_W, WND_H, 0.0f );
	//}

	void SetPos(D3DXVECTOR3 Pos)
	{
		m_vPos = Pos;
	}

	void AddPosX(float Add)
	{
		m_vPos.x += Add;
	}

	void AddPosY(float Add)
	{
		m_vPos.y += Add;
	}

	void AddPosZ(float Add)
	{
		m_vPos.z += Add;
	}

	void SetPosX(float PosX)
	{
		m_vPos.x = PosX;
	}

	void SetPosY(float PosY)
	{
		m_vPos.y = PosY;
	}

	void SetPosZ(float PosZ)
	{
		m_vPos.z = PosZ;
	}

	void SetPos(float x, float y, float z = 0.0f)
	{
		m_vPos.x = x;
		m_vPos.y = y;
		m_vPos.z = z;
	}

	void SetAlpha(float Alpha)
	{
		m_fAlpha = Alpha;
	}

	void AddAlpha(float Alpha)
	{
		m_fAlpha += Alpha;
	}
	void SetStrideX(float StrideW)
	{
		m_SState.Stride.w = StrideW;		//横コマ数.
	}
	void SetStrideY(float StrideH)
	{
		m_SState.Stride.h = StrideH;		//縦コマ数.
	}

	//横のパターン.
	void SetPatarnU(float Patarn)
	{
		m_fPatternNo.x = Patarn;
	}

	//縦のパターン.
	void SetPatarnV(float Patarn)
	{
		m_fPatternNo.y = Patarn;
	}

	void SetSs(SPRITE_STATE ss)
	{
		m_SState = ss;
	}

	//SPRITE_STATEにセット.
	void SetSs(
		float BaseW,			//元画像高さ.
		float BaseH,		    //元画像幅.
		float DispW = -1,	    //表示高さ.//何も入れてなかったらBaseに合わせる.
		float DispH = -1,       //表示幅.//何も入れてなかったらBaseに合わせる.
		float StrideW = 1,	    //縦コマ数.
		float StrideH = 1)	    //横コマ数.
	{
		m_SState.Base.h = BaseH;		//元画像高さ.
		m_SState.Base.w = BaseW;		//元画像幅.
		if (DispH < 0)
		{
			m_SState.Disp.h = m_SState.Base.h;
		}
		else
		{
			m_SState.Disp.h = DispH;		//表示高さ.
		}
		if (DispW < 0)
		{
			m_SState.Disp.w = m_SState.Base.w;	//表示幅.
		}
		else
		{
			m_SState.Disp.w = DispW;		//表示幅.
		}
		m_SState.Stride.h = StrideH;		//縦コマ数.
		m_SState.Stride.w = StrideW;		//横コマ数.
	}

	void SetDispH(float DispH)
	{
		m_SState.Disp.h = DispH;
	}

	void SetDispW(float DispW)
	{
		m_SState.Disp.w = DispW;
	}

	void AddDispH(float DispH)
	{
		m_SState.Disp.h += DispH;
	}

	void AddDispW(float DispW)
	{
		m_SState.Disp.w += DispW;
	}

	void MulDisp(float Disp)
	{
		m_SState.Disp.w = Disp * (m_SState.Base.w / m_SState.Stride.w);
		m_SState.Disp.h = Disp * (m_SState.Base.h / m_SState.Stride.h);
	}

	float GetAlpha()
	{
		return m_fAlpha;
	}

	SPRITE_STATE GetSs()
	{
		return m_SState;
	}

	WHSIZE_FLOAT GetCenterDisp()
	{
		WHSIZE_FLOAT tmp;
		tmp.w = m_SState.Disp.w / 2;
		tmp.h = m_SState.Disp.h / 2;
		return tmp;
	}

	D3DXVECTOR3 GetPos()
	{
		return m_vPos;
	}

	//SPRITE_STATE変更を適用するために必要.
	virtual void UpDateSpriteSs();

	//点滅用.
	void Flashing(float ChaAmo, float AlphaMax = 1.0f, float AlphaMin = 0.0f);

	//描画(ﾚﾝﾀﾞﾘﾝｸﾞ)(※DX9MESH内とMain内で2つ存在するので注意).
	virtual void Render();
	en_BlackMode m_BlackMode;

	bool Release();

protected:

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();
	////ﾓﾃﾞﾙ作成.
	virtual HRESULT InitModel(SPRITE_STATE ss);
	//ﾃｸｽﾁｬ作成.
	HRESULT CreateTexture(LPSTR fileName,
		ID3D11ShaderResourceView** pTex);

	bool			m_bFlashing;

	float			MulPoint = 1.0f;//掛け算用の変数.

	string			m_sFileName;

	D3DXVECTOR3		m_vPos;	//位置.

	int				m_AnimCount;//UVｽｸﾛｰﾙ.

	bool			m_bDispFlg;//表示ﾌﾗｸﾞ.

	float			m_fAlpha;	//ｱﾙﾌｧ値.

	SPRITE_STATE	m_SState;	//ｽﾌﾟﾗｲﾄ情報.
	POINTFLOAT		m_fPatternNo;//ﾏｽ目番号.

	//↓ﾓﾃﾞﾙの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;	//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;		//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pConstantBuffer;	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
	ID3D11ShaderResourceView*	m_pTexture;		//ﾃｸｽﾁｬ.

	//↓ﾓﾃﾞﾙごとに用意.
	ID3D11Buffer*			m_pVertexBuffer;	//頂点ﾊﾞｯﾌｧ.

	ID3D11SamplerState*		m_pSampleLinear;//ﾃｸｽﾁｬのｻﾝﾌﾟﾗｰ:/ﾃｸｽﾁｬに各種ﾌｨﾙﾀをかける.
};

#endif//#define _SPRITE_2D_H_
