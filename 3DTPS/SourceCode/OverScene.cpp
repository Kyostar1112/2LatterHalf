#include"OverScene.h"

clsOverScene::clsOverScene()
{
}

clsOverScene::~clsOverScene()
{
}

void clsOverScene::Create()
{
	//ゲームオーハ゛ーの背景.
	m_smpOverBack = make_unique<clsSprite2D>();
	m_smpOverBack->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\Back.png");
	m_smpOverBack->SetDispH(WND_H);
	m_smpOverBack->SetDispW(WND_W);
	m_smpOverBack->UpDateSpriteSs();

	//タイトルロゴ.
	m_smpOverTxt = make_unique<clsSprite2D>();
	m_smpOverTxt->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\OverTxt.png");

	m_smpPushTxt = make_unique<clsPushTxt>();
	m_smpPushTxt->Init(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11);

	m_smpBgm = make_unique<clsSound>();
	m_smpBgm->Open("Data\\Sound\\BGM\\OverScene.mp3", "OverBGM00", Resource->GetSpriteRenderSet().hWnd);
	m_smpBgm->SetVolume(10);
}

void clsOverScene::Init()
{
	Resource->m_smpFile->Write("Data\\Txt\\ScoreRank.csv", Resource->m_viScore);
	m_smpBgm->Play();
	m_smpBgm->SeekToStart();
	m_smpOverBack->SetPos(0.0f, 0.0f);
	m_smpOverTxt->SetPos(WND_W / 2 - m_smpOverTxt->GetSs().Disp.w / 2, WND_H / 2 - m_smpOverTxt->GetSs().Disp.h / 2);
}

void clsOverScene::UpDate()
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

void clsOverScene::Render()
{
	SetDepth(false);
	m_smpOverBack->Render();
	m_smpOverTxt->Render();

	m_smpPushTxt->Render();
	BlackOut();
	SetDepth(true);
}

void clsOverScene::Release()
{
}