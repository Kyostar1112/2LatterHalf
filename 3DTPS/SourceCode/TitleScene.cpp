#include"TitleScene.h"

clsTitleScene::clsTitleScene()
{
}

clsTitleScene::~clsTitleScene()
{
}

void clsTitleScene::Create()
{
	//タイトルの背景.
	m_smpTitleBack = make_unique<clsSprite2D>();
	m_smpTitleBack->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\Back.png");
	m_smpTitleBack->SetDispH(WND_H);
	m_smpTitleBack->SetDispW(WND_W);
	m_smpTitleBack->UpDateSpriteSs();

	//タイトルロゴ.
	m_smpTitleLogo = make_unique<clsSprite2D>();
	m_smpTitleLogo->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\TitleLogo.png");

	m_smpPushTxt = make_unique<clsPushTxt>();
	m_smpPushTxt->Init(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11);

	m_smpBgm = make_unique<clsSound>();
	m_smpBgm->Open("Data\\Sound\\BGM\\TitleScene.mp3", "TitleBGM00", Resource->GetSpriteRenderSet().hWnd);
	m_smpBgm->SetVolume(10);
}

void clsTitleScene::Init()
{
	m_smpBgm->Play();
	m_smpBgm->SeekToStart();
	m_smpTitleBack->SetPos(0.0f, 0.0f);
	m_smpTitleLogo->SetPos(WND_W / 2 - m_smpTitleLogo->GetSs().Disp.w / 2, WND_H / 2 - m_smpTitleLogo->GetSs().Disp.h / 2);
}

void clsTitleScene::UpDate()
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

void clsTitleScene::Render()
{
	SetDepth(false);
	m_smpTitleBack->Render();
	m_smpTitleLogo->Render();

	m_smpPushTxt->Render();
	BlackOut();
	SetDepth(true);
}

void clsTitleScene::Release()
{
}