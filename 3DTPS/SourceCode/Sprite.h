#ifndef _SPRITE_H_
#define _SPRITE_H_

//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

#include "Common.h"//共通クラス.

#include "Resource.h".

//======================================
//	構造体.
//======================================
//コンスタントバッファのアプリ側の定義(Simple.hlsl)
//※シェーダ内のコンスタントバッファと一致している必要あり.
struct SPRITESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX	mWVP;		//ワールド,ビュー,射影の合成変換行列.
	D3DXVECTOR4	vColor;		//カラー(RGBAの型に合わせる)
	D3DXVECTOR4	vUV;		//UV座標.
};

/*************************************************
*	Spriteクラス.
**/
class clsSprite
	: public clsCommon	//共通クラスを継承.
{
private:
	//↓モデルの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;//頂点シェーダ.
	ID3D11InputLayout*		m_pVertexLayout;//頂点レイアウト.
	ID3D11PixelShader*		m_pPixelShader;	//ピクセルシェーダ.
	ID3D11Buffer*			m_pConstantBuffer;//コンスタントバッファ.

	//↓モデルごとに用意.
	ID3D11Buffer*			m_pVertexBuffer;//頂点バッファ.

	ID3D11ShaderResourceView*	m_pTexture;//テクスチャ.
	ID3D11SamplerState*			m_pSampleLinear;//テクスチャのサンプラ-:テクスチャーに各種フィルタをかける.

protected:

public:
	D3DXVECTOR3		m_vPos;	//位置.
	D3DXVECTOR4 m_texUV;
	int				m_AnimCount;//UVスクロール用のカウンタ.

	bool			m_bDispFlg;	//表示フラグ.

	clsSprite();	//コンストラクタ.
	~clsSprite();	//デストラクタ.

	//初期化.
	HRESULT Init();

	//シェーダ作成.
	HRESULT InitShader();
	//モデル作成.
	HRESULT InitModel();

	//描画(レンダリング)
	void Render();
};

#endif//#ifndef _SPRITE_H_
