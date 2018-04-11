#include"BaseScene.h"

clsSceneBase::clsSceneBase()
{
	m_smpBlack = make_unique<clsSprite2D>();  //ˆÃ“]—p.

	m_smpWhite = make_unique<clsSprite2D>(); //ˆÃ“]—p.
	m_smpBlack->Create(Resource->GetSpriteRenderSet().pDevice11,
		Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\Black.png");

	m_smpWhite->Create(Resource->GetSpriteRenderSet().pDevice11,
		Resource->GetSpriteRenderSet().pContext11, "Data\\Image\\White.png");
}

clsSceneBase::~clsSceneBase()
{
	if (m_smpBlack) {
		m_smpBlack.reset();
	}
	if (m_smpWhite) {
		m_smpWhite.reset();
	}
}

void clsSceneBase::BlackStart(float ChaAmo, bool Color)
{
	if (m_smpBlack->m_BlackMode == Idle && Color)
	{
		m_fChangePoint = ChaAmo;
		m_bColor = Color;
		m_smpBlack->m_BlackMode = In;
		m_smpBlack->SetAlpha(0.0f);
	}
	else if (m_smpWhite->m_BlackMode == Idle && !Color)
	{
		m_fChangePoint = ChaAmo;
		m_bColor = Color;
		m_smpWhite->m_BlackMode = In;
		m_smpWhite->SetAlpha(0.0f);
	}
}
void clsSceneBase::BlackOut()
{
	if (m_bColor)
	{
		if (m_smpBlack->m_BlackMode != Idle)
		{
			if (m_smpBlack->m_BlackMode == Change)
			{
				m_smpBlack->m_BlackMode = Out;
			}
			float tmp = m_smpBlack->GetAlpha();
			m_smpBlack->SetPos(0.0f, 0.0f);
			if (m_smpBlack->m_BlackMode == In){
				m_smpBlack->AddAlpha(m_fChangePoint);
				if (tmp > 1.0f){
					m_smpBlack->m_BlackMode = Change;
				}
			}
			else
			{
				if (m_smpBlack->m_BlackMode == Out)
				{
					if (m_smpBlack->GetAlpha() > 0.0f)
					{
						m_smpBlack->AddAlpha(-1 * m_fChangePoint);
					}
					else
					{
						m_smpBlack->m_BlackMode = Idle;
					}
				}
			}
			m_smpBlack->Render();
		}
	}
	else{
		if (m_smpWhite->m_BlackMode != Idle)
		{
			m_smpWhite->SetPos(0.0f, 0.0f);
			if (m_smpWhite->m_BlackMode == Change)
			{
				m_smpWhite->m_BlackMode = Out;
			}
			float tmp = m_smpWhite->GetAlpha();
			if (m_smpWhite->m_BlackMode == In)
			{
				m_smpWhite->AddAlpha(m_fChangePoint);
				if (tmp > 1.0f)
				{
					m_smpWhite->m_BlackMode = Change;
				}
			}
			else
			{
				if (m_smpWhite->m_BlackMode == Out)
				{
					if (m_smpWhite->GetAlpha() > 0.0f)
					{
						m_smpWhite->AddAlpha(-1 * m_fChangePoint);
					}
					else
					{
						m_smpWhite->m_BlackMode = Idle;
					}
				}
			}
			m_smpWhite->Render();
		}
	}
}