#ifndef _C_CLEAR_SCENE_H_
#define _C_CLEAR_SCENE_H_

#include"BaseScene.h"
#include"Sp2DMgrPushButn.h"

namespace ClearScene {
}

class clsClearScene
	:public clsSceneBase
{
public:
	clsClearScene();
	~clsClearScene();

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
	unique_ptr<clsSprite2D> m_smpClearBack;
	unique_ptr<clsSprite2D> m_smpClearTxt;

	unique_ptr<clsPushTxt> m_smpPushTxt;

	unique_ptr<clsSound>   m_smpBgm;
};

#endif  //#ifdef _C_CLEAR_H_