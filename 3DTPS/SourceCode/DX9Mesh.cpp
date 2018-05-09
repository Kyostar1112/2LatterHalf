#include "DX9Mesh.h"
#include "MyMacro.h"

//ｼｪｰﾀﾞﾌｧｲﾙ名(ﾃﾞｨﾚｸﾄﾘも含む)
const char SHADER_NAME[] = "Shader\\Mesh.hlsl";

//================================================
//	ｺﾝｽﾄﾗｸﾀ.
//================================================
clsDX9Mesh::clsDX9Mesh()
{
	//ZeroMemory( this, sizeof( clsDX9Mesh ) );
	m_fScale = 1.0f;
}

//================================================
//	ﾃﾞｽﾄﾗｸﾀ.
//================================================
clsDX9Mesh::~clsDX9Mesh()
{
	Release();

	//ｵﾌﾞｼﾞｪｸﾄのﾘﾘｰｽ.
	SAFE_RELEASE(m_pDevice9);
	SAFE_RELEASE(m_pD3d);
}

//================================================
//	初期化.
//================================================
HRESULT clsDX9Mesh::Init(
	HWND hWnd, ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11, LPSTR fileName)
{
	m_bInitPosFlg = false;
	m_hWnd = hWnd;
	m_pDevice11 = pDevice11;
	m_pDeviceContext11 = pContext11;

	if (FAILED(InitDx9(m_hWnd)))
	{
		return E_FAIL;
	}
	if (FAILED(LoadXMesh(fileName)))
	{
		return E_FAIL;
	}
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//================================================
//	Dx9初期化.
//================================================
HRESULT clsDX9Mesh::InitDx9(HWND hWnd)
{
	m_hWnd = hWnd;

	//「Direct3D」ｵﾌﾞｼﾞｪｸﾄの作成.
	m_pD3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3d == NULL) {
		MessageBox(NULL,
			"Dx9ｵﾌﾞｼﾞｪｸﾄ作成失敗", "InitDx9", MB_OK);
		return E_FAIL;
	}

	//Direct3Dﾃﾞﾊﾞｲｽ ｵﾌﾞｼﾞｪｸﾄの作成.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;		//ﾊﾞｯｸﾊﾞｯﾌｧのﾌｫｰﾏｯﾄ(ﾃﾞﾌｫﾙﾄ)
	d3dpp.BackBufferCount = 1;					//ﾊﾞｯｸﾊﾞｯﾌｧの数.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//ｽﾜｯﾌﾟｴﾌｪｸﾄ(ﾃﾞﾌｫﾙﾄ)
	d3dpp.Windowed = true;					//ｳｨﾝﾄﾞｳﾓｰﾄﾞ.
	d3dpp.EnableAutoDepthStencil = true;					//ｽﾃﾝｼﾙ有効.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			//ｽﾃﾝｼﾙのﾌｫｰﾏｯﾄ(16ﾋﾞｯﾄ)

	//ﾃﾞﾊﾞｲｽ作成(HALﾓｰﾄﾞ:描画と頂点処理をGPUで行う)
	if (FAILED(m_pD3d->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pDevice9)))
	{
		//ﾃﾞﾊﾞｲｽ作成(HALﾓｰﾄﾞ:描画はGPU、頂点処理はCPUで行う)
		if (FAILED(m_pD3d->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pDevice9)))
		{
			MessageBox(NULL,
				"HALﾓｰﾄﾞでﾃﾞﾊﾞｲｽを作成できません\nREFﾓｰﾄﾞで再試行します",
				"警告", MB_OK);
			//ﾃﾞﾊﾞｲｽ作成(REFﾓｰﾄﾞ:描画はCPU、頂点処理はGPUで行う)
			if (FAILED(m_pD3d->CreateDevice(
				D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &m_pDevice9)))
			{
				//ﾃﾞﾊﾞｲｽ作成(REFﾓｰﾄﾞ:描画と頂点処理をCPUで行う)
				if (FAILED(m_pD3d->CreateDevice(
					D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &m_pDevice9)))
				{
					MessageBox(NULL,
						"Direct3Dﾃﾞﾊﾞｲｽ作成失敗", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}
	return S_OK;
}

//================================================
//Xﾌｧｲﾙからﾒｯｼｭをﾛｰﾄﾞする.
//================================================
HRESULT clsDX9Mesh::LoadXMesh(LPSTR fileName)
{
	//ﾏﾃﾘｱﾙﾊﾞｯﾌｧ.
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	//ﾚｲとﾒｯｼｭの判定のために追加.
	if (FAILED(D3DXLoadMeshFromXA(
		fileName,			//ﾌｧｲﾙ名.
		D3DXMESH_SYSTEMMEM,	//ｼｽﾃﾑﾒﾓﾘに読込.
		m_pDevice9, NULL,
		&pD3DXMtrlBuffer,	//(out)ﾏﾃﾘｱﾙ情報.
		NULL,
		&m_dwNumMaterials,	//(out)ﾏﾃﾘｱﾙ数.
		&m_pMeshForRay)))	//(out)ﾒｯｼｭｵﾌﾞｼﾞｪｸﾄ.
	{
		MessageBox(NULL, fileName,
			"Xﾌｧｲﾙ読込失敗", MB_OK);
		return E_FAIL;
	}

	//Xﾌｧｲﾙのﾛｰﾄﾞ.
	if (FAILED(D3DXLoadMeshFromXA(
		fileName,			//ﾌｧｲﾙ名.
		D3DXMESH_SYSTEMMEM	//ｼｽﾃﾑﾒﾓﾘに読込.
		| D3DXMESH_32BIT,
		m_pDevice9, NULL,
		&pD3DXMtrlBuffer,	//(out)ﾏﾃﾘｱﾙ情報.
		NULL,
		&m_dwNumMaterials,	//(out)ﾏﾃﾘｱﾙ数.
		&m_pMesh)))		//(out)ﾒｯｼｭｵﾌﾞｼﾞｪｸﾄ.
	{
		MessageBox(NULL, fileName,
			"Xﾌｧｲﾙ読込失敗", MB_OK);
		return E_FAIL;
	}

	D3D11_BUFFER_DESC		bd;	//Dx11ﾊﾞｯﾌｧ構造体.
	D3D11_SUBRESOURCE_DATA	InitData;//初期化ﾃﾞｰﾀ.

	//読込んだ情報から必要な情報を抜き出す.
	D3DXMATERIAL* d3dxMaterials
		= (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	//ﾏﾃﾘｱﾙ数分の領域を獲得.
	m_pMaterials = new MY_MATERIAL[m_dwNumMaterials];
	m_ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterials];
	//ﾏﾃﾘｱﾙ数分繰り返し.
	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		//このタイミングでインデックスバッファを初期化.
		m_ppIndexBuffer[i] = NULL;//解放時のNULLチェックに対応するため.

		//ﾏﾃﾘｱﾙ情報のｺﾋﾟｰ.
		//ｱﾝﾋﾞｴﾝﾄ.
		m_pMaterials[i].Ambient.x = d3dxMaterials[i].MatD3D.Ambient.r;
		m_pMaterials[i].Ambient.y = d3dxMaterials[i].MatD3D.Ambient.g;
		m_pMaterials[i].Ambient.z = d3dxMaterials[i].MatD3D.Ambient.b;
		m_pMaterials[i].Ambient.w = d3dxMaterials[i].MatD3D.Ambient.a;
		//ﾃﾞｨﾌｭｰｽﾞ.
		m_pMaterials[i].Diffuse.x = d3dxMaterials[i].MatD3D.Diffuse.r;
		m_pMaterials[i].Diffuse.y = d3dxMaterials[i].MatD3D.Diffuse.g;
		m_pMaterials[i].Diffuse.z = d3dxMaterials[i].MatD3D.Diffuse.b;
		m_pMaterials[i].Diffuse.w = d3dxMaterials[i].MatD3D.Diffuse.a;
		//ｽﾍﾟｷｭﾗ.
		m_pMaterials[i].Specular.x = d3dxMaterials[i].MatD3D.Specular.r;
		m_pMaterials[i].Specular.y = d3dxMaterials[i].MatD3D.Specular.g;
		m_pMaterials[i].Specular.z = d3dxMaterials[i].MatD3D.Specular.b;
		m_pMaterials[i].Specular.w = d3dxMaterials[i].MatD3D.Specular.a;

		//(その面に)ﾃｸｽﾁｬが貼られているか？.
		if (d3dxMaterials[i].pTextureFilename != NULL
			&& lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			m_bTexture = true;//ﾃｸｽﾁｬﾌﾗｸﾞを立てる.

			char path[64] = "";
			int  path_count = lstrlen(fileName);

			for (int k = path_count; k >= 0; k--) {
				if (fileName[k] == '\\') {
					for (int j = 0; j <= k; j++) {
						path[j] = fileName[j];
					}
					path[k + 1] = '\0';
					break;
				}
			}
			//ﾊﾟｽとﾃｸｽﾁｬﾌｧｲﾙ名を連結.
			strcat_s(path, sizeof(path),
				d3dxMaterials[i].pTextureFilename);

			//ﾃｸｽﾁｬﾌｧｲﾙ名をｺﾋﾟｰ.
			strcpy_s(m_pMaterials[i].szTextureName,
				sizeof(m_pMaterials[i].szTextureName), path);

			//ﾃｸｽﾁｬ作成.
			if (FAILED(
				D3DX11CreateShaderResourceViewFromFileA(
					m_pDevice11,
					m_pMaterials[i].szTextureName,//ﾃｸｽﾁｬﾌｧｲﾙ名.
					NULL, NULL,
					&m_pMaterials[i].pTexture,//(out)ﾃｸｽﾁｬｵﾌﾞｼﾞｪｸﾄ.
					NULL)))
			{
				MessageBox(NULL, m_pMaterials[i].szTextureName,
					"ﾃｸｽﾁｬ作成失敗", MB_OK);
				return E_FAIL;
			}
		}
	}

	//--------------------------------------------
	//	ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧを作成.
	//--------------------------------------------
	//ﾒｯｼｭの属性情報を得る.
	//属性情報でｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧから細かいﾏﾃﾘｱﾙごとのｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧを分離できる.
	D3DXATTRIBUTERANGE* pAttrTable = NULL;

	//ﾒｯｼｭの面および頂点の順番変更を制御し、ﾊﾟﾌｫｰﾏﾝｽを最適化する.
	// D3DXMESHOPT_COMPACT :面の順番を変更し、
	//						使用されていない頂点と面を削除する.
	// D3DXMESHOPT_ATTRSORT:ﾊﾟﾌｫｰﾏﾝｽを上げる為、
	//						面の順番を変更して最適化を行う.
	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT,
		0, 0, 0, 0);
	//属性ﾃｰﾌﾞﾙの取得.
	m_pMesh->GetAttributeTable(NULL, &m_NumAttr);
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if (FAILED(
		m_pMesh->GetAttributeTable(pAttrTable, &m_NumAttr)))
	{
		MessageBox(NULL, "属性ﾃｰﾌﾞﾙ取得失敗",
			"LoadXMesh", MB_OK);
		return E_FAIL;
	}

	//同じく、ﾛｯｸしないと取り出せない.
	int* pIndex = NULL;
	m_pMesh->LockIndexBuffer(
		D3DLOCK_READONLY, (void**)&pIndex);

	//属性ごとのｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧを作成.
	for (DWORD i = 0; i < m_NumAttr; i++)
	{
		m_AttrID[i] = pAttrTable[i].AttribId;

		//Dx9のｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧからの情報でDx11のｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧを作成.
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth
			= sizeof(int) * pAttrTable[i].FaceCount
			* 3; //面数×3で頂点数.
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		//大きいｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ内のｵﾌｾｯﾄ(×3する)
		InitData.pSysMem = &pIndex[pAttrTable[i].FaceStart * 3];

		if (FAILED(
			m_pDevice11->CreateBuffer(
				&bd, &InitData, &m_ppIndexBuffer[i])))
		{
			MessageBox(NULL, "ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧ作成失敗",
				"LoadXMesh()", MB_OK);
			return E_FAIL;
		}
		//面の数をｺﾋﾟｰ.
		m_pMaterials[m_AttrID[i]].dwNumFace
			= pAttrTable[i].FaceCount;
	}
	delete[] pAttrTable;//属性ﾃｰﾌﾞﾙの削除.

	m_pMesh->UnlockIndexBuffer();

	//不要になったﾏﾃﾘｱﾙﾊﾞｯﾌｧを解放.
	SAFE_RELEASE(pD3DXMtrlBuffer);

	//-----------------------------------------------
	//	頂点(ﾊﾞｰﾃｯｸｽ)ﾊﾞｯﾌｧの作成.
	//-----------------------------------------------
	//Dx9の場合、mapではなくﾛｯｸでﾊﾞｰﾃｯｸｽﾊﾞｯﾌｧからﾃﾞｰﾀを取り出す.
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	MeshVertex* pVertex = NULL;
	if (SUCCEEDED(
		pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertex = (MeshVertex*)pVertices;
		//Dx9のﾊﾞｰﾃｯｸｽﾊﾞｯﾌｧから情報で、Dx11のﾊﾞｰﾃｯｸｽﾊﾞｯﾌｧを作成.
		bd.Usage = D3D11_USAGE_DEFAULT;
		//頂点を格納するのに必要なﾊﾞｲﾄ数.
		bd.ByteWidth =
			m_pMesh->GetNumBytesPerVertex()
			* m_pMesh->GetNumVertices(); //頂点数.
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pVertex;

		//ﾃｸｽﾁｬ座標がﾏｲﾅｽ対策.
		if (m_bTexture) {
			//※ﾃｸｽﾁｬがあれば処理する.
			//頂点数分繰り返し.
			for (int i = 0; i < m_pMesh->GetNumVertices(); i++) {
				if (pVertex[i].vTex.x < 0.0f) {
					pVertex[i].vTex.x += 1.0f;
				}
				if (pVertex[i].vTex.y < 0.0f) {
					pVertex[i].vTex.y += 1.0f;
				}
			}
		}

		if (FAILED(m_pDevice11->CreateBuffer(
			&bd, &InitData, &m_pVertexBuffer)))
		{
			MessageBox(NULL, "頂点(ﾊﾞｰﾃｯｸｽ)ﾊﾞｯﾌｧ作成失敗",
				"LoadXMesh()", MB_OK);
			return E_FAIL;
		}
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);//ﾊﾞｰﾃｯｸｽﾊﾞｯﾌｧ解放.

	//-----------------------------------------------
	//	ｻﾝﾌﾟﾗ-作成.
	//-----------------------------------------------
	//ﾃｸｽﾁｬ用のｻﾝﾌﾟﾗｰ構造体.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	//ﾘﾆｱﾌｨﾙﾀ-(線形補間)
	//	POINT:高速だが粗い.
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//ﾗｯﾋﾟﾝｸﾞﾓｰﾄﾞ.
	//	WRAP  :繰り返し
	//	MIRROR:反転繰り返し.
	//	CLAMP :端の模様を引き伸ばす.
	//	BORDER:別途境界色を決める.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//ｻﾝﾌﾟﾗｰ作成.
	if (FAILED(
		m_pDevice11->CreateSamplerState(
			&SamDesc, &m_pSampleLinear)))//(out)ｻﾝﾌﾟﾗｰ.
	{
		MessageBox(NULL, "ｻﾝﾌﾟﾗ作成失敗", "ｴﾗｰ", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//======================================
//	HLSLﾌｧｲﾙを読み込みｼｪｰﾀﾞを作成する.
//	HLSL:HIGH-LEVEL-SHADER-LANGUAGE.
//======================================
HRESULT clsDX9Mesh::InitShader()
{
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	UINT uCompileFlag = 0;

#ifdef _DEBUG
	uCompileFlag
		= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSLからﾊﾞｰﾃｯｸｽｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (m_bTexture) {
		if (FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, NULL, NULL, "VS_Main", "vs_5_0",
				uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "hlsl読込失敗(VS_Main)", "ｴﾗｰ", MB_OK);
			return E_FAIL;
		}
	}
	else {
		if (FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, NULL, NULL, "VS_NoTex", "vs_5_0",
				uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "hlsl読込失敗(VS_NoTex)", "ｴﾗｰ", MB_OK);
			return E_FAIL;
		}
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
	D3D11_INPUT_ELEMENT_DESC layout[3];
	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄの配列要素数.
	UINT numElements = 0;
	if (m_bTexture)
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);//要素数算出.
		memcpy_s(layout, sizeof(layout),
			tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);//要素数算出.
		memcpy_s(layout, sizeof(layout),
			tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);
	}

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

	//HLSLからﾋﾟｸｾﾙｼｪｰﾀﾞのﾌﾞﾛﾌﾞを作成.
	if (m_bTexture) {
		if (FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, NULL, NULL, "PS_Main", "ps_5_0",
				uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "hlsl読込失敗(PS_Main)", "ｴﾗｰ", MB_OK);
			return E_FAIL;
		}
	}
	else {
		if (FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, NULL, NULL, "PS_NoTex", "ps_5_0",
				uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "hlsl読込失敗(PS_NoTex)", "ｴﾗｰ", MB_OK);
			return E_FAIL;
		}
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
	cb.ByteWidth = sizeof(MESHSHADER_CONSTANT_BUFFER_ZERO);//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｻｲｽﾞ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//書き込みでｱｸｾｽ.
	cb.MiscFlags = 0;//その他のﾌﾗｸﾞ(未使用)
	cb.StructureByteStride = 0;//構造体のｻｲｽﾞ(未使用)
	cb.Usage = D3D11_USAGE_DYNAMIC;//使用方法:直接書き込み.

	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			NULL,
			&m_pConstantBuffer0)))
	{
		MessageBox(NULL, "ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成失敗", "ｴﾗｰ", MB_OK);
		return E_FAIL;
	}

	//ﾏﾃﾘｱﾙ用　ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧを指定.
	cb.ByteWidth = sizeof(MESHSHADER_CONSTANT_BUFFER1);//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧのｻｲｽﾞ.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//書き込みでｱｸｾｽ.
	cb.MiscFlags = 0;//その他のﾌﾗｸﾞ(未使用)
	cb.StructureByteStride = 0;//構造体のｻｲｽﾞ(未使用)
	cb.Usage = D3D11_USAGE_DYNAMIC;//使用方法:直接書き込み.

	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			NULL,
			&m_pConstantBuffer1)))
	{
		MessageBox(NULL, "ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ作成失敗", "ｴﾗｰ", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//ﾚﾝﾀﾞﾘﾝｸﾞ用(※DX9MESH内とMain内で２つ存在するので注意)
void clsDX9Mesh::Render(D3DXMATRIX &mView, D3DXMATRIX &mProj,
	D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye,
	D3DXVECTOR4 vColor, bool alphaFlg)
{
	D3DXMATRIX	mWorld;	//ﾜｰﾙﾄﾞ行列.
	D3DXMATRIX	mScale, mYaw, mPitch, mRoll, mTrans;

	//ﾜｰﾙﾄﾞ変換(表示位置を設定する)
	//拡縮.
	D3DXMatrixScaling(&mScale,
		m_fScale, m_fScale, m_fScale);
	//回転.
	D3DXMatrixRotationY(&mYaw, m_fYaw);		//Y軸回転.
	D3DXMatrixRotationX(&mPitch, m_fPitch);	//X軸回転.
	D3DXMatrixRotationZ(&mRoll, m_fRoll);		//Z軸回転.

#if 0	//UpdatePos()関数へ移植.
	//============================================
	//ｷｬﾗｸﾀ移動処理(ﾗｼﾞｺﾝ)

	//Z軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3	vecAxisZ(0.0f, 0.0f, 1.0f);

	//Z軸ﾍﾞｸﾄﾙそのものを回転状態により変換する.
	D3DXVec3TransformCoord(
		&vecAxisZ,	//(out)
		&vecAxisZ,
		&mYaw);	//Y軸回転行列.

	//方向によって進行させる値を設定.
	switch (m_enDir) {
	case enDirection_Foward:	//前進.
		//向かう方向×進める値(0.1f)
		m_vPos += vecAxisZ * 0.1f;
		break;
	case enDirection_Backward:	//後退.
		m_vPos -= vecAxisZ * 0.1f;
		break;
	}
	m_enDir = enDirection_Stop;	//停止.
	//============================================
#endif

	//移動.
	D3DXMatrixTranslation(&mTrans,
		m_vPos.x, m_vPos.y, m_vPos.z);

	//合成(拡縮×回転×移動)
	mWorld = mScale * mYaw * mPitch * mRoll * mTrans;

	//使用するｼｪｰﾀﾞをｾｯﾄ.
	m_pDeviceContext11->VSSetShader(
		m_pVertexShader, NULL, 0);	//頂点ｼｪｰﾀﾞ.
	m_pDeviceContext11->PSSetShader(
		m_pPixelShader, NULL, 0);	//ﾋﾟｸｾﾙｼｪｰﾀﾞ.

	//ｼｪｰﾀﾞのｺﾝｽﾀﾝﾄﾊﾞｯﾌｧに各種ﾃﾞｰﾀを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	//ﾊﾞｯﾌｧ内のﾃﾞｰﾀの書き換え開始時にMap.
	if (SUCCEEDED(
		m_pDeviceContext11->Map(
			m_pConstantBuffer0, 0,
			D3D11_MAP_WRITE_DISCARD,
			0, &pData)))
	{
		MESHSHADER_CONSTANT_BUFFER_ZERO cb;//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.

		//ﾜｰﾙﾄﾞ行列を渡す.
		cb.mW = mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);

		//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ(ｶﾒﾗ),ﾌﾟﾛｼﾞｪｸｼｮﾝ(射影)行列を渡す.
		D3DXMATRIX m = mWorld * mView * mProj;
		D3DXMatrixTranspose(&m, &m);//行列を転置する.
		//※行列の計算方法がDirectXとGPUで異なるため.

		cb.mWVP = m;

		//ﾗｲﾄ方向を渡す.
		cb.vLightDir = (D3DXVECTOR4)vLight;

		//ｶﾒﾗの位置(視点)をｼｪｰﾀﾞに渡す.
		cb.vEye = D3DXVECTOR4(
			vEye.x, vEye.y, vEye.z, 0.0f);

		//色.
		cb.vColor = vColor;

		memcpy_s(
			pData.pData,	//ｺﾋﾟｰ先のﾊﾞｯﾌｧ.
			pData.RowPitch,	//ｺﾋﾟｰ先のﾊﾞｯﾌｧｻｲｽﾞ.
			(void*)(&cb),	//ｺﾋﾟｰ元のﾊﾞｯﾌｧ.
			sizeof(cb));	//ｺﾋﾟｰするｻｲｽﾞ.

		//ﾊﾞｯﾌｧ内のﾃﾞｰﾀの書き換え終了時にUnmap.
		m_pDeviceContext11->Unmap(m_pConstantBuffer0, 0);
	}

	//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧをどのｼｪｰﾀﾞで使うか？.
	m_pDeviceContext11->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer0);//頂点ｼｪｰﾀﾞ.
	m_pDeviceContext11->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer0);//ﾋﾟｸｾﾙｼｪｰﾀﾞ.

	//頂点ｲﾝﾌﾟｯﾄﾚｲｱｳﾄをｾｯﾄ.
	m_pDeviceContext11->IASetInputLayout(m_pVertexLayout);

	//ﾌﾟﾘﾐﾃｨﾌﾞ・ﾄﾎﾟﾛｼﾞｰをｾｯﾄ.
	m_pDeviceContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//頂点ﾊﾞｯﾌｧをｾｯﾄ.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer,
		&stride, &offset);

	//ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ用ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ作成.
	SetBlend(alphaFlg);

	//属性の数だけ、それぞれの属性のｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧを描画.
	for (DWORD i = 0; i < m_NumAttr; i++)
	{
		//使用されていないﾏﾃﾘｱﾙ対策.
		if (m_pMaterials[m_AttrID[i]].dwNumFace == 0) {
			continue;
		}
		//ｲﾝﾃﾞｯｸｽﾊﾞｯﾌｧをｾｯﾄ.
		m_pDeviceContext11->IASetIndexBuffer(
			m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//ﾏﾃﾘｱﾙの各要素をｼｪｰﾀﾞに渡す.
		D3D11_MAPPED_SUBRESOURCE pData1;
		if (SUCCEEDED(
			m_pDeviceContext11->Map(
				m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD,
				0, &pData1)))
		{
			MESHSHADER_CONSTANT_BUFFER1 cb;
			//ｱﾝﾋﾞｴﾝﾄ,ﾃﾞｨﾌｭｰｽﾞ,ｽﾍﾟｷｭﾗをｼｪｰﾀﾞに渡す.
			cb.vAmbient = m_pMaterials[m_AttrID[i]].Ambient;
			cb.vDiffuse = m_pMaterials[m_AttrID[i]].Diffuse;
			cb.vSpecular = m_pMaterials[m_AttrID[i]].Specular;

			memcpy_s(
				pData1.pData, pData1.RowPitch,
				(void*)&cb, sizeof(MESHSHADER_CONSTANT_BUFFER1));

			m_pDeviceContext11->Unmap(m_pConstantBuffer1, 0);
		}

		//このｺﾝｽﾀﾝﾄﾊﾞｯﾌｧをどのｼｪｰﾀﾞで使うか？.
		m_pDeviceContext11->VSSetConstantBuffers(
			1, 1, &m_pConstantBuffer1);
		m_pDeviceContext11->PSSetConstantBuffers(
			1, 1, &m_pConstantBuffer1);

		//ﾃｸｽﾁｬをｼｪｰﾀﾞに渡す.
		if (m_pMaterials[m_AttrID[i]].pTexture) {
			//ﾃｸｽﾁｬがあるとき.
			m_pDeviceContext11->PSSetSamplers(
				0, 1, &m_pSampleLinear);
			m_pDeviceContext11->PSSetShaderResources(
				0, 1, &m_pMaterials[m_AttrID[i]].pTexture);
		}
		else {
			//ﾃｸｽﾁｬがないとき.
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			m_pDeviceContext11->PSSetShaderResources(
				0, 1, Nothing);
		}
		//ﾌﾟﾘﾐﾃｨﾌﾞ(ﾎﾟﾘｺﾞﾝ)をﾚﾝﾀﾞﾘﾝｸﾞ.
		m_pDeviceContext11->DrawIndexed(
			m_pMaterials[m_AttrID[i]].dwNumFace * 3, 0, 0);
	}
}

//透過(ｱﾙﾌｧﾌﾞﾚﾝﾄﾞ)設定の切り替え.
void clsDX9Mesh::SetBlend(bool flg)
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
#if 1
//位置更新関数の初期化.
void clsDX9Mesh::InitPos()
{
	D3DXMatrixRotationY(&m_mYaw, m_fYaw);//Y軸回転.

	m_vecAxisZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//Z軸ﾍﾞｸﾄﾙそのものを回転状態により変換する.
	D3DXVec3TransformCoord(
		&m_vecAxisZ,	//(out)
		&m_vecAxisZ,
		&m_mYaw);	//Y軸回転行列.
}

//位置更新関数.
D3DXVECTOR3 clsDX9Mesh::UpdatePos(D3DXVECTOR3 oldPos)
{
	//============================================
	//ｷｬﾗｸﾀ移動処理(ﾗｼﾞｺﾝ)
	if (!m_bInitPosFlg)
	{
		InitPos();
		m_bInitPosFlg = true;
	}

	D3DXVECTOR3 Pos;
	Pos = oldPos;

	//方向によって進行させる値を設定.
	switch (m_enDir) {
	case enDirection_Foward:	//前進.
		//向かう方向×進める値(0.1f)
		Pos += m_vecAxisZ * 0.1f;
		break;
	case enDirection_Backward:	//後退.
		Pos -= m_vecAxisZ * 0.1f;
		break;
	}
	m_enDir = enDirection_Stop;	//停止.
	m_fYaw = 0.0f;
	return Pos;
	//============================================
}

#endif // 1

//解放関数.
HRESULT clsDX9Mesh::Release()
{
	SAFE_RELEASE(m_pDevice11);//ﾃﾞﾊﾞｲｽｵﾌﾞｼﾞｪｸﾄ.
	SAFE_RELEASE(m_pDeviceContext11);//ﾃﾞﾊﾞｲｽｺﾝﾃｷｽﾄ.
	SAFE_RELEASE(m_pVertexShader);//頂点ｼｪｰﾀﾞ.
	SAFE_RELEASE(m_pVertexLayout);//頂点ﾚｲｱｳﾄ.
	SAFE_RELEASE(m_pConstantBuffer0);//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
	SAFE_RELEASE(m_pConstantBuffer1);//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
	SAFE_RELEASE(m_pVertexBuffer);//頂点(ﾊﾞｰﾃｯｸｽ)ﾊﾞｯﾌｧ.
	SAFE_RELEASE(m_pSampleLinear);//ﾃｸｽﾁｬのｻﾝﾌﾟﾗ-:ﾃｸｽﾁｬーに各種ﾌｨﾙﾀをかける.

	SAFE_RELEASE(m_pBlendState);//ﾌﾞﾚﾝﾄﾞｽﾃｰﾄ.
	SAFE_DELETE_ARRAY(m_pMaterials)//ﾏﾃﾘｱﾙ構造体.

		if (m_ppIndexBuffer != NULL)
		{
			//for (DWORD i = 0; i < m_dwNumMaterials; i++)
			//{
			//	SAFE_DELETE(m_ppIndexBuffer[i]);
			//}
			SAFE_DELETE_ARRAY(m_ppIndexBuffer);
		}
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMeshForRay);

	//////頭わるわるー.
	//ZeroMemory(this,sizeof(clsDX9Mesh));

	return S_OK;
}