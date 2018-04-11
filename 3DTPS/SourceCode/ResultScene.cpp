#include"ResultScene.h"

clsResultScene::clsResultScene()
{
}

clsResultScene::~clsResultScene()
{
}
namespace ResultScene{
	int ciRank = 3;
	int ciScoredigit = 4;
}

void clsResultScene::Create()
{
	m_smpBgm = make_unique<clsSound>();
	m_smpBgm->Open("Data\\Sound\\BGM\\ResultScene.mp3", "ResultBGM00", Resource->GetSpriteRenderSet().hWnd);
	m_smpBgm->SetVolume(10);

	for (size_t i = 0; i < ResultScene::ciRank; i++)
	{
		m_vsmpDispScore.push_back(make_unique<clsNum>());
		m_vsmpDispScore[i]->Create(ResultScene::ciScoredigit);
		m_vsmpDispScore[i]->SetMulDisp(0.8f);
		m_vsmpDispScore[i]->UpDateSpriteSs();
		m_vsmpDispScore[i]->SetPosX(WND_W / 2 - m_vsmpDispScore[i]->GetDispW() / 4);
		m_vsmpDispScore[i]->SetPosY(m_vsmpDispScore[i]->GetDispH() / 3 + m_vsmpDispScore[i]->GetDispH()*i);
	}

	for (size_t i = 0; i < ResultScene::ciRank; i++)
	{
		m_vsmpScoreRank.push_back(make_unique<clsNum>());
		m_vsmpScoreRank[i]->Create(1);
		m_vsmpScoreRank[i]->SetMulDisp(0.8f);
		m_vsmpScoreRank[i]->UpDateSpriteSs();
		m_vsmpScoreRank[i]->SetPosX(WND_W / 2 - m_vsmpDispScore[i]->GetDispW() / 2);
		m_vsmpScoreRank[i]->SetPosY(m_vsmpDispScore[i]->GetDispH() / 3 + m_vsmpDispScore[i]->GetDispH()*i);
	}

	//クリアシーンの背景.
	m_smpBack = make_unique<clsSprite2D>();
	m_smpBack->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\ResultBack.png");
	m_smpBack->SetDispH(WND_H);
	m_smpBack->SetDispW(WND_W);
	m_smpBack->UpDateSpriteSs();

	//タイトルロゴ.
	m_smpTxt = make_unique<clsSprite2D>();
	m_smpTxt->Create(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\ResultTxt.png");

	m_smpPushTxt = make_unique<clsPushTxt>();
	m_smpPushTxt->Init(Resource->GetSpriteRenderSet().pDevice11, Resource->GetSpriteRenderSet().pContext11);
}

void clsResultScene::Init()
{
	sort(Resource->m_viScore.begin(), Resource->m_viScore.end());
	Resource->m_viScore.erase(unique(Resource->m_viScore.begin(), Resource->m_viScore.end()), Resource->m_viScore.end());
	reverse(Resource->m_viScore.begin(), Resource->m_viScore.end());
	m_viScore.clear();
	for (size_t i = 0; i < Resource->m_viScore.size(); i++)
	{
		m_viScore.push_back((Resource->m_viScore[i]));
	}

	m_smpBgm->SeekToStart();
	m_smpBgm->Play();
	m_smpBack->SetPos(0.0f, 0.0f);
	m_smpTxt->SetPos(0.0f, -250.0f);
}

void clsResultScene::UpDate()
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

void clsResultScene::ModelRender()
{
}

void clsResultScene::SpriteRender()
{
	m_smpBack->Render();
	m_smpTxt->Render();

	m_smpPushTxt->Render();

	for (size_t i = 0; i < ResultScene::ciRank; i++)
	{
		m_vsmpDispScore[i]->Render(m_viScore[i]);
		m_vsmpScoreRank[i]->Render(i + 1);
	}

	BlackOut();
}

void clsResultScene::Release()
{
}