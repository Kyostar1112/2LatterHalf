#ifndef _DX9MESH_H_
#define _DX9MESH_H_
//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>//「D3DX～」の定義使用時に必要.
#include <D3D10.h>
#include <d3dx9.h>
#include "MyMacro.h"

#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )//「D3DX～」の定義使用時に必要.
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
//======================================
//	構造体.
//======================================
//コンスタントバッファのアプリ側の定義(Mesh.hlsl)
//※シェーダ内のコンスタントバッファと一致している必要あり.
struct MESHSHADER_CONSTANT_BUFFER_ZERO
{
	D3DXMATRIX	mW;			//ワールド(位置)座標行列.
	D3DXMATRIX	mWVP;		//ワールド,ビュー,射影の合成変換行列.
	D3DXVECTOR4 vLightDir;	//ライト方向.
	D3DXVECTOR4	vEye;		//カメラ位置(視点位置).
	D3DXVECTOR4 vColor;		//カラー.
};
struct MESHSHADER_CONSTANT_BUFFER1
{
	D3DXVECTOR4	vAmbient;	//アンビエント.
	D3DXVECTOR4	vDiffuse;	//ディフューズ.
	D3DXVECTOR4	vSpecular;	//スペキュラ.
};

//頂点の構造体.
struct MeshVertex
{
	D3DXVECTOR3	vPos;	//位置.
	D3DXVECTOR3	vNormal;//法線(陰影計算に必須)
	D3DXVECTOR2	vTex;	//テクスチャ座標.
};
//マテリアル構造体.
struct MY_MATERIAL
{
	D3DXVECTOR4	Ambient;	//アンビエント.
	D3DXVECTOR4	Diffuse;	//ディフューズ.
	D3DXVECTOR4	Specular;	//スペキュラ.
	CHAR szTextureName[128];//テクスチャファイル名.
	ID3D11ShaderResourceView* pTexture;//テクスチャ.
	DWORD dwNumFace;	//そのマテリアルのポリゴン数.
	//コンストラクタ.
	MY_MATERIAL() {
		ZeroMemory(this, sizeof(MY_MATERIAL));
	}
	//デストラクタ.
	~MY_MATERIAL() {
		SAFE_RELEASE(pTexture);
	}
};

//Bボックス構造体.
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

//メッシュデータをファイルから取り出す為だけに、DirectX9を使用する.
//※レンダリング(表示)は、DirectX11で行う.
class clsDX9Mesh
{
public:
	HWND				m_hWnd;	//ウィンドウハンドル.

	//Dx9.
	LPDIRECT3D9			m_pD3d;	//Dx9オブジェクト.
	LPDIRECT3DDEVICE9	m_pDevice9;//Dx9デバイスオブジェクト.

	//	D3DMATERIAL9*		m_pMaterials;	//マテリアル情報.
	//	LPDIRECT3DTEXTURE9*	m_pTextures;	//テクスチャ情報.
	//	char		m_TexFileName[8][256];//テクスチャファイル名(8枚まで)
	DWORD		m_dwNumMaterials;//マテリアル数.
	LPD3DXMESH	m_pMesh;	//メッシュオブジェクト.

	//Dx11.
	ID3D11Device*			m_pDevice11;//デバイスオブジェクト.
	ID3D11DeviceContext*	m_pDeviceContext11;//デバイスコンテキスト.
	ID3D11VertexShader*		m_pVertexShader;//頂点シェーダ.
	ID3D11InputLayout*		m_pVertexLayout;//頂点レイアウト.
	ID3D11PixelShader*		m_pPixelShader;	//ピクセルシェーダ.
	ID3D11Buffer*			m_pConstantBuffer0;//コンスタントバッファ.
	ID3D11Buffer*			m_pConstantBuffer1;//コンスタントバッファ.

	ID3D11Buffer*	m_pVertexBuffer;//頂点(バーテックス)バッファ.
	ID3D11Buffer**	m_ppIndexBuffer;//インデックスバッファ.
	//	ID3D11ShaderResourceView*	m_pTexture;//テクスチャ.
	ID3D11SamplerState*			m_pSampleLinear;//テクスチャのサンプラ-:テクスチャーに各種フィルタをかける.

	MY_MATERIAL*	m_pMaterials;//マテリアル構造体.
	DWORD			m_NumAttr;//属性数.
	DWORD			m_AttrID[300];//属性ID ※300属性まで.

	bool			m_bTexture;	//テクスチャの有無.

	float			m_fScale;	//拡縮.
	float			m_fYaw;		//回転(Y軸)
	float			m_fPitch;	//回転(X軸)
	float			m_fRoll;	//回転(Z軸)
	D3DXVECTOR3		m_vPos;		//位置(X,Y,Z)

	enDirection		m_enDir;	//方向.

	bool			m_bShotFlg;	//ショットフラグ.
	bool			m_bDeadFlg;	//死亡フラグ.

	BBOX			m_BBox;		//Bボックス構造体.
	SPHERE			m_Sphere;	//スフィア構造体.

	D3DXVECTOR3		m_vRay;		//レイの位置.
	D3DXVECTOR3		m_vAxis;	//回転軸.
	LPD3DXMESH		m_pMeshForRay;	//レイとメッシュ用.

	D3DXMATRIX m_mYaw;//ラジコン操作用.

	ID3D11BlendState*	m_pBlendState;//ブレンドステート.

	clsDX9Mesh();	//コンストラクタ.
	~clsDX9Mesh();	//デストラクタ.

	HRESULT Init(HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPSTR fileName);
	//Dx9初期化.
	HRESULT InitDx9(HWND hWnd);
	//メッシュ読込.
	HRESULT LoadXMesh(LPSTR fileName);

	//シェーダ作成.
	HRESULT InitShader();
	//レンダリング用(※DX9MESH内とMain内で２つ存在するので注意)
	void Render(D3DXMATRIX &mView, D3DXMATRIX &mProj,
		D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye,
		D3DXVECTOR4 vColor, bool alphaFlg);

	//透過(アルファブレンド)設定の切り替え.
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