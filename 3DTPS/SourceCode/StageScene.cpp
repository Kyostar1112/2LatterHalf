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

	//�e�쐬.
	for (int i = 0; i < ConstantStageScene::SHOT_MAX; i++)
	{
		m_vsmpShot.push_back(make_unique<clsPlayerShot>());
		m_vsmpShotSphere.push_back(make_unique<clsSphere>());

		//���f���f�[�^�֘A�t��.
		m_vsmpShot[i]->AttachModel(Resource->GetStaticModels(Resource->enStaticModel_Shot));
		m_vsmpShot[i]->SetScale(1.0f);
		m_vsmpShotSphere[i]->AttachModel(Resource->GetStaticModels(Resource->enStaticModel_Sphere));
	}

	//�G�쐬.
	for (int i = 0; i < ConstantStageScene::ENEMYMAX; i++)
	{
		m_vsmpEnemy.push_back(make_unique<clsEnemy>());
		m_vsmpEnemySphere.push_back(make_unique<clsSphere>());

		//���f���f�[�^�֘A�t��.
		m_vsmpEnemy[i]->AttachModel(Resource->GetSkinModels(Resource->enSkinModel_Boss));
		m_vsmpEnemy[i]->SetScale(0.05f);
		m_vsmpEnemySphere[i]->AttachModel(Resource->GetStaticModels(Resource->enStaticModel_Sphere));
	}

	//�u�n�ʁv
	m_smpGround = make_unique<clsCharacter>();
	m_smpGround->AttachModel(Resource->GetStaticModels(Resource->enStaticModel_Plane));
	m_smpGround->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�u���@�v
	m_smpPlayer = make_unique<clsPlayer>();
	m_smpPlayer->AttachModel(Resource->GetSkinModels(Resource->enSkinModel_Player));
	m_smpPlayer->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_smpPlayer->SetScale(0.0008f);
	m_smpPlayerSphere = make_unique<clsSphere>();
	m_smpPlayerSphere->AttachModel(Resource->GetStaticModels(Resource->enStaticModel_Sphere));

	//HP�Q�[�V�J.
	m_smpHpGage = make_unique<clsSprite2D>();
	m_smpHpGage->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\LoadGage.png");
	m_smpHpGage->SetStrideX(20.0f);
	m_smpHpGage->SetPatarnU(1.0f);

	//�u�Ə��v
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

	//�n�[�g�}�[�N.
	m_smpHeart = make_unique<clsSprite2D>();
	m_smpHeart->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\Heart.png");
	m_smpHeart->SetDispH(m_smpHpNum->GetDispH());
	m_smpHeart->SetDispW(m_smpHpNum->GetDispH());
	m_smpHeart->UpDateSpriteSs();

	//���v�}�[�N.
	m_smpClock = make_unique<clsSprite2D>();
	m_smpClock->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\Time.png");
	m_smpClock->SetDispH(m_smpClearTimeNum->GetDispH());
	m_smpClock->SetDispW(m_smpClearTimeNum->GetDispH());
	m_smpClock->UpDateSpriteSs();

	//�X�R�A����.
	m_smpScoreTxt = make_unique<clsSprite2D>();
	m_smpScoreTxt->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\ScoreTxt.png");
	m_smpScoreTxt->SetDispH(m_smpHpNum->GetDispH());
	m_smpScoreTxt->SetDispW(m_smpHpNum->GetDispH() * 2);
	m_smpScoreTxt->UpDateSpriteSs();

	//�_���[�V�J���󂯂��Ƃ��̓_�ŗp�摜.
	m_smpDamageImg = make_unique<clsSprite2D>();
	m_smpDamageImg->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\Damage.png");

	//���y�쐬.
	//BGM�쐬.
#if 1
	m_vsmpBgm.push_back(make_unique<clsSound>());
	m_vsmpBgm[0]->Open("Data\\Sound\\BGM\\StageScene.mp3", "StageBGM00", Resource->GetSpriteRenderSet().hWnd);
	m_vsmpBgm[0]->SetVolume(10);

#endif // 1

	//�e�̔��ˉ��쐬.
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

	//�G�ɒe�������������̉�.
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

	//�A�j���[�V�����o�ߎ���
	m_smpPlayer->m_dAnimTime = 0;

	//�A�j���[�V�������x.
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
	//�A�j���[�V�������x���Z�͐��.
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
	//���y�֘A.
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

	//�Q�[���N���A���f�p.
	if (m_iClearTime < 1 && m_stMode == Stage)
	{
		m_stMode = Clear;
		Resource->m_viScore.push_back(m_iScore);
		sort(Resource->m_viScore.begin(), Resource->m_viScore.end());
		Resource->m_viScore.erase(unique(Resource->m_viScore.begin(), Resource->m_viScore.end()), Resource->m_viScore.end());
		reverse(Resource->m_viScore.begin(), Resource->m_viScore.end());
	}

	//�G�̓���.
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

	//�e�ۂ̏������p.
#if 0
	if (GetAsyncKeyState('I') & 0x0001/*||m_pDxInput->IsPressKey( enPKey_00 )*/) {
		for (UINT i = 0; i < m_vsmpShot.size(); i++)
		{
			m_vsmpShot[i]->SetPosition(m_smpPlayer->GetPosition());
		}
	}
#endif // 0

	//�����蔻��.
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

	//�u�n�ʁv�̕\��.
	m_smpGround->Render(m_stModelRenderSet.mView, m_stModelRenderSet.mProj, m_stModelRenderSet.vLight, m_stModelRenderSet.vEye);

	//�u���@�v�̕\��.
	m_smpPlayer->Render(m_stModelRenderSet.mView, m_stModelRenderSet.mProj, m_stModelRenderSet.vLight, m_stModelRenderSet.vEye);

	//�����蔻��m�F�p.
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
	//�V���b�g.
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

	//�X�L�����b�V���̕\��.
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
	//�L�����N�^�N���X�̔j��.
	for (UINT i = 0; i < m_vsmpEnemy.size(); i++)
	{
		//���f���f�[�^�̊֘A�t������.
		m_vsmpEnemy[i]->DetachModel();
		//�j��.
		m_vsmpEnemy[i].reset();
		m_vsmpEnemy[i] = NULL;
	}
	//�|�C���^�z���j��.
	m_vsmpEnemy.clear();
	m_vsmpEnemy.shrink_to_fit();

	for (UINT i = 0; i < m_vsmpShot.size(); i++)
	{
		//���f���f�[�^�̊֘A�t������.
		m_vsmpShot[i]->DetachModel();
		//�j��.
		m_vsmpShot[i].reset();
		m_vsmpShot[i] = NULL;
	}
	//�|�C���^�z���j��.
	m_vsmpShot.clear();
	m_vsmpShot.shrink_to_fit();

	//�j��.
	m_smpGround->DetachModel();
	m_smpGround.reset();
	m_smpGround = NULL;

	//�j��.
	m_smpPlayer->DetachModel();
	m_smpPlayer.reset();
	m_smpPlayer = NULL;

	//�����摜�j��.

	SAFE_RELEASE(m_smpHpNum);
	SAFE_RELEASE(m_smpClearTimeNum);

	//���y�j��.
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
	//�����蔻��͍Ō��.
	for (size_t i = 0; i < m_vsmpEnemy.size(); i++) {
		Ray(m_vsmpEnemy[i].get());
	}
	//�e�ƓG�Ƃ̓����蔻��.
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

	//���@�ƓG�̓����蔻��.
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

//�Փ˔���.
bool clsStageScene::Collision(
	clsDX9Mesh* pAttacker,	//�U����.
	clsDX9Mesh* pTarget)	//�W�I.
{
	//�Q�̕��̂̒��S�Ԃ̋��������߂�.
	D3DXVECTOR3 vLength
		= pTarget->m_vPos - pAttacker->m_vPos;
	//�����ɕϊ�����.
	float Length = D3DXVec3Length(&vLength);

	//�Q���̊Ԃ̋������A�Q���̂̔��a�𑫂��������.
	//�������Ƃ������Ƃ́A�X�t�B�A���m���d�Ȃ��Ă���.
	//�i�Փ˂��Ă���j�Ƃ�������.
	if (Length <=
		pAttacker->m_Sphere.fRadius + pTarget->m_Sphere.fRadius)
	{
		return true;//�Փ�.
	}
	return false;//�Փ˂��Ă��Ȃ�.
}

bool clsStageScene::Collision(
	clsCharacter* pAttacker,	//�U����.
	clsCharacter* pTarget)	//�W�I.
{
	pAttacker->SetPositionY(0.0f);
	pTarget->SetPositionY(0.0f);
	//�Q�̕��̂̒��S�Ԃ̋��������߂�.
	D3DXVECTOR3 vLength
		= pTarget->GetPosition() - pAttacker->GetPosition();
	//�����ɕϊ�����.
	float Length = D3DXVec3Length(&vLength);

	//�Q���̊Ԃ̋������A�Q���̂̔��a�𑫂��������.
	//�������Ƃ������Ƃ́A�X�t�B�A���m���d�Ȃ��Ă���.
	//�i�Փ˂��Ă���j�Ƃ�������.
	if (Length <=
		pAttacker->m_Sphere.fRadius + pTarget->m_Sphere.fRadius)
	{
		return true;//�Փ�.
	}
	return false;//�Փ˂��Ă��Ȃ�.
}

void clsStageScene::Ray(clsEnemy* Enemy)
{
	FLOAT		fDistance;	//����.
	D3DXVECTOR3 vIntersect;	//�������W.
	//���݈ʒu���R�s�[.
	Enemy->m_vRay = Enemy->GetPosition();
	//���C�̍��������@�̈ʒu����ɂ���.
	Enemy->m_vRay.y
		= Enemy->GetPositionY() + 1.0f;
	//���x�N�g���͐����ŉ�����.
	Enemy->m_vAxis
		= D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	Intersect(
		Enemy, m_smpGround.get(), &fDistance, &vIntersect);

	//��_�̍��W����y���W�����@��y���W�Ƃ��ăZ�b�g.
	Enemy->SetPositionY(vIntersect.y + 0.03f);
}
//���C�ƃ��b�V���̓����蔻��.
bool clsStageScene::Intersect(
	clsGameObject* pAttacker,	//��̕���.
	clsCharacter*  pTarget,		//�Ώۂ̕���.
	float* pfDistance,			//(out)����.
	D3DXVECTOR3* pvIntersect)	//(out)�������W.
{
	D3DXMATRIXA16 matRot;	//��]�s��.

	//��]�s����v�Z.
	D3DXMatrixRotationY(&matRot, pAttacker->GetRotationY());

	//���x�N�g����p��.
	D3DXVECTOR3 vecAxisZ;
	//Z���x�N�g�����̂��̂����݂̉�]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord(
		&vecAxisZ, &pAttacker->m_vAxis, &matRot);

	D3DXVECTOR3 vecStart, vecEnd, vecDistance;
	//���C�̊J�n�I���ʒu��Attacker�ƍ��킹��.
	vecStart = vecEnd = pAttacker->m_vRay;
	//Attacker�̍��W�ɉ�]���W����������.
	vecEnd += vecAxisZ * 1.0f;

	//�Ώۂ������Ă��镨�̂ł��A�Ώۂ�world�s��́A
	//�t�s���p����΁A���������C��������.
	D3DXMATRIX matWorld;
	D3DXMatrixTranslation(
		&matWorld,
		pTarget->GetPositionX(),
		pTarget->GetPositionY(),
		pTarget->GetPositionZ());

	//�t�s������߂�.
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	D3DXVec3TransformCoord(
		&vecStart, &vecStart, &matWorld);
	D3DXVec3TransformCoord(
		&vecEnd, &vecEnd, &matWorld);

	//���������߂�.
	vecDistance = vecEnd - vecStart;

	BOOL bHit = false;	//�����t���O.

	DWORD dwIndex = 0;		//�C���f�b�N�X�ԍ�.
	D3DXVECTOR3 vVertex[3];	//���_���W.
	FLOAT U = 0, V = 0;			//(out)�d�S�q�b�g���W.

	//���b�V���ƃ��C�̌����𒲂ׂ�.
	D3DXIntersect(
		pTarget->GetMesh(),	//�Ώۃ��b�V��.
		&vecStart,			//�J�n�ʒu.
		&vecDistance,		//���C�̋���.
		&bHit,				//(out)���茋��.
		&dwIndex,	//(out)bHit��True���A���C�̎n�_��.
		//�ł��߂��̖ʂ̃C���f�b�N�X�l�ւ̃|�C���^.
		&U, &V,				//(out)�d�S�q�b�g���W.
		pfDistance,			//�^�[�Q�b�g�Ƃ̋���.
		NULL, NULL);
	if (bHit) {
		//���������Ƃ�.
		FindVerticesOnPoly(
			pTarget->GetMeshForRay(), dwIndex, vVertex);
		//�d�S���W��������_���Z�o.
		//���[�J����_p�́Av0 + U*(v1-v0) + V*(v2-v0)�ŋ��܂�.
		*pvIntersect =
			vVertex[0]
			+ U * (vVertex[1] - vVertex[0])
			+ V * (vVertex[2] - vVertex[0]);

		return true;//�������Ă���.
	}
	return false;//�O��Ă���.
}

//�����ʒu�̃|���S���̒��_��������.
//�����_���W�̓��[�J�����W.
HRESULT clsStageScene::FindVerticesOnPoly(
	LPD3DXMESH pTarget, DWORD dwPolyIndex,
	D3DXVECTOR3* pVecVertices)
{
	//���_���Ƃ̃o�C�g�����擾.
	DWORD dwStride = pTarget->GetNumBytesPerVertex();
	//���_�����擾.
	DWORD dwVertexAmt = pTarget->GetNumVertices();
	//�ʐ����擾.
	DWORD dwPolyAmt = pTarget->GetNumFaces();

	WORD* pwPoly = NULL;

	//�C���f�b�N�X�o�b�t�@�����b�N(�Ǎ����[�h)
	pTarget->LockIndexBuffer(
		D3DLOCK_READONLY, (VOID**)&pwPoly);
	BYTE* pbVertices = NULL;//���_(�o�C�g�^)
	FLOAT* pfVertices = NULL;//���_(float�^)
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;//���_�o�b�t�@.
	pTarget->GetVertexBuffer(&VB);//���_���̎擾.

	//���_�o�b�t�@�̃��b�N.
	if (SUCCEEDED(
		VB->Lock(0, 0, (VOID**)&pbVertices, 0)))
	{
		//�|���S���̒��_�̂P�ڂ��擾.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3]];
		pVecVertices[0].x = pfVertices[0];
		pVecVertices[0].y = pfVertices[1];
		pVecVertices[0].z = pfVertices[2];

		//�|���S���̒��_�̂Q�ڂ��擾.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 1]];
		pVecVertices[1].x = pfVertices[0];
		pVecVertices[1].y = pfVertices[1];
		pVecVertices[1].z = pfVertices[2];

		//�|���S���̒��_�̂R�ڂ��擾.
		pfVertices
			= (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 2]];
		pVecVertices[2].x = pfVertices[0];
		pVecVertices[2].y = pfVertices[1];
		pVecVertices[2].z = pfVertices[2];

		pTarget->UnlockIndexBuffer();	//���b�N����.
		VB->Unlock();	//���b�N����.
	}
	VB->Release();

	return S_OK;
}

//�X�t�B�A�쐬.
HRESULT clsStageScene::InitSphere(clsDX9Mesh* pMesh, float fScale)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;	//���_�o�b�t�@.
	void*		pVertices = NULL;	//���_
	D3DXVECTOR3 vCenter;		//���S.
	float		fRadius;			//���a.

	//���_�o�b�t�@���擾.
	if (FAILED(pMesh->m_pMesh->GetVertexBuffer(&pVB)))
	{
		return E_FAIL;
	}

	//���b�V���̒��_�o�b�t�@�����b�N����.
	if (FAILED(pVB->Lock(0, 0, &pVertices, 0)))
	{
		SAFE_RELEASE(pVB);
		return E_FAIL;
	}

	//���b�V���̊O�ډ~�̒��S�Ɣ��a���v�Z����.
	D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)pVertices,
		pMesh->m_pMesh->GetNumVertices(),//���_�̐�.
		D3DXGetFVFVertexSize(pMesh->m_pMesh->GetFVF()),//���_�̏��.
		&vCenter,	//(out)���S���W.
		&fRadius);	//(out)���a.

	//�A�����b�N.
	pVB->Unlock();
	SAFE_RELEASE(pVB);

	//���S�Ɣ��a���\���̂ɐݒ�.
	pMesh->m_Sphere.vCenter = vCenter;
	pMesh->m_Sphere.fRadius = fRadius * fScale;

	return S_OK;
}