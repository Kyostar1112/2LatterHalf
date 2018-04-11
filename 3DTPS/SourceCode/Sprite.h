#ifndef _SPRITE_H_
#define _SPRITE_H_

//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

#include "Common.h"//共通ｸﾗｽ.

#include "Resource.h".

//======================================
//	構造体.
//======================================
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義(Simple.hlsl)
//※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
struct SPRITESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX	mWVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,射影の合成変換行列.
	D3DXVECTOR4	vColor;		//ｶﾗｰ(RGBAの型に合わせる)
	D3DXVECTOR4	vUV;		//UV座標.
};

/*************************************************
*	Spriteｸﾗｽ.
**/
class clsSprite
	: public clsCommon	//共通ｸﾗｽを継承.
{
private:
	//↓ﾓﾃﾞﾙの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;	//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pConstantBuffer;//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

	//↓ﾓﾃﾞﾙごとに用意.
	ID3D11Buffer*			m_pVertexBuffer;//頂点ﾊﾞｯﾌｧ.

	ID3D11ShaderResourceView*	m_pTexture;//ﾃｸｽﾁｬ.
	ID3D11SamplerState*			m_pSampleLinear;//ﾃｸｽﾁｬのｻﾝﾌﾟﾗ-:ﾃｸｽﾁｬーに各種ﾌｨﾙﾀをかける.

protected:

public:
	D3DXVECTOR3		m_vPos;	//位置.
	D3DXVECTOR4 m_texUV;
	int				m_AnimCount;//UVｽｸﾛｰﾙ用のｶｳﾝﾀ.

	bool			m_bDispFlg;	//表示ﾌﾗｸﾞ.

	clsSprite();	//ｺﾝｽﾄﾗｸﾀ.
	~clsSprite();	//ﾃﾞｽﾄﾗｸﾀ.

	//初期化.
	HRESULT Init();

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();
	//ﾓﾃﾞﾙ作成.
	HRESULT InitModel();

	//描画(ﾚﾝﾀﾞﾘﾝｸﾞ)
	void Render();
};

#endif//#ifndef _SPRITE_H_
