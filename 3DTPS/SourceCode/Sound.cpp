#include "Sound.h"
#include <stdio.h>

//ｺﾝｽﾄﾗｸﾀ.
clsSound::clsSound()
{
	ZeroMemory(this, sizeof(clsSound));
	m_bSeekFlg = true;
}

//ﾃﾞｽﾄﾗｸﾀ.
clsSound::~clsSound()
{
}

//音声ﾌｧｲﾙを開く.
bool clsSound::Open(LPCTSTR sFName, char *sAlias, HWND hWnd)
{
	//初期設定.
	SetInitParam(sAlias, hWnd);

	//ｺﾏﾝﾄﾞ.
	char command[STR_BUFF_MAX] = "open ";

	strcat_s(command, sizeof(command), sFName);//ﾌｧｲﾙ名を結合.
	strcat_s(command, sizeof(command), " alias ");
	strcat_s(command, sizeof(command), m_sAlias);//ｴｲﾘｱｽを結合.

	if (mciSendString(command, NULL, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}

//音声ﾌｧｲﾙを閉じる.
bool clsSound::Close()
{
	//ｺﾏﾝﾄﾞ.
	char command[STR_BUFF_MAX] = "close ";

	strcat_s(command, sizeof(command), m_sAlias);//ｴｲﾘｱｽを結合.

	if (mciSendString(command, NULL, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}

//再生関数.
bool clsSound::Play(bool bNotify)
{
	//ｺﾏﾝﾄﾞ.
	char command[STR_BUFF_MAX] = "play ";

	strcat_s(command, sizeof(command), m_sAlias);//ｴｲﾘｱｽを結合.
	if (bNotify) {
		strcat_s(command, sizeof(command), " notify");
		m_bSeekFlg = false;
		//notify:処理が終了したときにhWndにMM_MCINOTIFYﾒｯｾｰｼﾞをﾎﾟｽﾄする.
	}
	if (mciSendString(command, NULL, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}

//停止関数.
bool clsSound::Stop()
{
	//ｺﾏﾝﾄﾞ.
	char command[STR_BUFF_MAX] = "stop ";

	strcat_s(command, sizeof(command), m_sAlias);//ｴｲﾘｱｽを結合.

	if (mciSendString(command, NULL, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}

//状態を取得する関数.
//	sStatus の配列数は 256以下にすること.
//※ただし、状態を取得する場合は、再生時に「notify」を設定し、
//  ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙにﾒｯｾｰｼﾞを送っておく必要がある.
bool clsSound::GetStatus(char *sStatus)
{
	//ｺﾏﾝﾄﾞ.
	char command[STR_BUFF_MAX] = "status ";

	strcat_s(command, sizeof(command), m_sAlias);//ｴｲﾘｱｽを結合.
	strcat_s(command, sizeof(command), " mode");

	if (mciSendString(command, sStatus, STR_BUFF_MAX, m_hWnd) == 0) {
		return true;
	}
	return false;
}

//音の停止を確認する関数.
bool clsSound::IsStopped()
{
	char sStatus[STR_BUFF_MAX] = "";

	//状態の取得.
	if (GetStatus(sStatus)) {
		//文字列比較.
		if (strcmp(sStatus, "stopped") == 0) {
			return true;
		}
	}
	return false;
}

//音の再生中を確認する関数.
bool clsSound::IsPlaying()
{
	char sStatus[STR_BUFF_MAX] = "";

	//状態の取得.
	if (GetStatus(sStatus)) {
		//文字列比較.
		if (strcmp(sStatus, "playing") == 0) {
			return true;
		}
	}
	return false;
}

//再生位置を初めに戻す関数.
bool clsSound::SeekToStart()
{
	if (!m_bSeekFlg)
	{
		//ｺﾏﾝﾄﾞ.
		char command[STR_BUFF_MAX] = "seek ";

		strcat_s(command, sizeof(command), m_sAlias);//ｴｲﾘｱｽを結合.
		strcat_s(command, sizeof(command), " to start");

		if (mciSendString(command, NULL, 0, m_hWnd) == 0) {
			m_bSeekFlg = true;
			return true;
		}
		m_bSeekFlg = true;
	}
	return false;
}

//音量を設定する関数.
bool clsSound::SetVolume(int iVolume)
{
	//範囲内に丸める.
	if (iVolume < 0) { iVolume = 0; }
	if (iVolume > 1000) { iVolume = 1000; }

	//音量を文字列に変換.
	char sVolume[STR_BUFF_MAX] = "";
	sprintf_s(sVolume, sizeof(sVolume), "%d", iVolume);

	//ｺﾏﾝﾄﾞ.
	char command[STR_BUFF_MAX] = "setaudio ";

	strcat_s(command, sizeof(command), m_sAlias);//ｴｲﾘｱｽを結合.
	strcat_s(command, sizeof(command), " volume to ");
	strcat_s(command, sizeof(command), sVolume);//音量を結合.

	if (mciSendString(command, NULL, 0, m_hWnd) == 0) {
		return true;
	}
	return false;
}

//初期設定.
void clsSound::SetInitParam(char *sAlias, HWND hWnd)
{
	//ｳｨﾝﾄﾞｳﾊﾝﾄﾞﾙを渡す.
	m_hWnd = hWnd;

	//ｴｲﾘｱｽを渡す.
	strcpy_s(m_sAlias, sizeof(m_sAlias), sAlias);
}