//========================================================================================
//
//	��[dinput.h]��Ǎ��ލۂɂ��Ȃ���΂����Ȃ����ƁB
//
//		[°�]��[��߼��]��[��ۼު�Ă���ѿح����]��
//			[�S��]��[VC++ �ިڸ��]��[�ިڸ�؂�\��������ۼު��]��
//				[�ݸٰ�� ̧��]
//					C:\Program Files\Microsoft DirectX SDK (June 2010)\Include
//				[ײ���� ̧��]
//					C:\Program Files\Microsoft DirectX SDK (June 2010)\Lib\x86
//
//		[��ۼު��]��[�����������è]��[�\�������è]��
//			[�ݶ]��[����]��[�ǉ��̈ˑ�̧��]
//					dxguid.lib dinput8.lib			��DirectInput �Ŏg�p����K�v�Œ���B
//
//		�ǉ��ˑ�̧�ق́A���L�� #pragma comment( lib, "����.lib") �ő�p�\�B
//		���̃v���W�F�N�g�ɃR�[�h���ڐA���邱�Ƃ��l����Ƃ�����̕����y�ł悢�B
//
//========================================================================================

#ifndef _DX_INPUT_H_
#define _DX_INPUT_H_

#include <dinput.h>	// DirectInput�p.

// DirectX �֌W.
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "dinput8.lib" )	// �R���g���[������Ŏg�p.

//�񋓑̐錾.
//�����.
enum enPKey{
	enPKey_Up,
	enPKey_Down,
	enPKey_Left,
	enPKey_Right,
	enPKey_00,
	enPKey_01,
	enPKey_02,
	enPKey_03,
	enPKey_04,
	enPKey_05,
	enPKey_06,
	enPKey_07,
	enPKey_08,
	enPKey_09,
	enPKey_10,
	enPKey_11,

	enPKey_Max,	//���ő吔.
};

class clsDxInput
{
public:
	clsDxInput();
	~clsDxInput();

	//DirectInput�����ݒ�.
	bool initDI(HWND hWnd);

	//���͏��X�V�֐�.
	HRESULT UpdateInputState();

	//���͏���ǉ�����֐�.
	void AddInputState(enPKey enKey);

	//���͏�������������֐�.
	void InitInputState();

	//������������֐�.
	bool IsPressKey(enPKey enKey);

private:
	LPDIRECTINPUT8			m_pDI;//DxInput��޼ު��.
	LPDIRECTINPUTDEVICE8	m_pPad;//���޲�(���۰�)��޼ު��.

	unsigned int			m_uInputState;//���͏��.
};

#endif//#ifndef _DX_INPUT_H_
