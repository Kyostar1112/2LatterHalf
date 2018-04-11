#ifndef _C_NUM_H_
#define _C_NUM_H_
#define Resource clsResource::GetInstance()

#include"Sprite2D.h"
#include"Resource.h"
#include<cmath>
class clsNum
{
public:
	clsNum();
	~clsNum();

	//âÊëúÇ…ÇµÇΩÇ¢êîéöÅAåÖêîÇÊÇ±Çπ.
	void Create(int Keta);
	void Init();
	void Render(int Num);
	void Release();

	void AddPosX(float Add)
	{
		for (size_t i = 0; i < m_vsmpNum.size(); i++)
		{
			m_vsmpNum[i]->AddPosX(Add);
		}
	}
	void AddPosY(float Add)
	{
		for (size_t i = 0; i < m_vsmpNum.size(); i++)
		{
			m_vsmpNum[i]->AddPosY(Add);
		}
	}

	void AddPosX(float AddX, float AddY)
	{
		for (size_t i = 0; i < m_vsmpNum.size(); i++)
		{
			m_vsmpNum[i]->AddPosX(AddX);
			m_vsmpNum[i]->AddPosY(AddY);
		}
	}
	void SetPosX(float SetPosX)
	{
		int TmpNum = m_vsmpNum.size();
		for (size_t i = 0; i < m_vsmpNum.size(); i++)
		{
			TmpNum--;
			m_vsmpNum[i]->SetPosX(SetPosX + m_vsmpNum[i]->GetSs().Disp.w*TmpNum);
		}
	}
	void SetPosY(float SetPosY)
	{
		for (size_t i = 0; i < m_vsmpNum.size(); i++)
		{
			m_vsmpNum[i]->SetPosY(SetPosY + m_vsmpNum[i]->GetSs().Disp.h);
		}
	}

	void SetMulDisp(float MulPoint)
	{
		for (size_t i = 0; i < m_vsmpNum.size(); i++)
		{
			m_vsmpNum[i]->MulDisp(MulPoint);
		}
	}

	void UpDateSpriteSs()
	{
		for (size_t i = 0; i < m_vsmpNum.size(); i++)
		{
			m_vsmpNum[i]->UpDateSpriteSs();
		}
	}

	float GetDispW()
	{
		float WSIZE = 0.0f;
		for (size_t i = 0; i < m_vsmpNum.size(); i++)
		{
			WSIZE += m_vsmpNum[i]->GetSs().Disp.w;
		}
		return WSIZE;
	}

	float GetDispH()
	{
		float H;
		for (size_t i = 0; i < m_vsmpNum.size(); i++)
		{
			H = m_vsmpNum[i]->GetSs().Disp.h;
		}
		return H;
	}

	D3DXVECTOR3 GetPos()
	{
		D3DXVECTOR3 H;
		H = m_vsmpNum[0]->GetPos();
		return H;
	}
private:
	vector<unique_ptr<clsSprite2D>> m_vsmpNum;
	vector<int> m_viDispNum; //àÍÇØÇΩÇ√Ç¬.

	int m_iNum; //àÍÇØÇΩÇ√Ç¬.
};

#endif  //#ifdef _C_NUM_H_