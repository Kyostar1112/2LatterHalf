#ifndef _DX9MESH_H_
#define _DX9MESH_H_
//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning( disable:4005 )

#include <Windows.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>
#include <d3dx9.h>
#include "MyMacro.h"

#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
//======================================
//	�\����.
//======================================
//�ݽ����ޯ̧�̱��ؑ��̒�`(Mesh.hlsl)
//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
struct MESHSHADER_CONSTANT_BUFFER_ZERO
{
	D3DXMATRIX	mW;			//ܰ���(�ʒu)���W�s��.
	D3DXMATRIX	mWVP;		//ܰ���,�ޭ�,�ˉe�̍����ϊ��s��.
	D3DXVECTOR4 vLightDir;	//ײĕ���.
	D3DXVECTOR4	vEye;		//��׈ʒu(���_�ʒu).
	D3DXVECTOR4 vColor;		//�װ.
};
struct MESHSHADER_CONSTANT_BUFFER1
{
	D3DXVECTOR4	vAmbient;	//���޴��.
	D3DXVECTOR4	vDiffuse;	//�ި̭���.
	D3DXVECTOR4	vSpecular;	//��߷��.
};

//���_�̍\����.
struct MeshVertex
{
	D3DXVECTOR3	vPos;	//�ʒu.
	D3DXVECTOR3	vNormal;//�@��(�A�e�v�Z�ɕK�{)
	D3DXVECTOR2	vTex;	//ø������W.
};
//��رٍ\����.
struct MY_MATERIAL
{
	D3DXVECTOR4	Ambient;	//���޴��.
	D3DXVECTOR4	Diffuse;	//�ި̭���.
	D3DXVECTOR4	Specular;	//��߷��.
	CHAR szTextureName[128];//ø���̧�ٖ�.
	ID3D11ShaderResourceView* pTexture;//ø���.
	DWORD dwNumFace;	//������رق���غ�ݐ�.
	//�ݽ�׸�.
	MY_MATERIAL() {
		ZeroMemory(this, sizeof(MY_MATERIAL));
	}
	//�޽�׸�.
	~MY_MATERIAL() {
		SAFE_RELEASE(pTexture);
	}
};

//B�ޯ���\����.
struct BBOX
{
	D3DXVECTOR3 vPosMax;	//�ő咸�_.
	D3DXVECTOR3	vPosMin;	//�ŏ����_.
	D3DXVECTOR3	vAxisX;		//X��.
	D3DXVECTOR3 vAxisY;		//Y��.
	D3DXVECTOR3 vAxisZ;		//Z��.
	FLOAT		fLengthX;	//X����.
	FLOAT		fLengthY;	//Y����.
	FLOAT		fLengthZ;	//Z����.
	//������.
	BBOX() {
		ZeroMemory(this, sizeof(BBOX));
		vAxisX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		vAxisY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vAxisZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	}
};

//ү���ް���̧�ق�����o���ׂ����ɁADirectX9���g�p����.
//�������ݸ�(�\��)�́ADirectX11�ōs��.
class clsDX9Mesh
{
public:
	HWND				m_hWnd;	//����޳�����.

	//Dx9.
	LPDIRECT3D9			m_pD3d;	//Dx9��޼ު��.
	LPDIRECT3DDEVICE9	m_pDevice9;//Dx9���޲���޼ު��.

	//	D3DMATERIAL9*		m_pMaterials;	//��رُ��.
	//	LPDIRECT3DTEXTURE9*	m_pTextures;	//ø������.
	//	char		m_TexFileName[8][256];//ø���̧�ٖ�(8���܂�)
	DWORD		m_dwNumMaterials;//��رِ�.
	LPD3DXMESH	m_pMesh;	//ү����޼ު��.

	//Dx11.
	ID3D11Device*			m_pDevice11;//���޲���޼ު��.
	ID3D11DeviceContext*	m_pDeviceContext11;//���޲���÷��.
	ID3D11VertexShader*		m_pVertexShader;//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;	//�߸�ټ����.
	ID3D11Buffer*			m_pConstantBuffer0;//�ݽ����ޯ̧.
	ID3D11Buffer*			m_pConstantBuffer1;//�ݽ����ޯ̧.

	ID3D11Buffer*	m_pVertexBuffer;//���_(�ްï��)�ޯ̧.
	ID3D11Buffer**	m_ppIndexBuffer;//���ޯ���ޯ̧.
	//	ID3D11ShaderResourceView*	m_pTexture;//ø���.
	ID3D11SamplerState*			m_pSampleLinear;//ø����̻����-:ø����[�Ɋe��̨����������.

	MY_MATERIAL*	m_pMaterials;//��رٍ\����.
	DWORD			m_NumAttr;//������.
	DWORD			m_AttrID[300];//����ID ��300�����܂�.

	bool			m_bTexture;	//ø����̗L��.

	float			m_fScale;	//�g�k.
	float			m_fYaw;		//��](Y��)
	float			m_fPitch;	//��](X��)
	float			m_fRoll;	//��](Z��)
	D3DXVECTOR3		m_vPos;		//�ʒu(X,Y,Z)

	enDirection		m_enDir;	//����.

	bool			m_bShotFlg;	//�����׸�.
	bool			m_bDeadFlg;	//���S�׸�.

	BBOX			m_BBox;		//B�ޯ���\����.
	SPHERE			m_Sphere;	//�̨��\����.

	D3DXVECTOR3		m_vRay;		//ڲ�̈ʒu.
	D3DXVECTOR3		m_vAxis;	//��]��.
	LPD3DXMESH		m_pMeshForRay;	//ڲ��ү���p.

	D3DXMATRIX m_mYaw;//���W�R������p.

	ID3D11BlendState*	m_pBlendState;//�����޽ð�.

	clsDX9Mesh();	//�ݽ�׸�.
	~clsDX9Mesh();	//�޽�׸�.

	HRESULT Init(HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPSTR fileName);
	//Dx9������.
	HRESULT InitDx9(HWND hWnd);
	//ү���Ǎ�.
	HRESULT LoadXMesh(LPSTR fileName);

	//����ލ쐬.
	HRESULT InitShader();
	//�����ݸޗp(��DX9MESH����Main���łQ���݂���̂Œ���)
	void Render(D3DXMATRIX &mView, D3DXMATRIX &mProj,
		D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye,
		D3DXVECTOR4 vColor, bool alphaFlg);

	//����(��̧������)�ݒ�̐؂�ւ�.
	void SetBlend(bool flg);

	//�ʒu�X�V�֐�.
	D3DXVECTOR3 UpdatePos(D3DXVECTOR3 oldPos);
	//�ʒu�X�V�֐��̏�����.
	void InitPos();
	bool m_bInitPosFlg;
	D3DXVECTOR3 m_vecAxisZ;
	//����֐�.
	HRESULT Release();
};

#endif//#ifndef _DX9MESH_H_