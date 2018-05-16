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

#include <math.h>

namespace ConstantStageScene {
	const int iShotMax = 255;			//ショットの最大数.
	const int iShotInterValCnt = 20;	//ショットの間隔.
	const int iEnemyMax = 10;			//一度の画面に出てくる敵の最大数.
	const int iSpawnCnt = 60;			//敵のスポーンするタイミング.
	const int iPlayerHp = 15;			//プレイヤーの体力.
	const int PlayerInvincible = 60;//プレイヤーの無敵時間.
	const int iPlayerHpNumdigit = 2;		//自機の体力の桁数.
	const int iStageClearTimedigit = 2;	//クリアまでの数字の桁数.
	const int iStageEnemyDowndigit = 4;	//敵の撃破したスコアの桁数.
	const int iStageClearTimeMax = 60;	//ゲームクリアまでの時間.
	const float fTagetAngle = 20.0f;		//ターゲットの照準を色を変える角度.
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

	void RightRoll(float MovePoint);
	void LeftRoll(float MovePoint);
	void Fire();

	SceneMode GetMode()
	{
		return m_stMode;
	}

	void MusicStop()
	{
		m_vsmpBgm[0]->Stop();
		for (UINT i = 0; i < m_vsmpShotSe.size(); i++)
		{
			m_vsmpShotSe[i]->Stop();
		}
		for (UINT i = 0; i < m_vsmpHitSe.size(); i++)
		{
			m_vsmpHitSe[i]->Stop();
		}
	}
	vector<float> InnerProduct;

private:

	int m_icnt;//60カウント用.

	int m_iHp;	//プレイヤ-の体力;
	int m_iClearTime;	//ステージクリアまでの時間.

	int m_iScore;//敵の撃破によるスコア加算.

	//アニメーション経過時間
	double m_dAnimTime;

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

	unique_ptr<clsSprite2D>		m_smpTargetPoint;		//照準.
	unique_ptr<clsSprite2D>		m_smpTargetPointHit;	//照準当たる位置のみ.
	bool m_bTargetPointHitCheackFlg;

	//スフィア作成.
	HRESULT InitSphere(clsDX9Mesh* pMesh, float fScale = 0.7f);
	//スフィア作成.
	HRESULT InitSphere(D3DXVECTOR3 vPos, float fScale = 0.7f);

	//当たり判定まとめ.
	void HitCheak();

	//衝突判定.
	bool Collision(clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget);
	bool Collision(clsCharacter* pAttacker, clsCharacter* pTarget);

	D3DXVECTOR3 IntersectionLocation(clsGameObject* AttackObject, clsCharacter* DefenceObject, D3DXVECTOR3 RayOrientation);

	//レイとメッシュの当たり判定.
	bool Intersect(
		clsGameObject* pAttacker, clsCharacter* pTarget,
		float* pfDistance, D3DXVECTOR3* pvIntersect);

	//交差位置のポリゴンの頂点を見つける.
	HRESULT FindVerticesOnPoly(
		LPD3DXMESH pTarget, DWORD dwPolyIndex,
		D3DXVECTOR3* pVecVertices);

	//内積を使用した角度の間に存在確認関数
	bool InnerProductDot( const D3DXVECTOR3& vPos, const D3DXVECTOR3& vTargetPos, const float& fYAxis, const float& fAngle );



#if 0

	//バウンディングボックス作成.
	HRESULT InitBBox(clsDX9Mesh* pMesh);
	HRESULT InitBBox(clsD3DXSKINMESH* pMesh);

	//衝突判定(バウンディングボックス).
	bool BBoxCollision(clsDX9Mesh* pAttacker, clsDX9Mesh* pTarget);
	bool BBoxCollision(clsD3DXSKINMESH* pAttacker, clsD3DXSKINMESH* pTarget);

#endif // 0
};

#endif  //#ifdef _C_STAGE_H_