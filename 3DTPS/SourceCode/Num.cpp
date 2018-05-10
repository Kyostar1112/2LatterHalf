#include"Num.h"

clsNum::clsNum()
{
}

clsNum::~clsNum()
{
}
//‰æ‘œ‚É‚µ‚½‚¢”šAŒ…”‚æ‚±‚¹.
void clsNum::Create(int Keta)
{
	for (int i = 0; i < Keta; i++)
	{
		m_vsmpNum.push_back(make_unique<clsSprite2D>());
		m_vsmpNum[i]->Create(Resource->GetSpriteRenderSet().pDevice11,
			Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\NumberTen.png");

		m_vsmpNum[i]->SetSs(m_vsmpNum[i]->GetSs().Base.w, m_vsmpNum[i]->GetSs().Base.h,
			m_vsmpNum[i]->GetSs().Disp.w, m_vsmpNum[i]->GetSs().Disp.h / 10.0f,
			1.0f, 10.0f);
		m_vsmpNum[i]->UpDateSpriteSs();
		m_viDispNum.push_back(0);
	}
}
void clsNum::Init()
{
	for (UINT i = 0; i < m_vsmpNum.size(); i++)
	{
		int j = m_vsmpNum.size();
		m_vsmpNum[i]->SetPos((m_vsmpNum[i]->GetSs().Disp.w - 60.0f)*(j - i - 1), 0.0f);
	}
}

void clsNum::Render(int Num)
{
	for (UINT i = 0; i < m_vsmpNum.size(); i++)
	{
		int tmp = static_cast<int>(pow(10, i));
		m_viDispNum[i] = 9 - Num / tmp;
		m_vsmpNum[i]->SetPatarnV(static_cast<float>(m_viDispNum[i]));
		m_vsmpNum[i]->Render();
	}
}

void clsNum::Release()
{
	for (UINT i = 0; i < m_vsmpNum.size(); i++)
	{
		m_vsmpNum[i].reset();
	}
	m_vsmpNum.clear();
	m_vsmpNum.shrink_to_fit();

	m_viDispNum.clear();
	m_viDispNum.shrink_to_fit();
}