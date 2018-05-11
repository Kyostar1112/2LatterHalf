#include"PShot.h"

clsPlayerShot::clsPlayerShot()
{
	m_bShotFlg = false;
}

clsPlayerShot::~clsPlayerShot()
{
}

bool clsPlayerShot::GetShotFlg()
{
	return m_bShotFlg;
}

void clsPlayerShot::Fire()
{
	m_bShotFlg = true;
	m_iShotFiringCnt = Shot::SHOT_FIRING_CNT;
	m_fSpd = 2.0f;
}

void clsPlayerShot::FireInterval()
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
