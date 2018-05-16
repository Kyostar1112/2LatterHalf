#include"StageScene.h"

clsStageScene::clsStageScene()
{
}

clsStageScene::~clsStageScene()
{
}

void clsStageScene::Create()
{
	m_vsmpShot.reserve(ConstantStageScene::SHOT_MAX);

	//弾作成.
	for (int i = 0; i < ConstantStageScene::SHOT_MAX; i++)
	{
		m_vsmpShot.push_back(make_unique<clsPlayerShot>());
		m_vsmpShotSphere.push_back(make_unique<clsSphere>());

		//モデルデータ関連付け.
		m_vsmpShot[i]->AttachModel(Resource->GetStaticModels(Resource->enStaticModel_Shot));
		m_vsmpShot[i]->SetScale(1.0f);
		m_vsmpShotSphere[i]->AttachModel(Resource->GetStaticModels(Resource->enStaticModel_Sphere));
	}

	//敵作成.
	for (int i = 0; i < ConstantStageScene::ENEMYMAX; i++)
	{
		m_vsmpEnemy.push_back(make_unique<clsEnemy>());
		m_vsmpEnemySphere.push_back(make_unique<clsSphere>());

		//モデルデータ関連付け.
		m_vsmpEnemy[i]->AttachModel(Resource->GetSkinModels(Resource->enSkinModel_Boss));
		m_vsmpEnemy[i]->SetScale(0.05f);
		m_vsmpEnemySphere[i]->AttachModel(Resource->GetStaticModels(Resource->enStaticModel_Sphere));
	}

	//「地面」
	m_smpGround = make_unique<clsCharacter>();
	m_smpGround->AttachModel(Resource->GetStaticModels(Resource->enStaticModel_Plane));
	m_smpGround->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//「自機」
	m_smpPlayer = make_unique<clsPlayer>();
	m_smpPlayer->AttachModel(Resource->GetSkinModels(Resource->enSkinModel_Player));
	m_smpPlayer->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_smpPlayer->SetScale(0.0008f);
	m_smpPlayerSphere = make_unique<clsSphere>();
	m_smpPlayerSphere->AttachModel(Resource->GetStaticModels(Resource->enStaticModel_Sphere));

	//HPゲーシ゛.
	m_smpHpGage = make_unique<clsSprite2D>();
	m_smpHpGage->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\LoadGage.png");
	m_smpHpGage->SetStrideX(20.0f);
	m_smpHpGage->SetPatarnU(1.0f);

	//「照準」
	m_smpTargetPoint = make_unique<clsSprite2D>();
	m_smpTargetPoint->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\TargetPoint.png");
	m_smpTargetPoint->MulDisp(0.5f);
	m_smpTargetPoint->UpDateSpriteSs();

	m_smpScoreNum = make_unique<clsNum>();
	m_smpScoreNum->Create(ConstantStageScene::StageEnemyDowndigit);

	m_smpHpNum = make_unique<clsNum>();
	m_smpHpNum->Create(ConstantStageScene::PlayerHpNumdigit);

	m_smpClearTimeNum = make_unique<clsNum>();
	m_smpClearTimeNum->Create(ConstantStageScene::StageClearTimedigit);

	m_smpScoreNum->SetMulDisp(0.6f);
	m_smpScoreNum->UpDateSpriteSs();
	m_smpHpNum->SetMulDisp(0.6f);
	m_smpHpNum->UpDateSpriteSs();
	m_smpClearTimeNum->SetMulDisp(0.6f);
	m_smpClearTimeNum->UpDateSpriteSs();

	//ハートマーク.
	m_smpHeart = make_unique<clsSprite2D>();
	m_smpHeart->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\Heart.png");
	m_smpHeart->SetDispH(m_smpHpNum->GetDispH());
	m_smpHeart->SetDispW(m_smpHpNum->GetDispH());
	m_smpHeart->UpDateSpriteSs();

	//時計マーク.
	m_smpClock = make_unique<clsSprite2D>();
	m_smpClock->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\Time.png");
	m_smpClock->SetDispH(m_smpClearTimeNum->GetDispH());
	m_smpClock->SetDispW(m_smpClearTimeNum->GetDispH());
	m_smpClock->UpDateSpriteSs();

	//スコア文字.
	m_smpScoreTxt = make_unique<clsSprite2D>();
	m_smpScoreTxt->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\ScoreTxt.png");
	m_smpScoreTxt->SetDispH(m_smpHpNum->GetDispH());
	m_smpScoreTxt->SetDispW(m_smpHpNum->GetDispH() * 2);
	m_smpScoreTxt->UpDateSpriteSs();

	//ダメーシ゛を受けたときの点滅用画像.
	m_smpDamageImg = make_unique<clsSprite2D>();
	m_smpDamageImg->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\Damage.png");

	//音楽作成.
	//BGM作成.
#if 1
	m_vsmpBgm.push_back(make_unique<clsSound>());
	m_vsmpBgm[0]->Open("Data\\Sound\\BGM\\StageScene.mp3", "StageBGM00", Resource->GetSpriteRenderSet().hWnd);
	m_vsmpBgm[0]->SetVolume(10);

#endif // 1

	//弾の発射音作成.
#if 1
	m_vsmpShotSe.push_back(make_unique<clsSound>());
	strcpy(m_vsmpShotSe[0]->m_stSoundData.sPath, "Data\\Sound\\SE\\Shot01.mp3");
	strcpy(m_vsmpShotSe[0]->m_stSoundData.sAilias, "SeShot");
	m_vsmpShotSe.push_back(make_unique<clsSound>());
	strcpy(m_vsmpShotSe[1]->m_stSoundData.sPath, "Data\\Sound\\SE\\Shot01.mp3");
	strcpy(m_vsmpShotSe[1]->m_stSoundData.sAilias, "SeShot01");
	for (UINT i = 0; i < m_vsmpShotSe.size(); i++)
	{
		m_vsmpShotSe[i]->Open(m_vsmpShotSe[i]->m_stSoundData.sPath, m_vsmpShotSe[i]->m_stSoundData.sAilias, Resource->GetSpriteRenderSet().hWnd);
		m_vsmpShotSe[i]->SetVolume(15);
	}

#endif // 1

	//敵に弾が当たった時の音.
#if 1
	m_vsmpHitSe.push_back(make_unique<clsSound>());
	strcpy(m_vsmpHitSe[0]->m_stSoundData.sPath, "Data\\Sound\\SE\\EnemyHit.mp3");
	strcpy(m_vsmpHitSe[0]->m_stSoundData.sAilias, "SeEnemyHit00");
	m_vsmpHitSe.push_back(make_unique<clsSound>());
	strcpy(m_vsmpHitSe[1]->m_stSoundData.sPath, "Data\\Sound\\SE\\EnemyHit.mp3");
	strcpy(m_vsmpHitSe[1]->m_stSoundData.sAilias, "SeEnemyHit01");
	m_vsmpHitSe.push_back(make_unique<clsSound>());
	strcpy(m_vsmpHitSe[2]->m_stSoundData.sPath, "Data\\Sound\\SE\\EnemyHit.mp3");
	strcpy(m_vsmpHitSe[2]->m_stSoundData.sAilias, "SeEnemyHit02");
	for (UINT i = 0; i < m_vsmpHitSe.size(); i++)
	{
		m_vsmpHitSe[i]->Open(m_vsmpHitSe[i]->m_stSoundData.sPath, m_vsmpHitSe[i]->m_stSoundData.sAilias, Resource->GetSpriteRenderSet().hWnd);
		m_vsmpHitSe[i]->SetVolume(15);
	}
#endif // 1
}

void clsStageScene::Init()
{
	m_stMode = Stage;
	m_iScore = 0;

	//アニメーション経過時間
	m_smpPlayer->m_dAnimTime = 0;

	//アニメーション速度.
	m_smpPlayer->m_dAnimSpeed = 0.03f;

	m_smpPlayer->m_dAnimNum = 1;
	m_smpPlayer->ChangeAnimSet(m_smpPlayer->m_dAnimNum);

	m_iClearTime = ConstantStageScene::StageClearTimeMax;
	m_icnt = 0;
	m_vsmpBgm[0]->SeekToStart();
	for (UINT i = 0; i < m_vsmpEnemy.size(); i++)
	{
		m_vsmpEnemy[i]->InitEnemy(false, m_smpPlayer->GetPosition());
	}

	m_bPlayerDamage = false;
	m_iPlayerinvincible = 0;

	m_iShotIntervalCnt = 0;
	m_iEnemyIntervalCnt = 0;

	m_iHp = ConstantStageScene::PLAYERHP;

	m_smpHeart->SetPos(0.0f, 0.0f);
	m_smpHpNum->Init();
	m_smpHpNum->AddPosX(m_smpHeart->GetSs().Disp.w + m_smpHeart->GetPos().x);

	m_smpScoreTxt->SetPos(80.0f + m_smpHpNum->GetDispH() + m_smpHpNum->GetPos().x, m_smpHpNum->GetPos().y);
	m_smpScoreNum->Init();
	m_smpScoreNum->AddPosX(m_smpScoreTxt->GetSs().Disp.w + m_smpScoreTxt->GetPos().x);

	m_smpClock->SetPos(m_smpScoreNum->GetDispW() + m_smpScoreNum->GetPos().x - 120.0f, m_smpScoreNum->GetPos().y);
	m_smpClearTimeNum->Init();
	m_smpClearTimeNum->AddPosX(m_smpClock->GetSs().Disp.w + m_smpClock->GetPos().x);

	m_smpHpGage->SetPos(0.0f, 0.0f);
	m_smpTargetPoint->SetPos(WND_W / 2 - m_smpTargetPoint->GetSs().Disp.w / 2, WND_H / 2 - m_smpTargetPoint->GetSs().Disp.h / 2 - 60.0f);
}

void clsStageScene::UpDate()
{
	m_icnt++;
	//アニメーション速度加算は先に.
	m_smpPlayer->m_dAnimTime += m_smpPlayer->m_dAnimSpeed;
	if (m_smpPlayer->m_dAnimNum == 7)
	{
		if (m_smpPlayer->m_dAnimTime > m_smpPlayer->GetAnimPeriod(07))
		{
			m_smpPlayer->m_dAnimSpeed = 0.03f;
			m_smpPlayer->m_dAnimNum = 01;
			m_smpPlayer->ChangeAnimSet(m_smpPlayer->m_dAnimNum);
		}
	}
	//音楽関連.
	{
		for (UINT i = 0; i < m_vsmpBgm.size(); i++)
		{
			if (m_vsmpBgm[i]->IsStopped())
			{
				m_vsmpBgm[i]->SeekToStart();
			}
			if (!m_vsmpBgm[i]->IsPlaying())
			{
				m_vsmpBgm[i]->Play();
			}
		}

		for (UINT i = 0; i < m_vsmpShotSe.size(); i++)
		{
			if (m_vsmpShotSe[i]->IsStopped())
			{
				m_vsmpShotSe[i]->SeekToStart();
			}
		}
		for (UINT i = 0; i < m_vsmpHitSe.size(); i++)
		{
			if (m_vsmpHitSe[i]->IsStopped())
			{
				m_vsmpHitSe[i]->SeekToStart();
			}
		}
	}
	m_iShotIntervalCnt--;

#if 1
	for (UINT i = 0; i < m_vsmpShot.size(); i++)
	{
		if (!m_vsmpShot[i]->GetShotFlg())
		{
			m_vsmpShot[i]->SetRotationY(m_smpPlayer->GetRotationY());
			m_vsmpShot[i]->SetPosition(m_smpPlayer->GetPosition());
			m_vsmpShot[i]->SetPositionY(1.5f);
			break;
		}
		m_vsmpShot[i]->FireInterval();
	}

#endif // 0

	m_iEnemyIntervalCnt--;
	if (m_iEnemyIntervalCnt < 0)
	{
		for (UINT i = 0; i < m_vsmpEnemy.size(); i++)
		{
			if (!m_vsmpEnemy[i]->GetEnableFlg())
			{
				m_vsmpEnemy[i]->Spawn();
				m_iEnemyIntervalCnt = ConstantStageScene::SPAWNCNT;
				break;
			}
		}
	}

	if (m_stMode == Stage)
	{
		if (m_icnt % 60 == 0)
		{
			m_iClearTime--;
			m_icnt = 0;
		}
	}

	//ゲームクリア判断用.
	if (m_iClearTime < 1 && m_stMode == Stage)
	{
		m_stMode = Clear;
		Resource->m_viScore.push_back(m_iScore);
		sort(Resource->m_viScore.begin(), Resource->m_viScore.end());
		Resource->m_viScore.erase(unique(Resource->m_viScore.begin(), Resource->m_viScore.end()), Resource->m_viScore.end());
		reverse(Resource->m_viScore.begin(), Resource->m_viScore.end());
	}

	//敵の動き.
	for (UINT i = 0; i < m_vsmpEnemy.size(); i++)
	{
		if (m_vsmpEnemy[i]->GetEnableFlg())
		{
			m_vsmpEnemy[i]->Loop();
		}
	}
	m_iPlayerinvincible--;
	if (m_bPlayerDamage)
	{
		m_iPlayerinvincible = ConstantStageScene::PLAYERINVINCIBLETIME;
		m_bPlayerDamage = false;
	}

	//弾丸の初期化用.
#if 0
	if (GetAsyncKeyState('I') & 0x0001/*||m_pDxInput->IsPressKey( enPKey_00 )*/) {
		for (UINT i = 0; i < m_vsmpShot.size(); i++)
		{
			m_vsmpShot[i]->SetPosition(m_smpPlayer->GetPosition());
		}
	}
#endif // 0

	//当たり判定.
	HitCheak();
}

void clsStageScene::RightRoll(float MovePoint)
{
	m_smpPlayer->AddRotationY(MovePoint);
}
void clsStageScene::LeftRoll(float MovePoint)
{
	m_smpPlayer->AddRotationY(-MovePoint);
}
void clsStageScene::Fire()
{
	if (m_iShotIntervalCnt < 0)
	{
		for (UINT i = 0; i < m_vsmpShot.size(); i++)
		{
			if (!m_vsmpShot[i]->GetShotFlg())
			{
				for (UINT j = 0; j < m_vsmpShotSe.size(); j++)
				{
					if (m_vsmpShotSe[j]->m_bSeekFlg)
					{
						m_vsmpShotSe[j]->Play();
						break;
					}
				}
				m_smpPlayer->m_dAnimNum = 7;
				m_smpPlayer->m_dAnimTime = 0;
				m_smpPlayer->m_dAnimSpeed = 0.05f;
				m_smpPlayer->ChangeAnimSet(m_smpPlayer->m_dAnimNum);
				m_vsmpShot[i]->SetRotationY(m_smpPlayer->GetRotationY());
				m_vsmpShot[i]->SetPosition(m_smpPlayer->GetPosition());
				m_vsmpShot[i]->SetPositionY(1.5f);
				m_vsmpShot[i]->Fire();
				m_iShotIntervalCnt = ConstantStageScene::SHOT_INTERVAL_CNT;
				break;
			}
		}
	}
}

void clsStageScene::ModelRender1()
{
	m_stModelRenderSet = Resource->GetModelRenderSet();

	//「地面」の表示.
	m_smpGround->Render(m_stModelRenderSet.mView, m_stModelRenderSet.mProj, m_stModelRenderSet.vLight, m_stModelRenderSet.vEye);

	//「自機」の表示.
	m_smpPlayer->Render(m_stModelRenderSet.mView, m_stModelRenderSet.mProj, m_stModelRenderSet.vLight, m_stModelRenderSet.vEye);

	//当たり判定確認用.
#if 0
	{
		for (size_t i = 0; i < m_vsmpShotSphere.size(); i++)
		{
			m_vsmpShotSphere[i]->Render(m_stModelRenderSet.mView, m_stModelRenderSet.mProj, m_stModelRenderSet.vLight, m_stModelRenderSet.vEye);
		}
		for (size_t i = 0; i < m_vsmpEnemySphere.size(); i++)
		{
			m_vsmpEnemySphere[i]->Render(m_stModelRenderSet.mView, m_stModelRenderSet.mProj, m_stModelRenderSet.vLight, m_stModelRenderSet.vEye);
		}
		m_smpPlayerSphere->Render(m_stModelRenderSet.mView, m_stModelRenderSet.mProj, m_stModelRenderSet.vLight, m_stModelRenderSet.vEye);
	}

#endif // 0
}
void clsStageScene::ExpRender() {
	for (UINT i = 0; i < m_vsmpEnemy.size(); i++)
	{
		m_vsmpEnemy[i]->ExpRender();
	}
}
void clsStageScene::ModelRender2() {
	//ショット.
	for (UINT i = 0; i < m_vsmpShot.size(); i++)
	{
		if (m_vsmpShot[i]->GetShotFlg())
		{
			m_vsmpShot[i]->Update();
			m_vsmpShot[i]->m_enDir = enDirection_Foward;
			m_vsmpShot[i]->UpdatePos();
			m_vsmpShot[i]->Render(m_stModelRenderSet.mView, m_stModelRenderSet.mProj, m_stModelRenderSet.vLight, m_stModelRenderSet.vEye);
		}
	}

	//スキンメッシュの表示.
	for (UINT i = 0; i < m_vsmpEnemy.size(); i++)
	{
		if (m_vsmpEnemy[i]->GetEnableFlg())
		{
			m_vsmpEnemy[i]->Render(m_stModelRenderSet.mView, m_stModelRenderSet.mProj, m_stModelRenderSet.vLight, m_stModelRenderSet.vEye);
		}
	}
}

void clsStageScene::SpriteRender()
{
	m_smpTargetPoint->Render();
	//m_smpHpGage->Render();
	if (m_iPlayerinvincible > 0)
	{
		m_smpDamageImg->Flashing(0.05f, 0.5f);
	}
	else
	{
		m_smpDamageImg->SetAlpha(0.0f);
	}
	m_smpDamageImg->SetPos(0.0f, 0.0f);
	m_smpDamageImg->Render();

	m_smpHpNum->Render(m_iHp);
	m_smpScoreNum->Render(m_iScore);
	m_smpClearTimeNum->Render(m_iClearTime);

	m_smpHeart->Render();
	m_smpScoreTxt->Render();
	m_smpClock->Render();

	BlackOut();
}

void clsStageScene::Release()
{
	//キャラクタクラスの破棄.
	for (UINT i = 0; i < m_vsmpEnemy.size(); i++)
	{
		//モデルデータの関連付け解除.
		m_vsmpEnemy[i]->DetachModel();
		//破棄.
		m_vsmpEnemy[i].reset();
		m_vsmpEnemy[i] = NULL;
	}
	//ポインタ配列を破棄.
	m_vsmpEnemy.clear();
	m_vsmpEnemy.shrink_to_fit();

	for (UINT i = 0; i < m_vsmpShot.size(); i++)
	{
		//モデルデータの関連付け解除.
		m_vsmpShot[i]->DetachModel();
		//破棄.
		m_vsmpShot[i].reset();
		m_vsmpShot[i] = NULL;
	}
	//ポインタ配列を破棄.
	m_vsmpShot.clear();
	m_vsmpShot.shrink_to_fit();

	//破棄.
	m_smpGround->DetachModel();
	m_smpGround.reset();
	m_smpGround = NULL;

	//破棄.
	m_smpPlayer->DetachModel();
	m_smpPlayer.reset();
	m_smpPlayer = NULL;

	//数字画像破棄.

	SAFE_RELEASE(m_smpHpNum);
	SAFE_RELEASE(m_smpClearTimeNum);

	//音楽破棄.
	m_vsmpBgm[0]->Close();
	for (UINT i = 0; i < m_vsmpShotSe.size(); i++)
	{
		m_vsmpShotSe[i]->Close();
	}
	for (UINT i = 0; i < m_vsmpHitSe.size(); i++)
	{
		m_vsmpHitSe[i]->Close();
	}
}

void clsStageScene::HitCheak()
{
	//当たり判定は最後に.
	for (size_t i = 0; i < m_vsmpEnemy.size(); i++) {
		Ray(m_vsmpEnemy[i].get());
	}
	//弾と敵との当たり判定.
	for (UINT i = 0; i < m_vsmpShotSphere.size(); i++)
	{
		if (!m_vsmpShot[i]->GetShotFlg())
		{
			continue;
		}
		m_vsmpShotSphere[i]->SetPosition(m_vsmpShot[i]->m_Sphere.vCenter);
		m_vsmpShotSphere[i]->SetScale(m_vsmpShot[i]->m_Sphere.fRadius);
		m_vsmpShotSphere[i]->Update();
		for (UINT j = 0; j < m_vsmpEnemy.size(); j++)
		{
			if (!m_vsmpEnemy[j]->GetEnableFlg())
			{
				continue;
			}
			m_vsmpEnemySphere[j]->SetPosition(m_vsmpEnemy[j]->GetPosition());
			m_vsmpEnemySphere[j]->SetScale(0.5f);
			m_vsmpEnemySphere[j]->Update();
			if (Collision(m_vsmpShotSphere[i].get(), m_vsmpEnemySphere[j].get())) {
				for (UINT k = 0; k < m_vsmpHitSe.size(); k++)
				{
					if (m_vsmpHitSe[k]->m_bSeekFlg)
					{
						m_vsmpHitSe[k]->Play();
						break;
					}
				}
				m_iScore += 100;
				m_vsmpEnemy[j]->Damage();
				m_vsmpEnemy[j]->SetPositionY(99.0f);
				m_vsmpEnemy[j]->SetEnableFlg(false);
			}
		}
	}

	//自機と敵の当たり判定.
	if (!m_bPlayerDamage && m_iPlayerinvincible < 0)
	{
		for (UINT i = 0; i < m_vsmpEnemy.size(); i++)
		{
			if (!m_vsmpEnemy[i]->GetEnableFlg())
			{
				continue;
			}
			if (m_vsmpEnemy[i]->m_bAttakFlg) {
				if (m_iHp > 1)
				{
					m_iHp--;
				}
				else
				{
					if (m_stMode == Stage)
					{
						m_stMode = Over;
						Resource->m_viScore.push_back(m_iScore);
						sort(Resource->m_viScore.begin(), Resource->m_viScore.end());
						Resource->m_viScore.erase(unique(Resource->m_viScore.begin(), Resource->m_viScore.end()), Resource->m_viScore.end());
						reverse(Resource->m_viScore.begin(), Resource->m_viScore.end());
					}
				}
				m_bPlayerDamage = true;
				m_vsmpEnemy[i]->m_bAttakFlg = false;
			}
		}
	}
}

//衝突判定.
bool clsStageScene::Collision(
	clsDX9Mesh* pAttacker,	//攻撃側.
	clsDX9Mesh* pTarget)	//標的.
{
	//２つの物体の中心間の距離を求める.
	D3DXVECTOR3 vLength
		= pTarget->m_vPos - pAttacker->m_vPos;
	//長さに変換する.
	float Length = D3DXVec3Length(&vLength);

	//２物体間の距離が、２物体の半径を足したもより.
	//小さいということは、スフィア同士が重なっている.
	//（衝突している）ということ.
	if (Length <=
		pAttacker->m_Sphere.fRadius + pTarget->m_Sphere.fRadius)
	{
		return true;//衝突.
	}
	return false;//衝突していない.
}

bool clsStageScene::Collision(
	clsCharacter* pAttacker,	//攻撃側.
	clsCharacter* pTarget)	//標的.
{
	pAttacker->SetPositionY(0.0f);
	pTarget->SetPositionY(0.0f);
	//２つの物体の中心間の距離を求める.
	D3DXVECTOR3 vLength
		= pTarget->GetPosition() - pAttacker->GetPosition();
	//長さに変換する.
	float Length = D3DXVec3Length(&vLength);

	//２物体間の距離が、２物体の半径を足したもより.
	//小さいということは、スフィア同士が重なっている.
	//（衝突している）ということ.
	if (Length <=
		pAttacker->m_Sphere.fRadius + pTarget->m_Sphere.fRadius)
	{
		return true;//衝突.
	}
	return false;//衝突していない.
}

void clsStageScene::Ray(clsEnemy* Enemy)
{
	FLOAT		fDistance;	//距離.
	D3DXVECTOR3 vIntersect;	//交差座標.
	//現在位置をコピー.
	Enemy->m_vRay = Enemy->GetPosition();
	//レイの高さを自機の位置より上にする.
	Enemy->m_vRay.y
		= Enemy->GetPositionY() + 1.0f;
	//軸ベクトルは垂直で下向き.
	Enemy->m_vAxis
		= D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	Intersect(
		Enemy, m_smpGround.get(), &fDistance, &vIntersect);

	//交点の座標からy座標を自機のy座標としてセット.
	Enemy->SetPositionY(vIntersect.y + 0.03f);
}
//レイとメッシュの当たり判定.
bool clsStageScene::Intersect(
	clsGameObject* pAttacker,	//基準の物体.
	clsCharacter*  pTarget,		//対象の物体.
	float* pfDistance,			//(out)距離.
	D3DXVECTOR3* pvIntersect)	//(out)交差座標.
{
	D3DXMATRIXA16 matRot;	//回転行列.

	//回転行列を計算.
	D3DXMatrixRotationY(&matRot, pAttacker->GetRotationY());

	//軸ベクトルを用意.
	D3DXVECTOR3 vecAxisZ;
	//Z軸ベクトルそのものを現在の回転状態により変換する.
	D3DXVec3TransformCoord(
		&vecAxisZ, &pAttacker->m_vAxis, &matRot);

	D3DXVECTOR3 vecStart, vecEnd, vecDistance;
	//レイの開始終了位置をAttackerと合わせる.
	vecStart = vecEnd = pAttacker->m_vRay;
	//Attackerの座標に回転座標を合成する.
	vecEnd += vecAxisZ * 1.0f;

	//対象が動いている物体でも、対象のworld行列の、
	//逆行列を用いれば、正しくレイが当たる.
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(
		&matWorld,
		pTarget->GetPositionX(),
		pTarget->GetPositionY(),
		pTarget->GetPositionZ());

	//逆行列を求める.
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformCoord(
		&vecStart, &vecStart, &matWorld);
	D3DXVec3TransformCoord(
		&vecEnd, &vecEnd, &matWorld);

	//距離を求める.
	vecDistance = vecEnd - vecStart;

	BOOL bHit = false;	//命中フラグ.

	DWORD dwIndex = 0;		//インデックス番号.
	D3DXVECTOR3 vVertex[3];	//頂点座標.
	FLOAT U = 0, V = 0;			//(out)重心ヒット座標.

	//メッシュとレイの交差を調べる.
	D3DXIntersect(
		pTarget->GetMesh(),	//対象メッシュ.
		&vecStart,			//開始位置.
		&vecDistance,		//レイの距離.
		&bHit,				//(out)判定結果.
		&dwIndex,	//(out)bHitがTrue時、レイの始点に.
		//最も近くの面のインデックス値へのポインタ.
		&U, &V,				//(out)重心ヒット座標.
		pfDistance,			//ターゲットとの距離.
		NULL, NULL);
	if (bHit) {
		//命中したとき.
		FindVerticesOnPoly(
			pTarget->GetMeshForRay(), dwIndex, vVertex);
		//重心座標から交差点を算出.
		//ローカル交点pは、v0 + U*(v1-v0) + V*(v2-v0)で求まる.
		*pvIntersect =
			vVertex[0]
			+ U * (vVertex[1] - vVertex[0])
			+ V * (vVertex[2] - vVertex[0]);

		return true;//命中している.
	}
	return false;//外れている.
}

//交差位置のポリゴンの頂点を見つける.
//※頂点座標はローカル座標.
HRESULT clsStageScene::FindVerticesOnPoly(
	LPD3DXMESH pTarget, DWORD dwPolyIndex,
	D3DXVECTOR3* pVecVertices)
{
	//頂点ごとのバイト数を取得.
	DWORD dwStride = pTarget->GetNumBytesPerVertex();
	//頂点数を取得.
	DWORD dwVertexAmt = pTarget->GetNumVertices();
	//面数を取得.
	DWORD dwPolyAmt = pTarget->GetNumFaces();

	WORD* pwPoly = NULL;

	//インデックスバッファをロック(読込モード)
	pTarget->LockIndexBuffer(
		D3DLOCK_READONLY, (VOID**)&pwPoly);
	BYTE* pbVertices = NULL;//頂点(バイト型)
	FLOAT* pfVertices = NULL;//頂点(float型)
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;//頂点バッファ.
	pTarget->GetVertexBuffer(&VB);//頂点情報の取得.

	//頂点バッファのロック.
	if (SUCCEEDED(
		VB->Lock(0, 0, (VOID**)&pbVertices, 0)))
	{
		//ポリゴンの頂点の１つ目を取得.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3]];
		pVecVertices[0].x = pfVertices[0];
		pVecVertices[0].y = pfVertices[1];
		pVecVertices[0].z = pfVertices[2];

		//ポリゴンの頂点の２つ目を取得.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 1]];
		pVecVertices[1].x = pfVertices[0];
		pVecVertices[1].y = pfVertices[1];
		pVecVertices[1].z = pfVertices[2];

		//ポリゴンの頂点の３つ目を取得.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 2]];
		pVecVertices[2].x = pfVertices[0];
		pVecVertices[2].y = pfVertices[1];
		pVecVertices[2].z = pfVertices[2];

		pTarget->UnlockIndexBuffer();	//ロック解除.
		VB->Unlock();	//ロック解除.
	}
	VB->Release();

	return S_OK;
}

//スフィア作成.
HRESULT clsStageScene::InitSphere(clsDX9Mesh* pMesh, float fScale)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;	//頂点バッファ.
	void*		pVertices = NULL;	//頂点
	D3DXVECTOR3 vCenter;		//中心.
	float		fRadius;			//半径.

	//頂点バッファを取得.
	if (FAILED(pMesh->m_pMesh->GetVertexBuffer(&pVB)))
	{
		return E_FAIL;
	}

	//メッシュの頂点バッファをロックする.
	if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
	{
		SAFE_RELEASE(pVB);
		return E_FAIL;
	}

	//メッシュの外接円の中心と半径を計算する.
	D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)pVertices,
		pMesh->m_pMesh->GetNumVertices(),//頂点の数.
		D3DXGetFVFVertexSize(pMesh->m_pMesh->GetFVF()),//頂点の情報.
		&vCenter,	//(out)中心座標.
		&fRadius);	//(out)半径.

	//アンロック.
	pVB->Unlock();
	SAFE_RELEASE(pVB);

	//中心と半径を構造体に設定.
	pMesh->m_Sphere.vCenter = vCenter;
	pMesh->m_Sphere.fRadius = fRadius * fScale;

	return S_OK;
}