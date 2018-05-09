#include "DxInput.h"

LPDIRECTINPUT8 pDI2 = NULL;			// DxInput�I�u�W�F�N�g
LPDIRECTINPUTDEVICE8 pPad2 = NULL;	// �f�o�C�X(�R���g���[��)�I�u�W�F�N�g

// �W���C�X�e�B�b�N�񋓊֐�
BOOL CALLBACK EnumJoysticksCallBack(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	HRESULT hRlt;	// �֐����A�l

	// �f�o�C�X(�R���g���[��)�̍쐬
	hRlt = pDI2->CreateDevice(
		pdidInstance->guidInstance,	// �f�o�C�X�̔ԍ�
		&pPad2,	// (out)�쐬�����f�o�C�X�I�u�W�F�N�g
		NULL);
	if (hRlt != DI_OK) {
		return DIENUM_CONTINUE;	// ���̃f�o�C�X��v��
	}
	return DIENUM_STOP;	// �񋓒�~
}

// �I�u�W�F�N�g�̗񋓊֐�
BOOL CALLBACK EnumObjectsCallBack(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	// ��(�X�e�B�b�N)�������Ă��邩�H
	if (pdidoi->dwType & DIDFT_AXIS)
	{
		// ���͏��͈̔͂�ݒ肷��
		DIPROPRANGE diprg;	// �͈͐ݒ�\����

		diprg.diph.dwSize = sizeof(DIPROPRANGE);			// �\���̂̃T�C�Y
		diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);	// �w�b�_�[���̃T�C�Y
		diprg.diph.dwObj = pdidoi->dwType;					// ��(�I�u�W�F�N�g)
		diprg.diph.dwHow = DIPH_BYID;						// dwObj�ɐݒ肳�����̂�
		// �I�u�W�F�N�g�̎�ނ��H�C���X�^���X�ԍ����H

		diprg.lMax = +1000;	// �ő�l
		diprg.lMin = -1000;	// �ŏ��l

		// �͈͂�ݒ�
		if (FAILED(pPad2->SetProperty(
			DIPROP_RANGE,	// �͈�
			&diprg.diph)))// �͈͐ݒ�\����
		{
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}

//�ݽ�׸�.
clsDxInput::clsDxInput()
	: m_pDI(NULL)
	, m_pPad(NULL)
{
}

//�޽�׸�.
clsDxInput::~clsDxInput()
{
}

// DirectInput�̏����ݒ�B
bool clsDxInput::initDI(HWND hWnd)
{
	HRESULT hRlt;	// �֐����A�l.
	hRlt = DirectInput8Create(
		GetModuleHandle(NULL),	// DxInput���쐬����A�v���̃n���h��
		DIRECTINPUT_VERSION,	// �Œ�FDxInput�̃o�[�W����
		IID_IDirectInput8,		// �Œ�FUnicode��Ansi�̋�ʗp
		(VOID**)&m_pDI,			// (out)DxInput�I�u�W�F�N�g
		NULL);					// NULL�Œ�
	if (hRlt != DI_OK) {
		MessageBox(NULL, "DxInput�̍쐬�Ɏ��s", "�G���[", MB_OK);
		return false;
	}

	pDI2 = m_pDI;

	// ���p�\�ȃR���g���[����T��(�񋓂���)
	hRlt = (*m_pDI).EnumDevices(
		DI8DEVCLASS_GAMECTRL,	// �S�ẴQ�[���R���g���[��
		EnumJoysticksCallBack,	// �R���g���[���̗񋓊֐�
		NULL,					// �R�[���o�b�N�֐�����̒l
		DIEDFL_ATTACHEDONLY);	// �q�����Ă����ɂ̂�
	if (hRlt != DI_OK) {
		MessageBox(NULL, "���۰ׂ̊m�F�Ɏ��s", "�G���[", MB_OK);
	}

	m_pPad = pPad2;

	// �R���g���[���̐ڑ��m�F
	if (pPad2 == NULL) {
		//MessageBox( NULL, "���۰ׂ��ڑ�����Ă��܂���", "�G���[", MB_OK );
	}
	else {
		// �R���g���[���\���̂̃f�[�^�t�H�[�}�b�g���쐬
		hRlt = pPad2->SetDataFormat(
			&c_dfDIJoystick2);	//�Œ�
		if (hRlt != DI_OK) {
			MessageBox(NULL, "�ް�̫�ϯĂ̍쐬���s", "�G���[", MB_OK);
		}
		// (���̃f�o�C�X�Ƃ�)�������x���̐ݒ�
		hRlt = pPad2->SetCooperativeLevel(
			hWnd,
			DISCL_EXCLUSIVE |	// �r���A�N�Z�X
			DISCL_FOREGROUND);	// �t�H�A�O���E���h�A�N�Z�X��
		if (hRlt != DI_OK) {
			MessageBox(NULL, "�������ق̐ݒ莸�s", "�G���[", MB_OK);
		}
		// �g�p�\�ȃI�u�W�F�N�g(�{�^���Ȃ�)�̗�
		hRlt = pPad2->EnumObjects(
			EnumObjectsCallBack,	// �I�u�W�F�N�g�񋓊֐�
			(VOID*)hWnd,			// �R�[���o�b�N�֐��ɑ�����
			DIDFT_ALL);			// �S�ẴI�u�W�F�N�g
		if (hRlt != DI_OK) {
			MessageBox(NULL, "��޼ު�Ă̗񋓂Ɏ��s", "�G���[", MB_OK);
		}
	}

	return true;
}

//���͏��X�V�֐�.
HRESULT clsDxInput::UpdateInputState()
{
	HRESULT hRslt;	//�֐����A�l.
	DIJOYSTATE2 js;	//�ޮ��è�����\����.

	//���͏��̏�����.
	InitInputState();

	//�ޮ��è���̐ڑ��m�F.
	if (m_pPad == NULL) {
		return E_FAIL;
	}

	//�ޮ��è�����ύX����Ă��邩���m�F.
	hRslt = m_pPad->Poll();
	if (hRslt != DI_OK && hRslt != DI_NOEFFECT)//DI_NOEFFECT:�m�F�s�v�����޲�.
	{
		//���۰ׂւ̱��������擾����.
		hRslt = m_pPad->Acquire();
		while (hRslt == DIERR_INPUTLOST) {
			hRslt = m_pPad->Acquire();
		}
		return S_OK;
	}

	//���۰ׂ��璼���ް����擾����.
	hRslt = m_pPad->GetDeviceState(
		sizeof(DIJOYSTATE2),	//�擾���黲��.
		&js);					//(out)�擾�ް�.
	if (hRslt != DI_OK) {
		return hRslt;
	}

	//����۸޽è��(�è���̌X���(�V��)�̒l��500,-500�Ƃ��čl����)
	if (js.lX > 500) {
		//�E��.
		AddInputState(enPKey_Right);
	}
	else if (js.lX < -500) {
		//����.
		AddInputState(enPKey_Left);
	}
	if (js.lY > 500) {
		//�㷰.
		AddInputState(enPKey_Down);
	}
	else if (js.lY < -500) {
		//����.
		AddInputState(enPKey_Up);
	}

	//�\����.
	//switch( js.rgdwPOV[0] ){
	//case 4500:	//�E��.
	//	AddInputState( enPKey_Up );
	//	AddInputState( enPKey_Right );
	//	break;
	//case 13500:	//�E��.
	//	AddInputState( enPKey_Down );
	//	AddInputState( enPKey_Right );
	//	break;
	//case 22500:	//����.
	//	AddInputState( enPKey_Down );
	//	AddInputState( enPKey_Left );
	//	break;
	//case 31500:	//����.
	//	AddInputState( enPKey_Up );
	//	AddInputState( enPKey_Left );
	//	break;
	//case 0:		//��.
	//	AddInputState( enPKey_Up );
	//	break;
	//case 9000:	//�E.
	//	AddInputState( enPKey_Right );
	//	break;
	//case 18000:	//��.
	//	AddInputState( enPKey_Down );
	//	break;
	//case 27000:	//��.
	//	AddInputState( enPKey_Left );
	//	break;
	//default:
	//	break;
	//}

	//����(�񋓑̂������Ă��Ή����y�ȏ�����)
	//��ٰ�ߊJ�n�ʒu�������Ώۂ����݂ɐݒ肷��.
	for (int iKey = enPKey_00; iKey < enPKey_Max; iKey++)
	{
		//���۰ׂ����݂̔z��J�n�ԍ���0����Ȃ̂ŊJ�n�ʒu�����炷.
		if (js.rgbButtons[iKey - enPKey_00] & 0x80) {
			//ٰ�ߕϐ���񋓑̌^�ɷ��Ă��ēn��.
			AddInputState((enPKey)iKey);
		}
	}

	return S_OK;
}

//���͏���ǉ�����֐�.
void clsDxInput::AddInputState(enPKey enKey)
{
	// << ��ĉ��Z�q:���ɼ��.
	m_uInputState |= 1 << enKey;
}

//���͏�������������֐�.
void clsDxInput::InitInputState()
{
	m_uInputState = 0;
}

//������������֐�.
//��UpdateInputState()�֐��œ��͏�񂪍X�V����Ă��邱�Ƃ��O��.
bool clsDxInput::IsPressKey(enPKey enKey)
{
	// >> ��ĉ��Z�q:�E�ɼ��.
	if ((m_uInputState >> enKey) & 1) {
		return true;
	}
	return false;
}