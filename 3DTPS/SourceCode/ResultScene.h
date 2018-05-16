#ifndef _C_RESULT_SCENE_H_
#define _C_RESULT_SCENE_H_

#include"BaseScene.h"
#include"Sp2DMgrPushButn.h"
#include"Num.h"

class clsResultScene
	:public clsSceneBase
{
public:
	clsResultScene();
	~clsResultScene();

	void Create();
	void Init();
	void UpDate();
	void Render();
	void Release();

	void MusicStop()
	{
		m_smpBgm->Stop();
	}

private:
	unique_ptr<clsSprite2D> m_smpBack;
	unique_ptr<clsSprite2D> m_smpTxt;

	unique_ptr<clsPushTxt> m_smpPushTxt;

	//ƒ‰ƒ“ƒLƒ“ƒO•\Ž¦.
	vector<unique_ptr<clsNum>> m_vsmpDispScore;

	vector<unique_ptr<clsNum>> m_vsmpScoreRank;

	vector<int> m_viScore;
	unique_ptr<clsSound>   m_smpBgm;
};

#endif  //#ifdef _C_RESULT_SCENE_H_