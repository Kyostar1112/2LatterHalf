#ifndef _SPRITE_H_
#define _SPRITE_H_

//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning( disable:4005 )

#include "Common.h"//���ʸ׽.

#include "Resource.h".

//======================================
//	�\����.
//======================================
//�ݽ����ޯ̧�̱��ؑ��̒�`(Simple.hlsl)
//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
struct SPRITESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX	mWVP;		//ܰ���,�ޭ�,�ˉe�̍����ϊ��s��.
	D3DXVECTOR4	vColor;		//�װ(RGBA�̌^�ɍ��킹��)
	D3DXVECTOR4	vUV;		//UV���W.
};

/*************************************************
*	Sprite�׽.
**/
class clsSprite
	: public clsCommon	//���ʸ׽���p��.
{
private:
	//�����ق̎�ނ��Ƃɗp��.
	ID3D11VertexShader*		m_pVertexShader;//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;	//�߸�ټ����.
	ID3D11Buffer*			m_pConstantBuffer;//�ݽ����ޯ̧.

	//�����ق��Ƃɗp��.
	ID3D11Buffer*			m_pVertexBuffer;//���_�ޯ̧.

	ID3D11ShaderResourceView*	m_pTexture;//ø���.
	ID3D11SamplerState*			m_pSampleLinear;//ø����̻����-:ø����[�Ɋe��̨����������.

protected:

public:
	D3DXVECTOR3		m_vPos;	//�ʒu.
	D3DXVECTOR4 m_texUV;
	int				m_AnimCount;//UV��۰ٗp�̶���.

	bool			m_bDispFlg;	//�\���׸�.

	clsSprite();	//�ݽ�׸�.
	~clsSprite();	//�޽�׸�.

	//������.
	HRESULT Init();

	//����ލ쐬.
	HRESULT InitShader();
	//���ٍ쐬.
	HRESULT InitModel();

	//�`��(�����ݸ�)
	void Render();
};

#endif//#ifndef _SPRITE_H_
