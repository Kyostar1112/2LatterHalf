#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

#include "Common.h"//共通ｸﾗｽ.

//======================================
//	構造体.
//======================================
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義(PointSprite.hlsl)
//※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
struct PSPRITESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX	mWVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,射影の合成変換行列.
	//	D3DXVECTOR4	vColor;		//ｶﾗｰ(RGBAの型に合わせる)
	//	D3DXVECTOR4	vUV;		//UV座標.
};
//頂点の構造体.
struct PSpriteVertex
{
	D3DXVECTOR3	vPos;	//位置.
	//	D3DXVECTOR2	vTex;	//ﾃｸｽﾁｬ座標.
};

//ﾊﾟｰﾃｨｸﾙ用構造体.
struct PART
{
	D3DXVECTOR3	vPos;	//位置.
	D3DXVECTOR3	vDir;	//方向.
	float		fSpeed;	//速度.
	int			iBirthFrame;//開始ﾌﾚｰﾑ.

	//構造体の初期化(ｺﾝｽﾄﾗｸﾀ)
	PART(){
		ZeroMemory(this, sizeof(PART));
	}
};

/*************************************************
*	Particleｸﾗｽ.
**/
class clsParticle
	: public clsCommon	//共通ｸﾗｽを継承.
{
private:
	//↓ﾓﾃﾞﾙの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;//頂点ﾚｲｱｳﾄ.
	ID3D11GeometryShader*	m_pGeometryShader;//ｼﾞｵﾒﾄﾘｼｪｰﾀﾞ.
	ID3D11PixelShader*		m_pPixelShader;	//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pConstantBuffer;//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

	//↓ﾓﾃﾞﾙごとに用意.
	ID3D11Buffer*			m_pVertexBuffer;//頂点ﾊﾞｯﾌｧ.

	ID3D11ShaderResourceView*	m_pTexture;//ﾃｸｽﾁｬ.
	ID3D11SamplerState*			m_pSampleLinear;//ﾃｸｽﾁｬのｻﾝﾌﾟﾗ-:ﾃｸｽﾁｬーに各種ﾌｨﾙﾀをかける.

public:
	D3DXVECTOR3		m_vPos;	//位置.

	int				m_AnimCount;//UVｽｸﾛｰﾙ用のｶｳﾝﾀ.

	bool			m_bDispFlg;	//表示ﾌﾗｸﾞ.

	int				m_MaxParticle;	//ﾊﾟｰﾃｨｸﾙ最大数.
	D3DXVECTOR3		m_vEmitPos;		//放射位置.
	PART*			m_pParticleArray;//ﾊﾟｰﾃｨｸﾙ配列.
	int				m_Frame;		//ﾌﾚｰﾑ数.

	//ｵｰﾊﾞｰﾛｰﾄﾞ関数.
	clsParticle(int MaxParticle, D3DXVECTOR3 &vEmitPos);//ｺﾝｽﾄﾗｸﾀ.

	//ｺﾝｽﾄﾗｸﾀ.
	clsParticle(){
		ZeroMemory(this, sizeof(clsParticle));
	};
	~clsParticle();	//ﾃﾞｽﾄﾗｸﾀ.

	//初期化.
	HRESULT Init(ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11);

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();
	//ﾎﾟｲﾝﾄｽﾌﾟﾗｲﾄ初期化.
	HRESULT InitPointSprite();
	//ﾃｸｽﾁｬ読込＆作成.
	HRESULT InitTexture(char *pFileName);

	//描画(ﾚﾝﾀﾞﾘﾝｸﾞ)
	void Render(D3DXMATRIX &mView, D3DXMATRIX &mProj,
		D3DXVECTOR3 &vEye, D3DXVECTOR3 &vPos);

	//ｼｪｰﾀﾞをﾃﾞﾀｯﾁする.
	void DetachShader();

	//ﾊﾟｰﾃｨｸﾙの移動.
	void Run();

	//ﾊﾟｰﾃｨｸﾙ位置の取得.
	D3DXVECTOR3& GetParticlePos(int index){
		//配列の範囲かどうかﾁｪｯｸ.
		if (0 <= index && index < m_MaxParticle){
			return m_pParticleArray[index].vPos;
		}
		//範囲外のときは先頭を返す.
		return m_pParticleArray[index].vPos;
	}
	//ﾊﾟｰﾃｨｸﾙ最大数の取得.
	int GetMaxPartice(){ return m_MaxParticle; }
};

#endif//#ifndef _PARTICLE_H_
