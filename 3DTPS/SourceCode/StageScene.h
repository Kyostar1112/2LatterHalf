#ifndef _C_STAGE_SCENE_H_
#define _C_STAGE_SCENE_H_

#include"BaseScene.h"
#include"Sprite2D.h"
#include "PShot.h"
#include "Player.h"
#include "Sphere.h"
#include "Enemy.h"
#include "Player.h"
#include "Num.h"

namespace ConstantStageScene {
	const int SHOT_MAX = 255;	//ショットの最大数.
	const int SHOT_INTERVAL_CNT = 20;	//ショットの間隔.
	const int ENEMYMAX = 1;	//一度の画面に出てくる敵の最大数.
	const int SPAWNCNT = 60.0f;//敵のスポーンするタイミング.
	const int PLAYERHP = 15;	//プレイヤーの体力.
	const int PLAYERINVINCIBLETIME = 60;	//プレイヤーの無敵時間.
	const int PlayerHpNumdigit = 2;	//自機の体力の桁数.
	const int StageClearTimedigit = 2;	//クリアまでの数字の桁数.
	const int StageEnemyDowndigit = 4;	//敵の撃破したスコアの桁数.
	const int StageClearTimeMax = 60;	//ゲームクリアまでの時間.
}
namespace CSS = ConstantStageScene;

class clsStageScene
	:public clsSceneBase
{
public:
	clsStageScene();
	~clsStageScene();

	void Create();
	void Init();
	void UpDate();
	void ModelRender() {};
	void ModelRender1();
	void ExpRender();
	void ModelRender2();
	void SpriteRender();
	void Release();

	vector<unique_ptr<clsEnemy>>		m_vsmpEnemy;	//ボス.
	vector<unique_ptr<clsPlayerShot>>	m_vsmpShot;	//弾.
	unique_ptr<clsPlayer>		m_smpPlayer;	//自機.
	unique_ptr<clsCharacter>	m_smpGround;	//地面.

	void RightRoll();
	void LeftRoll();
	void Fire();

	SceneMode GetMode()
	{
		return m_stMode;
	}

	void MusicStop()
	{
		m_vsmpBgm[0]->Stop();
		for (int i = 0; i < m_vsmpShotSe.size(); i++)
		{
			m_vsmpShotSe[i]->Stop();
		}
		for (int i = 0; i < m_vsmpHitSe.size(); i++)
		{
			m_vsmpHitSe[i]->Stop();
		}
	}

private:

	int m_icnt;//60カウント用.

	int m_iHp;	//プレイヤ-の体力;
	int m_iClearTime;	//ステージクリアまでの時間.

	int m_iScore;//敵の撃破によるスコア加算.

	SceneMode m_stMode;

	//HP表示.
	unique_ptr<clsNum> m_smpHpNum;

	//残り時間表示.
	unique_ptr<clsNum> m_smpClearTimeNum;

	//スコア表示.
	unique_ptr<clsNum> m_smpScoreNum;

	//ハートマーク.
	unique_ptr<clsSprite2D> m_smpHeart;
	//時計マーク.
	unique_ptr<clsSprite2D> m_smpClock;
	//スコア文字.
	unique_ptr<clsSprite2D> m_smpScoreTxt;

	//Hpゲーシ゛用.
	unique_ptr<clsSprite2D> m_smpHpGage;
	//ダメーシ゛点滅用.
	unique_ptr<clsSprite2D> m_smpDamageImg;

	bool m_bPlayerDamage;//trueがダメーシ゛を受けた状態.
	int	 m_iPlayerinvincible;//0より上は無敵状態.

	//BGM.
	vector<unique_ptr<clsSound>> m_vsmpBgm;
	//SE.
	vector<unique_ptr<clsSound>> m_vsmpShotSe;
	vector<unique_ptr<clsSound>> m_vsmpHitSe;

	int m_iShotIntervalCnt;	//ショットの間隔.

	int m_iEnemyIntervalCnt;//敵の出てくる間隔.

	vector<unique_ptr<clsCharacter>> m_vsmpEnemySphere;
	unique_ptr<clsCharacter> m_smpPlayerSphere;
	vector<unique_ptr<clsCharacter>> m_vsmpShotSphere;

	ModelRenderSet m_stModelRenderSet;

	unique_ptr<clsSprite2D>		m_smpTargetPoint;	//照準.

	//ｽﾌｨｱ作成.
	HRESULT InitSphere(clsDX9Mesh* pMesh, float fScale = 0.7f);
	//ｽﾌｨｱ作成.
	HRESULT InitSphere(D3DXVECTOR3 vPos, float fScale = 0.7f);

	//当たり判定まとめ.
	void HitCheak();

	//衝突判定.
	bool Collision(clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget);
	bool Collision(clsCharacter* pAttacker, clsCharacter* pTarget);

	void Ray(clsEnemy* Enemy);
	//ﾚｲとﾒｯｼｭの当たり判定.
	bool Intersect(
		clsGameObject* pAttacker, clsCharacter* pTarget,
		float* pfDistance, D3DXVECTOR3* pvIntersect);
	//交差位置のﾎﾟﾘｺﾞﾝの頂点を見つける.
	HRESULT FindVerticesOnPoly(
		LPD3DXMESH pTarget, DWORD dwPolyIndex,
		D3DXVECTOR3* pVecVertices);

#if 0

	//ﾊﾞｳﾝﾃﾞｨﾝｸﾞﾎﾞｯｸｽ作成.
	HRESULT InitBBox(clsDX9Mesh* pMesh);
	HRESULT InitBBox(clsD3DXSKINMESH* pMesh);

	//衝突判定(ﾊﾞｳﾝﾃﾞｨﾝｸﾞﾎﾞｯｸｽ).
	bool BBoxCollision(clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget);
	bool BBoxCollision(clsD3DXSKINMESH* pAttacker, clsD3DXSKINMESH* pTarget);

#endif // 0
};

#endif  //#ifdef _C_STAGE_H_