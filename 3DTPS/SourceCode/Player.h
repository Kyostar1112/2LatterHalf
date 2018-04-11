#ifndef _C_PLAYER_H_
#define _C_PLAYER_H_

#include"Character.h"

class clsPlayer
	:public clsCharacter
{
public:
	clsPlayer();
	~clsPlayer();

#if 0
	bool GetEnableFlg()
	{
		return m_bEnableFlg;
	}

	void SetEnableFlg(bool EnableFlg)
	{
		m_bEnableFlg = EnableFlg;
	}

#endif // 0

private:
#if 0
	bool m_bEnableFlg;//Žg—p‚µ‚Ä‚¢‚é‚©‚Ç‚¤‚©.
#endif // 0
};

#endif  //#ifdef _C_PLAYER_H_