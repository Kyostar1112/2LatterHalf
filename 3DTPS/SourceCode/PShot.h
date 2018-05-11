#ifndef _C_PLAYER_SHOT_H_
#define _C_PLAYER_SHOT_H_

#include"Character.h"
namespace Shot {
	const int SHOT_FIRING_CNT = 100;		//ショットの発射されてからの生存時間の最大値.
}

class clsPlayerShot
	:public clsCharacter
{
public:
	clsPlayerShot();
	~clsPlayerShot();

	//
	bool GetShotFlg();

	void Fire();

	void FireInterval();

private:
	bool m_bShotFlg;//trueなら発射している.
	int  m_iShotFiringCnt;//trueなら発射している.
};

#endif  //#ifdef _C_PLAYER_SHOT_H_