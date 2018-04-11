#ifndef _C_PLAYER_SHOT_H_
#define _C_PLAYER_SHOT_H_

#include"Character.h"
namespace Shot{
	const int SHOT_FIRING_CNT = 100;		//ショットの発射されてからの生存時間の最大値.
}

class clsPlayerShot
	:public clsCharacter
{
public:
	clsPlayerShot();
	~clsPlayerShot();

	bool GetShotFlg()
	{
		return m_bShotFlg;
	}

	void Fire()
	{
		m_bShotFlg = true;
		m_iShotFiringCnt = Shot::SHOT_FIRING_CNT;
		m_fSpd = 2.0f;
	}

	void FiringCnt()
	{
		if (m_bShotFlg)
		{
			m_iShotFiringCnt--;
			if (m_iShotFiringCnt < 0)
			{
				m_bShotFlg = false;
			}
		}
	}

private:
	bool m_bShotFlg;//trueなら発射している.
	int  m_iShotFiringCnt;//trueなら発射している.
};

#endif  //#ifdef _C_PLAYER_SHOT_H_