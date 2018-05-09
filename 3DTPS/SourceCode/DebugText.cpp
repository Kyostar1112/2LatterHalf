#include "DebugText.h"

//ｼｪｰﾀﾞﾌｧｲﾙ名(ﾊﾟｽも含む).
const char SHADER_NAME[] = "Shader\\DebugText.hlsl";

//ｺﾝｽﾄﾗｸﾀ.
clsDebugText::clsDebugText()
{
	ZeroMemory(this, sizeof(clsDebugText));//初期化.

	m_fScale = 1.0f;
}

//ﾃﾞｽﾄﾗｸﾀ.
clsDebugText::~clsDebugText()
{
}

//初期化関数.
HRESULT clsDebugText::Init(
	ID3D11DeviceContext* pContext,
	DWORD dwWidth, DWORD dwHeight,
	float fSize, D3DXVECTOR4 vColor)
{
	m_fAlpha = vColor.w;
	m_vColor = vColor;
	m_fScale = fSize / 25.0f;

	//95文字分繰り返し.
	for (int i = 0; i < 95; i++) {
		m_fKerning[i] = 10.0f;
	}
	//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄをｺﾋﾟｰ.
	m_pDeviceContext11 = pContext;
	m_pDeviceContext11->GetDevice(&m_pDevice11);

	//windowｻｲｽﾞ.
	m_dwWindowWidth = dwWidth;
	m_dwWindowHeight = dwHeight;

	//ﾌｫﾝﾄ毎にｸｱｯﾄﾞ(矩形)作成
	float left = 0.0f, top = 0.0f, right = 0.0f, bottom = 0.0f;
	int cnt = 0;
	//2重ﾙｰﾌﾟで1文字ずつ指定する.
	for (int k = 0; k < 10; k++)
	{
		for (int i = 0; i < 10; i++)
		{
			left = k * WDIMENSION;
			top = i * DIMENSION;
			right = left + m_fKerning[cnt];
			bottom = top + DIMENSION;

			left /= TEX_DIMENSION;
			top /= TEX_DIMENSION;
			right /= TEX_DIMENSION;
			bottom /= TEX_DIMENSION;

			TextVertex vertices[] =
			{
				D3DXVECTOR3(0.0f, DIMENSION, 0.0f), D3DXVECTOR2(left, bottom),	//頂点1.
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(left, top),		//頂点2.
				D3DXVECTOR3(WDIMENSION, DIMENSION, 0.0f), D3DXVECTOR2(right, bottom),	//頂点3.
				D3DXVECTOR3(WDIMENSION, 0.0, 0.0f), D3DXVECTOR2(right, top)		//頂点4.
			};
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(TextVertex) * 4;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = vertices;
			if (FAILED(m_pDevice11->CreateBuffer(
				&bd, &InitData, &m_pVertexBuffer[cnt])))
			{
				MessageBox(NULL,
					"頂点ﾊﾞｯﾌｧ作成失敗(DebugText:Init)",
					"error", MB_OK);
				return E_FAIL;
			}
			cnt++;
		}
	}

	//ﾃｸｽﾁｬ用ｻﾝﾌﾟﾗ-作成.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	//ﾃｸｽﾁｬﾌｨﾙﾀと貼り付け方の指定.
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if (FAILED(m_pDevice11->CreateSamplerState(
		&SamDesc, &m_pSampleLinear)))
	{
		MessageBox(NULL,
			"ｻﾝﾌﾟﾗ作成失敗(DebugText:Init)",
			"", MB_OK);
		return E_FAIL;
	}

	//ﾌｫﾝﾄのﾃｸｽﾁｬ作成.
	if (FAILED(
		D3DX11CreateShaderResourceViewFromFile(
			m_pDevice11,
			"Data\\DebugText\\ascii.png",
			NULL, NULL,
			&m_pAsciiTexture, NULL)))
	{
		MessageBox(NULL,
			"ﾌｫﾝﾄﾃｸｽﾁｬ作成失敗(DebugText:Init)",
			"error", MB_OK);
		return E_FAIL;
	}

	//=====================================
	//	hlslﾌｧｲﾙ読込.
	//=====================================
	ID3DBlob *pCompileShader = NULL;
	ID3DBlob *pErrors = NULL;
	if (FAILED(D3DX11CompileFromFile(
		SHADER_NAME, NULL, NULL,
		"VS", "vs_5_0", 0, 0,
		NULL, &pCompileShader,
		&pErrors, NULL)))
	{
		MessageBox(NULL,
			"hlsl読込失敗(VS)",
			"DebugText:Init", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice11->CreateVertexShader(
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompileShader);
		MessageBox(NULL,
			"ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ作成失敗",
			"DebugText:Init", MB_OK);
		return E_FAIL;
	}

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout, numElements,
			pCompileShader->GetBufferPointer(),
			pCompileShader->GetBufferSize(),
			&m_pVertexLayout)))
	{
		MessageBox(NULL,
			"頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成失敗",
			"DebugText:Init", MB_OK);
		return E_FAIL;
	}

	//ﾋﾟｸｾﾙｼｪｰﾀﾞ作成.
	if (FAILED(D3DX11CompileFromFile(
		SHADER_NAME, NULL, NULL,
		"PS", "ps_5_0", 0, 0,
		NULL, &pCompileShader,
		&pErrors, NULL)))
	{
		MessageBox(NULL,
			"hlsl読込失敗(PS)",
			"DebugText:Init", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice11->CreatePixelShader(
		pCompileShader->GetBufferPointer(),
		pCompileShader->GetBufferSize(),
		NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompileShader);
		MessageBox(NULL,
			"ﾋﾟｸｾﾙｼｪｰﾀﾞ作成失敗",
			"DebugText:Init", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompileShader);

	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(TEXT_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice11->CreateBuffer(
		&cb, NULL, &m_pConstantBuffer)))
	{
		MessageBox(NULL,
			"ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成",
			"DebugText:Init", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//透過(ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ)設定の切り替え.
void clsDebugText::SetBlend(bool flg)
{
	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ用ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ構造体.
	//pngﾌｧｲﾙ内にｱﾙﾌｧ情報があるので、
	//透過するようにﾌﾞﾚﾝﾄﾞｽﾃｰﾄを設定する.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));//初期化.

	blendDesc.IndependentBlendEnable
		= false;//false:RenderTarget[0]のﾒﾝﾊﾞｰのみが使用する.
	//true :RenderTarget[0～7]が使用できる.
	//      (ﾚﾝﾀﾞｰﾀｰｹﾞｯﾄ毎に独立したﾌﾞﾚﾝﾄﾞ処理)
	blendDesc.AlphaToCoverageEnable
		= false;//true :ｱﾙﾌｧﾄｩｶﾊﾞﾚｯｼﾞを使用する.
	blendDesc.RenderTarget[0].BlendEnable
		= flg;	//true :ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを使用する.
	blendDesc.RenderTarget[0].SrcBlend	//元素材に対する設定.
		= D3D11_BLEND_SRC_ALPHA;		//	ｱﾙﾌｧﾌﾞﾚﾝﾄﾞを指定.
	blendDesc.RenderTarget[0].DestBlend	//重ねる素材に対する設定.
		= D3D11_BLEND_INV_SRC_ALPHA;	//	ｱﾙﾌｧﾌﾞﾚﾝﾄﾞの反転を指定.

	blendDesc.RenderTarget[0].BlendOp	//ﾌﾞﾚﾝﾄﾞｵﾌﾟｼｮﾝ.
		= D3D11_BLEND_OP_ADD;			//	ADD:加算合成.

	blendDesc.RenderTarget[0].SrcBlendAlpha	//元素材のｱﾙﾌｧに対する設定.
		= D3D11_BLEND_ONE;					//	そのまま使用.
	blendDesc.RenderTarget[0].DestBlendAlpha//重ねる素材のｱﾙﾌｧに対する設定.
		= D3D11_BLEND_ZERO;					//	何もしない.

	blendDesc.RenderTarget[0].BlendOpAlpha	//ｱﾙﾌｧのﾌﾞﾚﾝﾄﾞｵﾌﾟｼｮﾝ.
		= D3D11_BLEND_OP_ADD;				//	ADD:加算合成.

	blendDesc.RenderTarget[0].RenderTargetWriteMask	//ﾋﾟｸｾﾙ毎の書き込みﾏｽｸ.
		= D3D11_COLOR_WRITE_ENABLE_ALL;				//	全ての成分(RGBA)へのﾃﾞｰﾀの格納を許可する.

	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成.
	if (FAILED(
		m_pDevice11->CreateBlendState(
			&blendDesc, &m_pBlendState)))
	{
		MessageBox(NULL, "ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成失敗", "ｴﾗｰ", MB_OK);
	}

	//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄの設定.
	UINT mask = 0xffffffff;	//ﾏｽｸ値.
	m_pDeviceContext11->OMSetBlendState(
		m_pBlendState, NULL, mask);
}

//ﾚﾝﾀﾞﾘﾝｸﾞ関数.
void clsDebugText::Render(char* text, int x, int y)
{
	//ﾋﾞｭｰﾄﾗﾝｽﾌｫｰﾑ.
	D3DXVECTOR3 vEye(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 vLook(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(
		&m_mView, &vEye, &vLook, &vUp);

#if 0
	//ﾌﾟﾛｼﾞｪｸｼｮﾝﾄﾗﾝｽﾌｫｰﾑ(固定)
	D3DMATRIX mOtho = {
		2.0f / (float)(m_dwWindowWidth), 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / (float)(m_dwWindowHeight), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};
#else
	D3DXMATRIX mOtho;
	D3DXMatrixIdentity(&mOtho);//単位行列.
	mOtho._11 = 2.0f / (float)(m_dwWindowWidth);
	mOtho._22 = -2.0f / (float)(m_dwWindowHeight);
	mOtho._41 = -1.0f;
	mOtho._42 = 1.0f;
#endif
	m_mProj = mOtho;

	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰ.
	m_pDeviceContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄをｾｯﾄ.
	m_pDeviceContext11->IASetInputLayout(
		m_pVertexLayout);

	//使用するｼｪｰﾀﾞの登録.
	m_pDeviceContext11->VSSetShader(
		m_pVertexShader, NULL, 0);
	m_pDeviceContext11->PSSetShader(
		m_pPixelShader, NULL, 0);

	//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧを使うｼｪｰﾀﾞの登録.
	m_pDeviceContext11->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);
	m_pDeviceContext11->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pDeviceContext11->PSSetSamplers(
		0, 1, &m_pSampleLinear);
	m_pDeviceContext11->PSSetShaderResources(
		0, 1, &m_pAsciiTexture);

	SetBlend(true);

	//文字数分ﾙｰﾌﾟ.
	for (int i = 0; i < strlen(text); i++)
	{
		char font = text[i];
		int index = font - 32;	//ﾌｫﾝﾄｲﾝﾃﾞｯｸｽ作成.

		//ﾌｫﾝﾄﾚﾝﾀﾞﾘﾝｸﾞ.
		RenderFont(index, x, y);

		x += m_fKerning[index];
	}

	SetBlend(false);
}

//ﾌｫﾝﾄﾚﾝﾀﾞﾘﾝｸﾞ関数.
void clsDebugText::RenderFont(
	int FontIndex, int x, int y)
{
	//ﾜｰﾙﾄﾞ変換.
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity(&mWorld);
	D3DXMatrixTranslation(
		&mWorld, (float)x, (float)y,
		-100.0f);	//Z(-100)で手前表示可能.

	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE	pData;
	TEXT_CONSTANT_BUFFER		cb;
	if (SUCCEEDED(m_pDeviceContext11->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成行列を渡す.
		D3DXMATRIX m = mWorld * m_mView * m_mProj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;
		//ｶﾗｰを渡す.
		cb.vColor = m_vColor;
		//透明度を渡す.
		cb.fAlpha.x = m_fAlpha;

		memcpy_s(pData.pData, pData.RowPitch,
			(void*)(&cb), sizeof(cb));
		m_pDeviceContext11->Unmap(
			m_pConstantBuffer, 0);
	}
	//ﾊﾞｰﾃｯｸｽﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = sizeof(TextVertex);
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer[FontIndex],
		&stride, &offset);

	//描画.
	UINT ColoKey = 0xffffffff;
	m_pDeviceContext11->OMSetBlendState(
		m_pBlendState, NULL, ColoKey);
	m_pDeviceContext11->Draw(4, 0);
	m_pDeviceContext11->OMSetBlendState(
		0, NULL, ColoKey);
}