#ifndef _C_COMMON_H_
#define _C_COMMON_H_

//ｲﾝｸﾙｰﾄﾞ.
#include "Global.h"

//頂点の構造体.
struct SpriteVertex
{
	D3DXVECTOR3 vPos;	//位置.
	D3DXVECTOR2 vTex;	//ﾃｸｽﾁｬ座標.
};

//幅高さ構造体.
struct WHSIZE_FLOAT
{
	float w;
	float h;
};

//共通ｸﾗｽ(親ｸﾗｽ).
class clsCommon
{
public:
	clsCommon() {};
	virtual ~clsCommon() {};

	//透過(ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ)設定の切り替え.
	void SetBlend(bool flg);

	//↓ｱﾌﾟﾘに一つ.
	ID3D11Device*			m_pDevice11;		//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext*	m_pDeviceContext11;	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.

	ID3D11BlendState*		m_pBlendState;	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ.

private:
};

#endif//#define _C_COMMON_H_
