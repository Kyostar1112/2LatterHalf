#ifndef _SP_2D_MANAGER_PUSH_BUTTON_H_
#define _SP_2D_MANAGER_PUSH_BUTTON_H_

#include "Sprite2D.h"

class clsPushTxt
{
public:

	enum enAlphaMode
	{
		Flassing = 0,
		Out
	};

	clsPushTxt();
	~clsPushTxt();

	void Init(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	void InitSetPos();
	void Render();
	void Release();

	void AddAlpha(float AlphaPoint);
	void SetAlphaMode(enAlphaMode AlphaMode) {
		m_enAlphaMode = AlphaMode;
	}

private:
	enAlphaMode m_enAlphaMode;
	float m_fAlphaPoint;//ìßâﬂÇ∑ÇÈç€ÇÃïœâªó .

	unique_ptr<clsSprite2D> m_smpAButton;	//AÉ{É^ÉìÇÀÇ•Ç≈Ç‚ÇÒÇ∑.
	unique_ptr<clsSprite2D> m_smpPushTxt;	//PUSH KETÇÀÇ•Ç≈Ç‚ÇÒÇ∑.
};

#endif//#ifndef _SP_2D_MANAGER_PUSH_BUTTON_H_