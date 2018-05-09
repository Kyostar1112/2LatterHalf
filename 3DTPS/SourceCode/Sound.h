#ifndef _C_SOUND_H_
#define _C_SOUND_H_

#include <Windows.h>

//ライブラリ読込.
#pragma comment( lib, "winmm.lib" )

//文字列バッファ.
#define STR_BUFF_MAX	256

/********************************************************
*
*	サウンドクラス.
*		midi, mp3, wav形式ファイルの再生・停止等を行う.
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

	//音声ファイルを開く.
	bool Open(LPCTSTR sFName, char *sAlias, HWND hWnd);

	//音声ファイルを閉じる.
	bool Close();

	//再生関数.
	bool Play(bool bNotify = true);

	//停止関数.
	bool Stop();

	//状態を取得する関数.
	bool GetStatus(char *sStatus);

	//音の停止を確認する関数.
	bool IsStopped();

	//音の再生中を確認する関数.
	bool IsPlaying();

	//再生位置を初めに戻す関数.
	bool SeekToStart();

	//音量を設定する関数.
	bool SetVolume(int iVolume);

	//再生されて再生位置を戻したかどうか.
	bool	m_bSeekFlg;

	SoundData m_stSoundData;

private:
	HWND	m_hWnd;					//ウィンドウハンドル.
	char	m_sAlias[STR_BUFF_MAX];	//エイリアス.

	//初期設定.
	void SetInitParam(char *sAlias, HWND hWnd);
};

#endif//#ifndef _C_SOUND_H_