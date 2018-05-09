#ifndef _C_SCENE_H_
#define _C_SCENE_H_

#define _CRT_SECURE_NO_WARNINGS

#include "Resource.h"

#include "Character.h"		//キャラクタクラス.
#include "CharacterSkin.h"	//キャラクタスキンクラス.

#include "Sprite2D.h"		//Uiクラス.

#include "Sound.h"			//音楽クラス.
#include <algorithm>

#define Resource clsResource::GetInstance()

namespace SceneBase {
}

//======================================
//	列挙体.
//======================================
//シーンモード.
enum SceneMode
{
	Title = 0,
	Stage,
	Over,
	Clear,
	Result,
};
class clsSceneBase
{
public:
	clsSceneBase();
	virtual ~clsSceneBase();

	virtual void Create() = 0;//ゲーム起動時の初期化関数.
	virtual void Init() = 0;//シーンごとの初期化関数.
	virtual void UpDate() = 0;//ずっと回すよ.
	virtual void ModelRender() = 0;//ずっと回すよモデル表示だけこっちに.
	virtual void SpriteRender() = 0;//ずっと回すよ表示だけこっちに.
	virtual void MusicStop() = 0;//音楽止めて.
	virtual void Release() = 0;//破棄するよ.

	void BlackOut();
	void BlackStart(float ChaAmo, bool BFlg = true);

	//暗転機能が動いているかどうかbyInoue.
	bool GetBlackIdle() {
		if (m_smpBlack->m_BlackMode == Idle &&
			m_smpWhite->m_BlackMode == Idle)
		{
			return true;
		}
		return false;
	}

	//暗転しきった瞬間を知らせるbyTahara.
	bool GetBlackEnd() {
		if (m_smpBlack->m_BlackMode == Change ||
			m_smpWhite->m_BlackMode == Change)
		{
			return true;
		}
		return false;
	}

private:

	unique_ptr<clsSprite2D> m_smpBlack;	//暗転用.
	unique_ptr<clsSprite2D> m_smpWhite;	//暗転用.

	float	m_fChangePoint;
	bool	m_bColor;
};

#endif  //#ifdef _C_SCENE_H_