#ifndef _C_OVER_SCENE_H_
#define _C_OVER_SCENE_H_

#include"BaseScene.h"
#include"Sp2DMgrPushButn.h"
namespace OverScene{
}

class clsOverScene
	:public clsSceneBase
{
public:
	clsOverScene();
	~clsOverScene();

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
	unique_ptr<clsSprite2D> m_smpOverBack;
	unique_ptr<clsSprite2D> m_smpOverTxt;

	unique_ptr<clsPushTxt> m_smpPushTxt;

	unique_ptr<clsSound>   m_smpBgm;
};

#endif  //#ifdef _C_OVER_H_