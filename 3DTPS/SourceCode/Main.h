//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning( disable:4005 )

#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>

#include "DX9Mesh.h"	//Dx9���g�p����׽.
#include "DebugText.h"	//���ޯ��÷�ĸ׽.
#include "D3DXSKINMESH.h"	//���ү���׽.

#include "DxInput.h"	//���۰׸׽.
#include "Ray.h"		//ڲ�\���׽.

#include <thread>

//�V�[���N���X�B.
#include "TitleScene.h"
#include "StageScene.h"
#include "OverScene.h"
#include "ClearScene.h"
#include "ResultScene.h"

//======================================
//	ײ����.
//======================================
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )//�uD3DX�`�v�̒�`�g�p���ɕK�v.

//======================================
//	�萔.
//======================================
#define WND_TITLE	"�f�B�t�F���T�["
#define APR_NAME	"�f�B�t�F���T�["

const float WSPACE = 0.8f;	//�ǂƂ̌��E����.

//======================================
//	�\����.
//======================================
//��׏��.
struct CAMERA
{
	D3DXVECTOR3	vEye;	//��׈ʒu.
	D3DXVECTOR3	vLook;	//�����ʒu.
	float		fYaw;	//��]�l.
	D3DXMATRIX	mRot;	//��]�s��.
};

/*************************************************
*	Ҳݸ׽.
**/
class clsMain
{
private:
	HWND	m_hWnd;	//����޳�����.

public:
	clsMain();	//�ݽ�׸�.
	~clsMain();	//�޽�׸�.

	//����޳�������֐�.
	HRESULT InitWindow(
		HINSTANCE hInstance,
		INT x, INT y, INT width, INT height,
		LPSTR WindowName);

	//����޳�֐�(ү���ޖ��̏���)
	LRESULT MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);

	//ү����ٰ��.
	void Loop();
	//���ع����Ҳݏ���.
	void AppMain();
	//�`��(�����ݸ�)
	void Render();

private:
	//�����؂ɂЂƂ�.
	ID3D11Device*			m_pDevice;	//���޲���޼ު��.
	ID3D11DeviceContext*	m_pDeviceContext;//���޲���÷��.
	IDXGISwapChain*			m_pSwapChain;//�ܯ������.
	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;//���ް���ޯ��ޭ�.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;//�ޯ��ޯ̧.
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;//���߽��ݼ��ޭ�.

	D3DXVECTOR3		m_vLight;		//ײĂ̕���.

	CAMERA			m_Camera;		//��׏��.

	D3DXMATRIX		m_mView;		//�ޭ�(���)�s��.
	D3DXMATRIX		m_mProj;		//��ۼު���ݍs��.

	int					m_AnimationNo;//��Ұ��ݔԍ�.
	double				m_AnimTime;	//��Ұ��ݎ���.

	//���ޯ��÷�ĸ׽.
	unique_ptr<clsDebugText>	m_smpText;

	//�[�x(Z)ýĐݒ�.
	ID3D11DepthStencilState* m_pDepthStencilState;

	//���۰׸׽.
	unique_ptr<clsDxInput>	m_smpDxInput;

	//ڲ�\���׽.
	unique_ptr<clsRay>			m_smpRayV;	//����.
	unique_ptr<clsRay>			m_smpRayFB;	//�O��.
	unique_ptr<clsRay>			m_smpRayH;	//���E.

	//�L�����N�^�N���X.
	int	m_iAnimModelArrayMax;

	clsResource*				m_pResource;//���\�[�X�N���X.

	//�����݂̃V�[��.
	SceneMode m_enScene;
	//�V�[�����ς�����u�Ԃ��Ƃ邽�߂̕ϐ�.
	SceneMode m_enOldScene;
	//�V�[����.
	vector<string> m_vsSceneName;

	//�V�[���N���X�B.
	unique_ptr<clsTitleScene>	m_smpTitleScene;
	unique_ptr<clsStageScene>	m_smpStageScene;
	unique_ptr<clsOverScene>	m_smpOverScene;
	unique_ptr<clsClearScene>	m_smpClearScene;
	unique_ptr<clsResultScene>	m_smpResultScene;

	unique_ptr<clsSound>	m_smpSeClick;

	void SceneChange();
	bool m_bTitleFlg;

public:
	//DirectX������.
	HRESULT InitD3D();
	//Direct3D�I������.
	void DestroyD3D();

	//ү���Ǎ��֐��i�܂Ƃ߂��j
	HRESULT MeshRead();

	//�[�xý�(Zý�) ON/OFF�ؑ�.
	void SetDepth(bool bFlg);

	//ڲ��ү���̓����蔻��.
	bool Intersect(
		clsGameObject* pAttacker, clsCharacter* pTarget,
		float* pfDistance, D3DXVECTOR3* pvIntersect);
	//�����ʒu����غ�݂̒��_��������.
	HRESULT FindVerticesOnPoly(
		LPD3DXMESH pTarget, DWORD dwPolyIndex,
		D3DXVECTOR3* pVecVertices);

	//�ǂ̓����蔻��֘A.
	void WallJudge(clsGameObject* pAttacker, clsCharacter* pWall);
	void dirOverGuard(float* fYaw);//��]�l����.

	//��׊֐�.
	void Camera();
	//��ۼު���݊֐�.
	void Proj();
};