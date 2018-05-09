#ifndef _C_TITLE_SCENE_H_
#define _C_TITLE_SCENE_H_

#include"BaseScene.h"
#include"Sp2DMgrPushButn.h"

namespace TitleScene {
}

class clsTitleScene
	:public clsSceneBase
{
public:
	clsTitleScene();
	~clsTitleScene();

	void Create();
	void Init();
	void UpDate();
	void ModelRender();
	void SpriteRender();
	void Release();

	void MusicStop()
	{
		m_smpBgm->Stop();
	}

private:
	unique_ptr<clsSprite2D> m_smpTitleLogo;
	unique_ptr<clsSprite2D> m_smpTitleBack;

	unique_ptr<clsPushTxt> m_smpPushTxt;

	unique_ptr<clsSound>   m_smpBgm;
};

#endif  //#ifdef _C_TITLE_SCENE_H_