#ifndef _C_SOUND_H_
#define _C_SOUND_H_

#include <Windows.h>

//ײ���ؓǍ�.
#pragma comment( lib, "winmm.lib" )

//�������ޯ̧.
#define STR_BUFF_MAX	256

/********************************************************
*
*	����޸׽.
*		midi, mp3, wav�`��̧�ق̍Đ��E��~�����s��.
*
**/

struct SoundData
{
	char sPath[256];
	char sAilias[256];
};

class clsSound
{
public:
	clsSound();
	~clsSound();

	//����̧�ق��J��.
	bool Open(LPCTSTR sFName, char *sAlias, HWND hWnd);

	//����̧�ق����.
	bool Close();

	//�Đ��֐�.
	bool Play(bool bNotify = true);

	//��~�֐�.
	bool Stop();

	//��Ԃ��擾����֐�.
	bool GetStatus(char *sStatus);

	//���̒�~���m�F����֐�.
	bool IsStopped();

	//���̍Đ������m�F����֐�.
	bool IsPlaying();

	//�Đ��ʒu�����߂ɖ߂��֐�.
	bool SeekToStart();

	//���ʂ�ݒ肷��֐�.
	bool SetVolume(int iVolume);

	//�Đ�����čĐ��ʒu��߂������ǂ���.
	bool	m_bSeekFlg;

	SoundData m_stSoundData;

private:
	HWND	m_hWnd;					//����޳�����.
	char	m_sAlias[STR_BUFF_MAX];	//��ر�.

	//�����ݒ�.
	void SetInitParam(char *sAlias, HWND hWnd);
};

#endif//#ifndef _C_SOUND_H_