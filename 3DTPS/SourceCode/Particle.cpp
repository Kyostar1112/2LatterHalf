#include "Particle.h"

//ｼｪｰﾀﾞｰ名(ﾊﾟｽも含む)
const char SHADER_NAME[] = "Shader\\PointSprite.hlsl";

//ｺﾝｽﾄﾗｸﾀ.
clsParticle::clsParticle(
	int MaxParticle,		//ﾊﾟｰﾃｨｸﾙ最大数.
	D3DXVECTOR3 &vEmitPos)	//放射位置.
{
	ZeroMemory(this, sizeof(clsParticle));

	m_MaxParticle = MaxParticle;
	m_vEmitPos = vEmitPos;

	//ﾊﾟｰﾃｨｸﾙ数分の配列を作成.
	m_pParticleArray = new PART[m_MaxParticle];

	//ﾊﾟｰﾃｨｸﾙ数分繰り返し(ﾊﾟｰﾃｨｸﾙの初期化)
	for (int i = 0; i < m_MaxParticle; i++)
	{
		m_pParticleArray[i].vPos = m_vEmitPos;//放射位置(初期位置)
		m_pParticleArray[i].vDir			//方向.
			= D3DXVECTOR3(
			(2.0f*((float)rand() / (float)RAND_MAX) - 1.0f) / 100.0f, //X:-0.01～0.01
				(float)rand() / (float)RAND_MAX,    //Y: 0.0～1.0.
				(2.0f*((float)rand() / (float)RAND_MAX) - 1.0f) / 100.0f);//Z:-0.01～0.01

		D3DXVec3Normalize(
			&m_pParticleArray[i].vDir,
			&m_pParticleArray[i].vDir);//正規化.

		//速度.
		m_pParticleArray[i].fSpeed
			= (5 + ((float)rand() / (float)RAND_MAX))*0.001f;

		//開始ﾌﾚｰﾑ.
		m_pParticleArray[i].iBirthFrame = rand();
	}
}

//ﾃﾞｽﾄﾗｸﾀ.
clsParticle::~clsParticle()
{
}

//初期化.
HRESULT clsParticle::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11)
{
	m_pDevice11 = pDevice11;
	m_pDeviceContext11 = pContext11;

	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	if (FAILED(InitPointSprite()))
	{
		return E_FAIL;
	}
	if (FAILED(InitTexture("Data\\Image\\kemuri.png")))
	{
		return E_FAIL;
	}

	return S_OK;
}

//ﾎﾟｲﾝﾄｽﾌﾟﾗｲﾄの初期化.
HRESULT clsParticle::InitPointSprite()
{
	//ﾊﾞｰﾃｯｸｽﾊﾞｯﾌｧ作成.
	PSpriteVertex vertices[] =
	{
		D3DXVECTOR3(-0.5f, 0.5f, 0.0f),
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PSpriteVertex) * 1;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&bd, &InitData, &m_pVertexBuffer)))
	{
		MessageBox(NULL, "頂点ﾊﾞｯﾌｧ作成失敗",
			"InitPointSprite:Init", MB_OK);
		return E_FAIL;
	}

	//ﾊﾞｰﾃｯｸｽﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = sizeof(PSpriteVertex);
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset);

	return S_OK;
}

//======================================
//	HLSLﾌｧｲﾙを読み込みｼｪｰﾀﾞを作成する.
//	HLSL:HIGH-LEVEL-SHADER-LANGUAGE.
//======================================
HRESULT clsParticle::InitShader()
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	UINT uCompileFlag = 0;

#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSLからﾊﾞｰﾃｯｸｽｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//ｼｪｰﾀﾞﾌｧｲﾙ名(HLSLﾌｧｲﾙ)
			NULL,			//ﾏｸﾛ定義の配列へのﾎﾟｲﾝﾀ(未使用)
			NULL,			//ｲﾝｸﾙｰﾄﾞﾌｧｲﾙを扱うｲﾝﾀｰﾌｪｲｽへのﾎﾟｲﾝﾀ(未使用)
			"VS",			//ｼｪｰﾀﾞｴﾝﾄﾘｰﾎﾟｲﾝﾄ関数の名前.
			"vs_5_0",		//ｼｪｰﾀﾞのﾓﾃﾞﾙを指定する文字列(ﾌﾟﾛﾌｧｲﾙ)
			uCompileFlag,	//ｼｪｰﾀﾞｺﾝﾊﾟｲﾙﾌﾗｸﾞ.
			0,				//ｴﾌｪｸﾄｺﾝﾊﾟｲﾙﾌﾗｸﾞ(未使用)
			NULL,			//ｽﾚｯﾄﾞﾎﾟﾝﾌﾟｲﾝﾀｰﾌｪｲｽへのﾎﾟｲﾝﾀ(未使用)
			&pCompiledShader,//ﾌﾞﾛﾌﾞを格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			&pErrors,		//ｴﾗｰと警告一覧を格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			NULL)))		//戻り値へのﾎﾟｲﾝﾀ(未使用)
	{
		MessageBox(NULL, "hlsl読込失敗", "ｴﾗｰ", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したﾌﾞﾛﾌﾞから「ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ」を作成.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pVertexShader)))//(out)ﾊﾞｰﾃｯｸｽｼｪｰﾀﾞ.
	{
		MessageBox(NULL, "vs作成失敗", "ｴﾗｰ", MB_OK);
		return E_FAIL;
	}

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄを定義.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",				//位置.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,//DXGIのﾌｫｰﾏｯﾄ(32bit float型*3)
			0,
			0,							//ﾃﾞｰﾀの開始位置.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		},
		//		{
		//			"TEXCOORD",		//ﾃｸｽﾁｬ位置.
		//			0,
		//			DXGI_FORMAT_R32G32_FLOAT,	//DXGIのﾌｫｰﾏｯﾄ(32bit float型*2)
		//			0,
		//			12,							//ﾃｸｽﾁｬﾃﾞｰﾀの開始位置.
		//			D3D11_INPUT_PER_VERTEX_DATA, 0
		//		}
	};
	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄの配列要素数を算出.
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄの作成.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))//(out)頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ.
	{
		MessageBox(NULL, "頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄ作成失敗", "ｴﾗｰ", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSLからｼﾞｵﾒﾄﾘｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//ｼｪｰﾀﾞﾌｧｲﾙ名(HLSLﾌｧｲﾙ)
			NULL,
			NULL,
			"GS_Point",		//ｼｪｰﾀﾞｴﾝﾄﾘｰﾎﾟｲﾝﾄ関数の名前.
			"gs_5_0",		//ｼｪｰﾀﾞのﾓﾃﾞﾙを指定する文字列(ﾌﾟﾛﾌｧｲﾙ)
			uCompileFlag,	//ｼｪｰﾀﾞｺﾝﾊﾟｲﾙﾌﾗｸﾞ.
			0,
			NULL,
			&pCompiledShader,//ﾌﾞﾛﾌﾞを格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			&pErrors,
			NULL)))
	{
		MessageBox(NULL, "hlsl(gs)読込失敗", "ｴﾗｰ", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したﾌﾞﾛﾌﾞから「ｼﾞｵﾒﾄﾘｼｪｰﾀﾞ」を作成.
	if (FAILED(
		m_pDevice11->CreateGeometryShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pGeometryShader)))//(out)ｼﾞｵﾒﾄﾘｼｪｰﾀﾞ.
	{
		MessageBox(NULL, "gs作成失敗", "ｴﾗｰ", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);//ﾌﾞﾛﾌﾞ解放.

	//HLSLからﾋﾟｸｾﾙｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//ｼｪｰﾀﾞﾌｧｲﾙ名(HLSLﾌｧｲﾙ)
			NULL,
			NULL,
			"PS",			//ｼｪｰﾀﾞｴﾝﾄﾘｰﾎﾟｲﾝﾄ関数の名前.
			"ps_5_0",		//ｼｪｰﾀﾞのﾓﾃﾞﾙを指定する文字列(ﾌﾟﾛﾌｧｲﾙ)
			uCompileFlag,	//ｼｪｰﾀﾞｺﾝﾊﾟｲﾙﾌﾗｸﾞ.
			0,
			NULL,
			&pCompiledShader,//ﾌﾞﾛﾌﾞを格納するﾒﾓﾘへのﾎﾟｲﾝﾀ.
			&pErrors,
			NULL)))
	{
		MessageBox(NULL, "hlsl(ps)読込失敗", "ｴﾗｰ", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	//上記で作成したﾌﾞﾛﾌﾞから「ﾋﾟｸｾﾙｼｪｰﾀﾞ」を作成.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pPixelShader)))//(out)ﾋﾟｸｾﾙｼｪｰﾀﾞ.
	{
		MessageBox(NULL, "ps作成失敗", "ｴﾗｰ", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);//ﾌﾞﾛﾌﾞ解放.

	//ｺﾝｽﾀﾝﾄ(定数)ﾊﾞｯﾌｧ作成 ｼｪｰﾀﾞに特定の数値を送るﾊﾞｯﾌｧ.
	//ここでは変換行列渡し用.
	//ｼｪｰﾀﾞにWorld,View,Projection行列を渡す.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧを指定.
	cb.ByteWidth = sizeof(PSPRITESHADER_CONSTANT_BUFFER);//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｻｲｽﾞ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//書き込みでｱｸｾｽ.
	cb.MiscFlags = 0;//その他のﾌﾗｸﾞ(未使用)
	cb.StructureByteStride = 0;//構造体のｻｲｽﾞ(未使用)
	cb.Usage = D3D11_USAGE_DYNAMIC;//使用方法:直接書き込み.

	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			NULL,
			&m_pConstantBuffer)))
	{
		MessageBox(NULL, "ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成失敗", "ｴﾗｰ", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//ﾎﾟｲﾝﾄｽﾌﾟﾗｲﾄを画面にﾚﾝﾀﾞﾘﾝｸﾞ.
void clsParticle::Render(
	D3DXMATRIX &mView, D3DXMATRIX &mProj,
	D3DXVECTOR3 &vEye, D3DXVECTOR3 &vPos)
{
	D3DXMATRIX mWorld;

	//ﾜｰﾙﾄﾞﾄﾗﾝｽﾌｫｰﾑ.
	// 動作確認ｺｰﾄﾞ(要修正)
	//	static float x = -1.0f;
	float fScale = 0.1f;
	//	x += 0.001f; //少しずつ右へ移動.

	D3DXMATRIX mScale, mTrans;
	//拡大・縮小.
	D3DXMatrixScaling(&mScale, fScale, fScale, fScale);
	//平行移動.
	//	D3DXMatrixTranslation( &mTrans, x, 1.0f, 0.0f );
	D3DXMatrixTranslation(&mTrans,
		vPos.x, vPos.y, vPos.z);
	mWorld = mScale * mTrans;

	//使用するｼｪｰﾀﾞのｾｯﾄ.
	m_pDeviceContext11->VSSetShader(m_pVertexShader,
		NULL, 0);	//頂点ｼｪｰﾀﾞ.
	m_pDeviceContext11->GSSetShader(m_pGeometryShader,
		NULL, 0);	//ｼﾞｵﾒﾄﾘｼｪｰﾀﾞ
	m_pDeviceContext11->PSSetShader(m_pPixelShader,
		NULL, 0);	//ﾋﾟｸｾﾙｼｪｰﾀﾞ.

	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	PSPRITESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(m_pDeviceContext11->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成行列を渡す.
		D3DXMATRIX m = mWorld * mView * mProj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;

		memcpy_s(pData.pData, pData.RowPitch,
			(void*)(&cb), sizeof(cb));
		m_pDeviceContext11->Unmap(m_pConstantBuffer, 0);
	}

	//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧをどのｼｪｰﾀﾞで使うか？.
	m_pDeviceContext11->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);
	m_pDeviceContext11->GSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);
	m_pDeviceContext11->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄをｾｯﾄ.
	m_pDeviceContext11->IASetInputLayout(
		m_pVertexLayout);
	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pDeviceContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
	m_pDeviceContext11->PSSetSamplers(
		0, 1, &m_pSampleLinear);
	m_pDeviceContext11->PSSetShaderResources(
		0, 1, &m_pTexture);

	//透過設定.
	SetBlend(true);

	//ﾌﾟﾘﾐﾃｨﾌﾞをﾚﾝﾀﾞﾘﾝｸﾞ.
	m_pDeviceContext11->Draw(1, 0);

	//ｼﾞｵﾒﾄﾘｼｪｰﾀﾞを無効にする.
	m_pDeviceContext11->GSSetShader(NULL, NULL, 0);
	//	m_pDeviceContext11->GSSetConstantBuffers( 0, 0, NULL );
}

//ｼｪｰﾀﾞをﾃﾞﾀｯﾁする.
void clsParticle::DetachShader()
{
	//各ｼｪｰﾀﾞをﾃﾞﾀｯﾁする.
	m_pDeviceContext11->VSSetShader(NULL, NULL, 0);//頂点ｼｪｰﾀﾞ.
	m_pDeviceContext11->HSSetShader(NULL, NULL, 0);//ﾊﾙｼｪｰﾀﾞ.

	// ～～～ ﾃｯｾﾚｰﾀ ～～～ (上下の間の処理)

	m_pDeviceContext11->DSSetShader(NULL, NULL, 0);//ﾄﾞﾒｲﾝｼｪｰﾀﾞ.
	m_pDeviceContext11->GSSetShader(NULL, NULL, 0);//ｼﾞｵﾒﾄﾘｼｪｰﾀﾞ.

	// ～～～ ﾗｽﾀﾗｲｻﾞ ～～～ (上下の間の処理)

	m_pDeviceContext11->PSSetShader(NULL, NULL, 0);//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
}

//ﾃｸｽﾁｬ読込＆作成.
HRESULT clsParticle::InitTexture(char *pFileName)
{
	//ﾃｸｽﾁｬ用ｻﾝﾌﾟﾗｰ作成.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc,
		sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if (FAILED(m_pDevice11->CreateSamplerState(
		&SamDesc, &m_pSampleLinear)))
	{
		MessageBox(NULL,
			"ｻﾝﾌﾟﾗ作成失敗",
			"clsParticle::InitTexture()", MB_OK);
		return E_FAIL;
	}

	//ﾃｸｽﾁｬの読込.
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(
		m_pDevice11, pFileName, NULL, NULL,
		&m_pTexture, NULL)))
	{
		MessageBox(NULL,
			pFileName, "ﾃｸｽﾁｬ読込失敗(clsParticle)", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//ﾊﾟｰﾃｨｸﾙの移動.
void clsParticle::Run()
{
	m_Frame++;	//ﾌﾚｰﾑｶｳﾝﾄUP.

	//ﾊﾟｰﾃｨｸﾙ数繰り返し.
	for (int i = 0; i < m_MaxParticle; i++)
	{
		//ﾌﾚｰﾑｶｳﾝﾄが開始ﾌﾚｰﾑ(ﾗﾝﾀﾞﾑ)より大きい.
		if (m_Frame > m_pParticleArray[i].iBirthFrame)
		{
			//方向×速度で移動する値を算出.
			m_pParticleArray[i].vPos
				+= m_pParticleArray[i].vDir
				* m_pParticleArray[i].fSpeed;
			////重力.
			//m_pParticleArray[i].vDir
			//	+= D3DXVECTOR3( 0.0f, -0.00098f, 0.0f );
			////地面でのﾊﾞｳﾝﾄﾞ.
			//if( m_pParticleArray[i].vPos.y < 0.0f )
			//{
			//	m_pParticleArray[i].vDir.y
			//		= -m_pParticleArray[i].vDir.y;
			//}

			//一定の高さまで上昇したか？.
			if (m_pParticleArray[i].vPos.y > 3.0f)
			{
				m_pParticleArray[i].vPos.y
					= ((float)rand() / (float)RAND_MAX) - 1.0f;//-1.0～0.0
				m_pParticleArray[i].vPos.x
					= (2.0f*((float)rand() / (float)RAND_MAX) - 1.0f) / 100.0f;//-0.01～0.01
			}
		}
	}
}