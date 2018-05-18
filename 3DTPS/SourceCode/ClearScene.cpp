#include"ClearScene.h"

clsClearScene::clsClearScene()
{
}

clsClearScene::~clsClearScene()
{
}

void clsClearScene::Create()
{
	m_smpBgm = make_unique<clsSound>();
	m_smpBgm->Open("Data\\Sound\\BGM\\ClearScene.mp3", "ClearBGM00", Resource->GetSpriteRenderSet().hWnd);
	m_smpBgm->SetVolume(10);

	//クリアシーンの背景.
	m_smpClearBack = make_unique<clsSprite2D>();
	m_smpClearBack->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\Back.png");
	m_smpClearBack->SetDispH(WND_H);
	m_smpClearBack->SetDispW(WND_W);
	m_smpClearBack->UpDateSpriteSs();

	//ゲームクリア.
	m_smpClearTxt = make_unique<clsSprite2D>();
	m_smpClearTxt->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\ClearTxt.png");
	m_smpClearTxt->MulDisp(0.7f);

	//最終スコア.
	m_smpThankyouTxt = make_unique<clsSprite2D>();
	m_smpThankyouTxt->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\Thankyou.png");
	m_smpThankyouTxt->MulDisp(0.3f);

	//最終スコアテキスト.
	m_smpLastScoreTxt = make_unique<clsSprite2D>();
	m_smpLastScoreTxt->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\LastScore.png");
	m_smpLastScoreTxt->MulDisp(0.2f);

	m_smpLastScoreNum = make_unique<clsNum>();
	m_smpLastScoreNum->Create(4);

	m_smpPushTxt = make_unique<clsPushTxt>();
	m_smpPushTxt->Init(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11);
}

void clsClearScene::Init()
{
	Resource->m_smpFile->Write("Data\\Txt\\ScoreRank.csv", Resource->m_viScore);
	m_smpBgm->SeekToStart();
	m_smpBgm->Play();
	m_smpClearBack->SetPos(0.0f, 0.0f);
	m_smpClearTxt->SetPos(WND_W/2 - m_smpClearTxt->GetSs().Disp.w / 2,
		WND_H / 2 - m_smpClearTxt->GetSs().Disp.h / 2 - 350.0f);

	m_smpThankyouTxt->SetPos(WND_W / 4 - m_smpThankyouTxt->GetSs().Disp.w / 2,
		m_smpClearTxt->GetPos().y +	m_smpClearTxt->GetSs().Disp.h);

	m_smpLastScoreTxt->SetPos(WND_W / 4 - m_smpLastScoreTxt->GetSs().Disp.w / 2,
		m_smpThankyouTxt->GetPos().y +	m_smpThankyouTxt->GetSs().Disp.h + 200);
	m_smpLastScoreNum->SetPosX(WND_W / 2 - m_smpLastScoreNum->GetDispW()/2);
	m_smpLastScoreNum->SetPosY(220.0f);
}

void clsClearScene::UpDate()
{
	if (m_smpBgm->IsStopped())
	{
		m_smpBgm->SeekToStart();
	}
	if (!m_smpBgm->IsPlaying())
	{
		m_smpBgm->Play();
	}
}

void clsClearScene::Render()
{
	SetDepth(false);
	m_smpClearBack->Render();
	m_smpClearTxt->Render();
	m_smpThankyouTxt->Render();
	m_smpLastScoreTxt->Render();
	m_smpLastScoreNum->Render(Resource->m_viScore.back());
	m_smpPushTxt->Render();
	BlackOut();
	SetDepth(true);
}

void clsClearScene::Release()
{
}