#ifndef _DX9MESH_H_
#define _DX9MESH_H_
//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>//「D3DX〜」の定義使用時に必要.
#include <D3D10.h>
#include <d3dx9.h>
#include "MyMacro.h"

#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )//「D3DX〜」の定義使用時に必要.
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
//======================================
//	構造体.
//======================================
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｱﾌﾟﾘ側の定義(Mesh.hlsl)
//※ｼｪｰﾀﾞ内のｺﾝｽﾀﾝﾄﾊﾞｯﾌｧと一致している必要あり.
struct MESHSHADER_CONSTANT_BUFFER_ZERO
{
	D3DXMATRIX	mW;			//ﾜｰﾙﾄﾞ(位置)座標行列.
	D3DXMATRIX	mWVP;		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,射影の合成変換行列.
	D3DXVECTOR4 vLightDir;	//ﾗｲﾄ方向.
	D3DXVECTOR4	vEye;		//ｶﾒﾗ位置(視点位置).
	D3DXVECTOR4 vColor;		//ｶﾗｰ.
};
struct MESHSHADER_CONSTANT_BUFFER1
{
	D3DXVECTOR4	vAmbient;	//ｱﾝﾋﾞｴﾝﾄ.
	D3DXVECTOR4	vDiffuse;	//ﾃﾞｨﾌｭｰｽﾞ.
	D3DXVECTOR4	vSpecular;	//ｽﾍﾟｷｭﾗ.
};

//頂点の構造体.
struct MeshVertex
{
	D3DXVECTOR3	vPos;	//位置.
	D3DXVECTOR3	vNormal;//法線(陰影計算に必須)
	D3DXVECTOR2	vTex;	//ﾃｸｽﾁｬ座標.
};
//ﾏﾃﾘｱﾙ構造体.
struct MY_MATERIAL
{
	D3DXVECTOR4	Ambient;	//ｱﾝﾋﾞｴﾝﾄ.
	D3DXVECTOR4	Diffuse;	//ﾃﾞｨﾌｭｰｽﾞ.
	D3DXVECTOR4	Specular;	//ｽﾍﾟｷｭﾗ.
	CHAR szTextureName[128];//ﾃｸｽﾁｬﾌｧｲﾙ名.
	ID3D11ShaderResourceView* pTexture;//ﾃｸｽﾁｬ.
	DWORD dwNumFace;	//そのﾏﾃﾘｱﾙのﾎﾟﾘｺﾞﾝ数.
	//ｺﾝｽﾄﾗｸﾀ.
	MY_MATERIAL() {
		ZeroMemory(this, sizeof(MY_MATERIAL));
	}
	//ﾃﾞｽﾄﾗｸﾀ.
	~MY_MATERIAL() {
		SAFE_RELEASE(pTexture);
	}
};

//Bﾎﾞｯｸｽ構造体.
struct BBOX
{
	D3DXVECTOR3 vPosMax;	//最大頂点.
	D3DXVECTOR3	vPosMin;	//最小頂点.
	D3DXVECTOR3	vAxisX;		//X軸.
	D3DXVECTOR3 vAxisY;		//Y軸.
	D3DXVECTOR3 vAxisZ;		//Z軸.
	FLOAT		fLengthX;	//X長さ.
	FLOAT		fLengthY;	//Y長さ.
	FLOAT		fLengthZ;	//Z長さ.
	//初期化.
	BBOX() {
		ZeroMemory(this, sizeof(BBOX));
		vAxisX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		vAxisY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vAxisZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
};

//ﾒｯｼｭﾃﾞｰﾀをﾌｧｲﾙから取り出す為だけに、DirectX9を使用する.
//※ﾚﾝﾀﾞﾘﾝｸﾞ(表示)は、DirectX11で行う.
class clsDX9Mesh
{
public:
	HWND				m_hWnd;	//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙ.

	//Dx9.
	LPDIRECT3D9			m_pD3d;	//Dx9ｵﾌﾞｼﾞｪｸﾄ.
	LPDIRECT3DDEVICE9	m_pDevice9;//Dx9ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.

	//	D3DMATERIAL9*		m_pMaterials;	//ﾏﾃﾘｱﾙ情報.
	//	LPDIRECT3DTEXTURE9*	m_pTextures;	//ﾃｸｽﾁｬ情報.
	//	char		m_TexFileName[8][256];//ﾃｸｽﾁｬﾌｧｲﾙ名(8枚まで)
	DWORD		m_dwNumMaterials;//ﾏﾃﾘｱﾙ数.
	LPD3DXMESH	m_pMesh;	//ﾒｯｼｭｵﾌﾞｼﾞｪｸﾄ.

	//Dx11.
	ID3D11Device*			m_pDevice11;//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	ID3D11DeviceContext*	m_pDeviceContext11;//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
	ID3D11VertexShader*		m_pVertexShader;//頂点ｼｪｰﾀﾞ.
	ID3D11InputLayout*		m_pVertexLayout;//頂点ﾚｲｱｳﾄ.
	ID3D11PixelShader*		m_pPixelShader;	//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	ID3D11Buffer*			m_pConstantBuffer0;//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
	ID3D11Buffer*			m_pConstantBuffer1;//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

	ID3D11Buffer*	m_pVertexBuffer;//頂点(ﾊﾞｰﾃｯｸｽ)ﾊﾞｯﾌｧ.
	ID3D11Buffer**	m_ppIndexBuffer;//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ.
	//	ID3D11ShaderResourceView*	m_pTexture;//ﾃｸｽﾁｬ.
	ID3D11SamplerState*			m_pSampleLinear;//ﾃｸｽﾁｬのｻﾝﾌﾟﾗ-:ﾃｸｽﾁｬーに各種ﾌｨﾙﾀをかける.

	MY_MATERIAL*	m_pMaterials;//ﾏﾃﾘｱﾙ構造体.
	DWORD			m_NumAttr;//属性数.
	DWORD			m_AttrID[300];//属性ID ※300属性まで.

	bool			m_bTexture;	//ﾃｸｽﾁｬの有無.

	float			m_fScale;	//拡縮.
	float			m_fYaw;		//回転(Y軸)
	float			m_fPitch;	//回転(X軸)
	float			m_fRoll;	//回転(Z軸)
	D3DXVECTOR3		m_vPos;		//位置(X,Y,Z)

	enDirection		m_enDir;	//方向.

	bool			m_bShotFlg;	//ｼｮｯﾄﾌﾗｸﾞ.
	bool			m_bDeadFlg;	//死亡ﾌﾗｸﾞ.

	BBOX			m_BBox;		//Bﾎﾞｯｸｽ構造体.
	SPHERE			m_Sphere;	//ｽﾌｨｱ構造体.

	D3DXVECTOR3		m_vRay;		//ﾚｲの位置.
	D3DXVECTOR3		m_vAxis;	//回転軸.
	LPD3DXMESH		m_pMeshForRay;	//ﾚｲとﾒｯｼｭ用.

	D3DXMATRIX m_mYaw;//ラジコン操作用.

	ID3D11BlendState*	m_pBlendState;//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ.

	clsDX9Mesh();	//ｺﾝｽﾄﾗｸﾀ.
	~clsDX9Mesh();	//ﾃﾞｽﾄﾗｸﾀ.

	HRESULT Init(HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPSTR fileName);
	//Dx9初期化.
	HRESULT InitDx9(HWND hWnd);
	//ﾒｯｼｭ読込.
	HRESULT LoadXMesh(LPSTR fileName);

	//ｼｪｰﾀﾞ作成.
	HRESULT InitShader();
	//ﾚﾝﾀﾞﾘﾝｸﾞ用(※DX9MESH内とMain内で２つ存在するので注意)
	void Render(D3DXMATRIX &mView, D3DXMATRIX &mProj,
		D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye,
		D3DXVECTOR4 vColor, bool alphaFlg);

	//透過(ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ)設定の切り替え.
	void SetBlend(bool flg);

	//位置更新関数.
	D3DXVECTOR3 UpdatePos(D3DXVECTOR3 oldPos);
	//位置更新関数の初期化.
	void InitPos();
	bool m_bInitPosFlg;
	D3DXVECTOR3 m_vecAxisZ;
	//解放関数.
	HRESULT Release();
};

#endif//#ifndef _DX9MESH_H_