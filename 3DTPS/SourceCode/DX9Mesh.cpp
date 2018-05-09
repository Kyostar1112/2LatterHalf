#include "DX9Mesh.h"
#include "MyMacro.h"

//�����̧�ٖ�(�ިڸ�؂��܂�)
const char SHADER_NAME[] = "Shader\\Mesh.hlsl";

//================================================
//	�ݽ�׸�.
//================================================
clsDX9Mesh::clsDX9Mesh()
{
	//ZeroMemory( this, sizeof( clsDX9Mesh ) );
	m_fScale = 1.0f;
}

//================================================
//	�޽�׸�.
//================================================
clsDX9Mesh::~clsDX9Mesh()
{
	Release();

	//��޼ު�Ă��ذ�.
	SAFE_RELEASE(m_pDevice9);
	SAFE_RELEASE(m_pD3d);
}

//================================================
//	������.
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
//	Dx9������.
//================================================
HRESULT clsDX9Mesh::InitDx9(HWND hWnd)
{
	m_hWnd = hWnd;

	//�uDirect3D�v��޼ު�Ă̍쐬.
	m_pD3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3d == NULL) {
		MessageBox(NULL,
			"Dx9��޼ު�č쐬���s", "InitDx9", MB_OK);
		return E_FAIL;
	}

	//Direct3D���޲� ��޼ު�Ă̍쐬.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;		//�ޯ��ޯ̧��̫�ϯ�(��̫��)
	d3dpp.BackBufferCount = 1;					//�ޯ��ޯ̧�̐�.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//�ܯ�ߴ̪��(��̫��)
	d3dpp.Windowed = true;					//����޳Ӱ��.
	d3dpp.EnableAutoDepthStencil = true;					//��ݼٗL��.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			//��ݼق�̫�ϯ�(16�ޯ�)

	//���޲��쐬(HALӰ��:�`��ƒ��_������GPU�ōs��)
	if (FAILED(m_pD3d->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pDevice9)))
	{
		//���޲��쐬(HALӰ��:�`���GPU�A���_������CPU�ōs��)
		if (FAILED(m_pD3d->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pDevice9)))
		{
			MessageBox(NULL,
				"HALӰ�ނ����޲����쐬�ł��܂���\nREFӰ�ނōĎ��s���܂�",
				"�x��", MB_OK);
			//���޲��쐬(REFӰ��:�`���CPU�A���_������GPU�ōs��)
			if (FAILED(m_pD3d->CreateDevice(
				D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &m_pDevice9)))
			{
				//���޲��쐬(REFӰ��:�`��ƒ��_������CPU�ōs��)
				if (FAILED(m_pD3d->CreateDevice(
					D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &m_pDevice9)))
				{
					MessageBox(NULL,
						"Direct3D���޲��쐬���s", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}
	return S_OK;
}

//================================================
//X̧�ق���ү����۰�ނ���.
//================================================
HRESULT clsDX9Mesh::LoadXMesh(LPSTR fileName)
{
	//��ر��ޯ̧.
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;

	//ڲ��ү���̔���̂��߂ɒǉ�.
	if (FAILED(D3DXLoadMeshFromXA(
		fileName,			//̧�ٖ�.
		D3DXMESH_SYSTEMMEM,	//������؂ɓǍ�.
		m_pDevice9, NULL,
		&pD3DXMtrlBuffer,	//(out)��رُ��.
		NULL,
		&m_dwNumMaterials,	//(out)��رِ�.
		&m_pMeshForRay)))	//(out)ү����޼ު��.
	{
		MessageBox(NULL, fileName,
			"X̧�ٓǍ����s", MB_OK);
		return E_FAIL;
	}

	//X̧�ق�۰��.
	if (FAILED(D3DXLoadMeshFromXA(
		fileName,			//̧�ٖ�.
		D3DXMESH_SYSTEMMEM	//������؂ɓǍ�.
		| D3DXMESH_32BIT,
		m_pDevice9, NULL,
		&pD3DXMtrlBuffer,	//(out)��رُ��.
		NULL,
		&m_dwNumMaterials,	//(out)��رِ�.
		&m_pMesh)))		//(out)ү����޼ު��.
	{
		MessageBox(NULL, fileName,
			"X̧�ٓǍ����s", MB_OK);
		return E_FAIL;
	}

	D3D11_BUFFER_DESC		bd;	//Dx11�ޯ̧�\����.
	D3D11_SUBRESOURCE_DATA	InitData;//�������ް�.

	//�Ǎ��񂾏�񂩂�K�v�ȏ��𔲂��o��.
	D3DXMATERIAL* d3dxMaterials
		= (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	//��رِ����̗̈���l��.
	m_pMaterials = new MY_MATERIAL[m_dwNumMaterials];
	m_ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterials];
	//��رِ����J��Ԃ�.
	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		//���̃^�C�~���O�ŃC���f�b�N�X�o�b�t�@��������.
		m_ppIndexBuffer[i] = NULL;//�������NULL�`�F�b�N�ɑΉ����邽��.

		//��رُ��̺�߰.
		//���޴��.
		m_pMaterials[i].Ambient.x = d3dxMaterials[i].MatD3D.Ambient.r;
		m_pMaterials[i].Ambient.y = d3dxMaterials[i].MatD3D.Ambient.g;
		m_pMaterials[i].Ambient.z = d3dxMaterials[i].MatD3D.Ambient.b;
		m_pMaterials[i].Ambient.w = d3dxMaterials[i].MatD3D.Ambient.a;
		//�ި̭���.
		m_pMaterials[i].Diffuse.x = d3dxMaterials[i].MatD3D.Diffuse.r;
		m_pMaterials[i].Diffuse.y = d3dxMaterials[i].MatD3D.Diffuse.g;
		m_pMaterials[i].Diffuse.z = d3dxMaterials[i].MatD3D.Diffuse.b;
		m_pMaterials[i].Diffuse.w = d3dxMaterials[i].MatD3D.Diffuse.a;
		//��߷��.
		m_pMaterials[i].Specular.x = d3dxMaterials[i].MatD3D.Specular.r;
		m_pMaterials[i].Specular.y = d3dxMaterials[i].MatD3D.Specular.g;
		m_pMaterials[i].Specular.z = d3dxMaterials[i].MatD3D.Specular.b;
		m_pMaterials[i].Specular.w = d3dxMaterials[i].MatD3D.Specular.a;

		//(���̖ʂ�)ø������\���Ă��邩�H.
		if (d3dxMaterials[i].pTextureFilename != NULL
			&& lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			m_bTexture = true;//ø����׸ނ𗧂Ă�.

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
			//�߽��ø���̧�ٖ���A��.
			strcat_s(path, sizeof(path),
				d3dxMaterials[i].pTextureFilename);

			//ø���̧�ٖ����߰.
			strcpy_s(m_pMaterials[i].szTextureName,
				sizeof(m_pMaterials[i].szTextureName), path);

			//ø����쐬.
			if (FAILED(
				D3DX11CreateShaderResourceViewFromFileA(
					m_pDevice11,
					m_pMaterials[i].szTextureName,//ø���̧�ٖ�.
					NULL, NULL,
					&m_pMaterials[i].pTexture,//(out)ø�����޼ު��.
					NULL)))
			{
				MessageBox(NULL, m_pMaterials[i].szTextureName,
					"ø����쐬���s", MB_OK);
				return E_FAIL;
			}
		}
	}

	//--------------------------------------------
	//	���ޯ���ޯ̧���쐬.
	//--------------------------------------------
	//ү���̑������𓾂�.
	//�������Ų��ޯ���ޯ̧����ׂ�����رق��Ƃ̲��ޯ���ޯ̧�𕪗��ł���.
	D3DXATTRIBUTERANGE* pAttrTable = NULL;

	//ү���̖ʂ���ђ��_�̏��ԕύX�𐧌䂵�A��̫��ݽ���œK������.
	// D3DXMESHOPT_COMPACT :�ʂ̏��Ԃ�ύX���A
	//						�g�p����Ă��Ȃ����_�Ɩʂ��폜����.
	// D3DXMESHOPT_ATTRSORT:��̫��ݽ���グ��ׁA
	//						�ʂ̏��Ԃ�ύX���čœK�����s��.
	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT,
		0, 0, 0, 0);
	//����ð��ق̎擾.
	m_pMesh->GetAttributeTable(NULL, &m_NumAttr);
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if (FAILED(
		m_pMesh->GetAttributeTable(pAttrTable, &m_NumAttr)))
	{
		MessageBox(NULL, "����ð��َ擾���s",
			"LoadXMesh", MB_OK);
		return E_FAIL;
	}

	//�������Aۯ����Ȃ��Ǝ��o���Ȃ�.
	int* pIndex = NULL;
	m_pMesh->LockIndexBuffer(
		D3DLOCK_READONLY, (void**)&pIndex);

	//�������Ƃ̲��ޯ���ޯ̧���쐬.
	for (DWORD i = 0; i < m_NumAttr; i++)
	{
		m_AttrID[i] = pAttrTable[i].AttribId;

		//Dx9�̲��ޯ���ޯ̧����̏���Dx11�̲��ޯ���ޯ̧���쐬.
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth
			= sizeof(int) * pAttrTable[i].FaceCount
			* 3; //�ʐ��~3�Œ��_��.
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		//�傫�����ޯ���ޯ̧���̵̾��(�~3����)
		InitData.pSysMem = &pIndex[pAttrTable[i].FaceStart * 3];

		if (FAILED(
			m_pDevice11->CreateBuffer(
				&bd, &InitData, &m_ppIndexBuffer[i])))
		{
			MessageBox(NULL, "���ޯ���ޯ̧�쐬���s",
				"LoadXMesh()", MB_OK);
			return E_FAIL;
		}
		//�ʂ̐����߰.
		m_pMaterials[m_AttrID[i]].dwNumFace
			= pAttrTable[i].FaceCount;
	}
	delete[] pAttrTable;//����ð��ق̍폜.

	m_pMesh->UnlockIndexBuffer();

	//�s�v�ɂȂ�����ر��ޯ̧�����.
	SAFE_RELEASE(pD3DXMtrlBuffer);

	//-----------------------------------------------
	//	���_(�ްï��)�ޯ̧�̍쐬.
	//-----------------------------------------------
	//Dx9�̏ꍇ�Amap�ł͂Ȃ�ۯ����ްï���ޯ̧�����ް������o��.
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	MeshVertex* pVertex = NULL;
	if (SUCCEEDED(
		pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertex = (MeshVertex*)pVertices;
		//Dx9���ްï���ޯ̧������ŁADx11���ްï���ޯ̧���쐬.
		bd.Usage = D3D11_USAGE_DEFAULT;
		//���_���i�[����̂ɕK�v���޲Đ�.
		bd.ByteWidth =
			m_pMesh->GetNumBytesPerVertex()
			* m_pMesh->GetNumVertices(); //���_��.
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pVertex;

		//ø������W��ϲŽ�΍�.
		if (m_bTexture) {
			//��ø���������Ώ�������.
			//���_�����J��Ԃ�.
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
			MessageBox(NULL, "���_(�ްï��)�ޯ̧�쐬���s",
				"LoadXMesh()", MB_OK);
			return E_FAIL;
		}
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);//�ްï���ޯ̧���.

	//-----------------------------------------------
	//	�����-�쐬.
	//-----------------------------------------------
	//ø����p�̻���װ�\����.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	//�Ʊ̨��-(���`���)
	//	POINT:���������e��.
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//ׯ��ݸ�Ӱ��.
	//	WRAP  :�J��Ԃ�
	//	MIRROR:���]�J��Ԃ�.
	//	CLAMP :�[�̖͗l�������L�΂�.
	//	BORDER:�ʓr���E�F�����߂�.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//����װ�쐬.
	if (FAILED(
		m_pDevice11->CreateSamplerState(
			&SamDesc, &m_pSampleLinear)))//(out)����װ.
	{
		MessageBox(NULL, "����׍쐬���s", "�װ", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//======================================
//	HLSĻ�ق�ǂݍ��ݼ���ނ��쐬����.
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

	//HLSL�����ްï������ނ�����ނ��쐬.
	if (m_bTexture) {
		if (FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, NULL, NULL, "VS_Main", "vs_5_0",
				uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "hlsl�Ǎ����s(VS_Main)", "�װ", MB_OK);
			return E_FAIL;
		}
	}
	else {
		if (FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, NULL, NULL, "VS_NoTex", "vs_5_0",
				uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "hlsl�Ǎ����s(VS_NoTex)", "�װ", MB_OK);
			return E_FAIL;
		}
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬��������ނ���u�ްï������ށv���쐬.
	if (FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pVertexShader)))//(out)�ްï�������.
	{
		MessageBox(NULL, "vs�쐬���s", "�װ", MB_OK);
		return E_FAIL;
	}

	//���_���߯�ڲ��Ă��`.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	//���_���߯�ڲ��Ă̔z��v�f��.
	UINT numElements = 0;
	if (m_bTexture)
	{
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);//�v�f���Z�o.
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
		numElements = sizeof(tmp) / sizeof(tmp[0]);//�v�f���Z�o.
		memcpy_s(layout, sizeof(layout),
			tmp, sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements);
	}

	//���_���߯�ڲ��Ă̍쐬.
	if (FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout)))//(out)���_���߯�ڲ���.
	{
		MessageBox(NULL, "���_���߯�ڲ��č쐬���s", "�װ", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//HLSL�����߸�ټ���ނ�����ނ��쐬.
	if (m_bTexture) {
		if (FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, NULL, NULL, "PS_Main", "ps_5_0",
				uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "hlsl�Ǎ����s(PS_Main)", "�װ", MB_OK);
			return E_FAIL;
		}
	}
	else {
		if (FAILED(
			D3DX11CompileFromFile(
				SHADER_NAME, NULL, NULL, "PS_NoTex", "ps_5_0",
				uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			MessageBox(NULL, "hlsl�Ǎ����s(PS_NoTex)", "�װ", MB_OK);
			return E_FAIL;
		}
	}
	SAFE_RELEASE(pErrors);

	//��L�ō쐬��������ނ���u�߸�ټ���ށv���쐬.
	if (FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL,
			&m_pPixelShader)))//(out)�߸�ټ����.
	{
		MessageBox(NULL, "ps�쐬���s", "�װ", MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);//����މ��.

	//�ݽ���(�萔)�ޯ̧�쐬 ����ނɓ���̐��l�𑗂��ޯ̧.
	//�����ł͕ϊ��s��n���p.
	//����ނ�World,View,Projection�s���n��.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�ݽ����ޯ̧���w��.
	cb.ByteWidth = sizeof(MESHSHADER_CONSTANT_BUFFER_ZERO);//�ݽ����ޯ̧�̻���.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//�������݂ű���.
	cb.MiscFlags = 0;//���̑����׸�(���g�p)
	cb.StructureByteStride = 0;//�\���̻̂���(���g�p)
	cb.Usage = D3D11_USAGE_DYNAMIC;//�g�p���@:���ڏ�������.

	//�ݽ����ޯ̧�쐬.
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			NULL,
			&m_pConstantBuffer0)))
	{
		MessageBox(NULL, "�ݽ����ޯ̧�쐬���s", "�װ", MB_OK);
		return E_FAIL;
	}

	//��رٗp�@�ݽ����ޯ̧.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�ݽ����ޯ̧���w��.
	cb.ByteWidth = sizeof(MESHSHADER_CONSTANT_BUFFER1);//�ݽ����ޯ̧�̻���.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//�������݂ű���.
	cb.MiscFlags = 0;//���̑����׸�(���g�p)
	cb.StructureByteStride = 0;//�\���̻̂���(���g�p)
	cb.Usage = D3D11_USAGE_DYNAMIC;//�g�p���@:���ڏ�������.

	//�ݽ����ޯ̧�쐬.
	if (FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			NULL,
			&m_pConstantBuffer1)))
	{
		MessageBox(NULL, "�ݽ����ޯ̧�쐬���s", "�װ", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//�����ݸޗp(��DX9MESH����Main���łQ���݂���̂Œ���)
void clsDX9Mesh::Render(D3DXMATRIX &mView, D3DXMATRIX &mProj,
	D3DXVECTOR3 &vLight, D3DXVECTOR3 &vEye,
	D3DXVECTOR4 vColor, bool alphaFlg)
{
	D3DXMATRIX	mWorld;	//ܰ��ލs��.
	D3DXMATRIX	mScale, mYaw, mPitch, mRoll, mTrans;

	//ܰ��ޕϊ�(�\���ʒu��ݒ肷��)
	//�g�k.
	D3DXMatrixScaling(&mScale,
		m_fScale, m_fScale, m_fScale);
	//��].
	D3DXMatrixRotationY(&mYaw, m_fYaw);		//Y����].
	D3DXMatrixRotationX(&mPitch, m_fPitch);	//X����].
	D3DXMatrixRotationZ(&mRoll, m_fRoll);		//Z����].

#if 0	//UpdatePos()�֐��ֈڐA.
	//============================================
	//��׸��ړ�����(׼޺�)

	//Z���޸�ق�p��.
	D3DXVECTOR3	vecAxisZ(0.0f, 0.0f, 1.0f);

	//Z���޸�ق��̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord(
		&vecAxisZ,	//(out)
		&vecAxisZ,
		&mYaw);	//Y����]�s��.

	//�����ɂ���Đi�s������l��ݒ�.
	switch (m_enDir) {
	case enDirection_Foward:	//�O�i.
		//�����������~�i�߂�l(0.1f)
		m_vPos += vecAxisZ * 0.1f;
		break;
	case enDirection_Backward:	//���.
		m_vPos -= vecAxisZ * 0.1f;
		break;
	}
	m_enDir = enDirection_Stop;	//��~.
	//============================================
#endif

	//�ړ�.
	D3DXMatrixTranslation(&mTrans,
		m_vPos.x, m_vPos.y, m_vPos.z);

	//����(�g�k�~��]�~�ړ�)
	mWorld = mScale * mYaw * mPitch * mRoll * mTrans;

	//�g�p���鼪��ނ��.
	m_pDeviceContext11->VSSetShader(
		m_pVertexShader, NULL, 0);	//���_�����.
	m_pDeviceContext11->PSSetShader(
		m_pPixelShader, NULL, 0);	//�߸�ټ����.

	//����ނ̺ݽ����ޯ̧�Ɋe���ް���n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	//�ޯ̧�����ް��̏��������J�n����Map.
	if (SUCCEEDED(
		m_pDeviceContext11->Map(
			m_pConstantBuffer0, 0,
			D3D11_MAP_WRITE_DISCARD,
			0, &pData)))
	{
		MESHSHADER_CONSTANT_BUFFER_ZERO cb;//�ݽ����ޯ̧.

		//ܰ��ލs���n��.
		cb.mW = mWorld;
		D3DXMatrixTranspose(&cb.mW, &cb.mW);

		//ܰ���,�ޭ�(���),��ۼު����(�ˉe)�s���n��.
		D3DXMATRIX m = mWorld * mView * mProj;
		D3DXMatrixTranspose(&m, &m);//�s���]�u����.
		//���s��̌v�Z���@��DirectX��GPU�ňقȂ邽��.

		cb.mWVP = m;

		//ײĕ�����n��.
		cb.vLightDir = (D3DXVECTOR4)vLight;

		//��ׂ̈ʒu(���_)�𼪰�ނɓn��.
		cb.vEye = D3DXVECTOR4(
			vEye.x, vEye.y, vEye.z, 0.0f);

		//�F.
		cb.vColor = vColor;

		memcpy_s(
			pData.pData,	//��߰����ޯ̧.
			pData.RowPitch,	//��߰����ޯ̧����.
			(void*)(&cb),	//��߰�����ޯ̧.
			sizeof(cb));	//��߰���黲��.

		//�ޯ̧�����ް��̏��������I������Unmap.
		m_pDeviceContext11->Unmap(m_pConstantBuffer0, 0);
	}

	//�ݽ����ޯ̧���ǂ̼���ނŎg�����H.
	m_pDeviceContext11->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer0);//���_�����.
	m_pDeviceContext11->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer0);//�߸�ټ����.

	//���_���߯�ڲ��Ă��.
	m_pDeviceContext11->IASetInputLayout(m_pVertexLayout);

	//����è�ށE���ۼް���.
	m_pDeviceContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//���_�ޯ̧���.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer,
		&stride, &offset);

	//��̧�����ޗp�����޽ðč쐬.
	SetBlend(alphaFlg);

	//�����̐������A���ꂼ��̑����̲��ޯ���ޯ̧��`��.
	for (DWORD i = 0; i < m_NumAttr; i++)
	{
		//�g�p����Ă��Ȃ���رّ΍�.
		if (m_pMaterials[m_AttrID[i]].dwNumFace == 0) {
			continue;
		}
		//���ޯ���ޯ̧���.
		m_pDeviceContext11->IASetIndexBuffer(
			m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//��رق̊e�v�f�𼪰�ނɓn��.
		D3D11_MAPPED_SUBRESOURCE pData1;
		if (SUCCEEDED(
			m_pDeviceContext11->Map(
				m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD,
				0, &pData1)))
		{
			MESHSHADER_CONSTANT_BUFFER1 cb;
			//���޴��,�ި̭���,��߷�ׂ𼪰�ނɓn��.
			cb.vAmbient = m_pMaterials[m_AttrID[i]].Ambient;
			cb.vDiffuse = m_pMaterials[m_AttrID[i]].Diffuse;
			cb.vSpecular = m_pMaterials[m_AttrID[i]].Specular;

			memcpy_s(
				pData1.pData, pData1.RowPitch,
				(void*)&cb, sizeof(MESHSHADER_CONSTANT_BUFFER1));

			m_pDeviceContext11->Unmap(m_pConstantBuffer1, 0);
		}

		//���̺ݽ����ޯ̧���ǂ̼���ނŎg�����H.
		m_pDeviceContext11->VSSetConstantBuffers(
			1, 1, &m_pConstantBuffer1);
		m_pDeviceContext11->PSSetConstantBuffers(
			1, 1, &m_pConstantBuffer1);

		//ø����𼪰�ނɓn��.
		if (m_pMaterials[m_AttrID[i]].pTexture) {
			//ø���������Ƃ�.
			m_pDeviceContext11->PSSetSamplers(
				0, 1, &m_pSampleLinear);
			m_pDeviceContext11->PSSetShaderResources(
				0, 1, &m_pMaterials[m_AttrID[i]].pTexture);
		}
		else {
			//ø������Ȃ��Ƃ�.
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			m_pDeviceContext11->PSSetShaderResources(
				0, 1, Nothing);
		}
		//����è��(��غ��)�������ݸ�.
		m_pDeviceContext11->DrawIndexed(
			m_pMaterials[m_AttrID[i]].dwNumFace * 3, 0, 0);
	}
}

//����(��̧������)�ݒ�̐؂�ւ�.
void clsDX9Mesh::SetBlend(bool flg)
{
	//��̧�����ޗp�����޽ðč\����.
	//pnģ�ٓ��ɱ�̧��񂪂���̂ŁA
	//���߂���悤�������޽ðĂ�ݒ肷��.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));//������.

	blendDesc.IndependentBlendEnable
		= false;//false:RenderTarget[0]�����ް�݂̂��g�p����.
	//true :RenderTarget[0�`7]���g�p�ł���.
	//      (���ް���ޯĖ��ɓƗ����������ޏ���)
	blendDesc.AlphaToCoverageEnable
		= false;//true :��̧ĩ���گ�ނ��g�p����.
	blendDesc.RenderTarget[0].BlendEnable
		= flg;	//true :��̧�����ނ��g�p����.
	blendDesc.RenderTarget[0].SrcBlend	//���f�ނɑ΂���ݒ�.
		= D3D11_BLEND_SRC_ALPHA;		//	��̧�����ނ��w��.
	blendDesc.RenderTarget[0].DestBlend	//�d�˂�f�ނɑ΂���ݒ�.
		= D3D11_BLEND_INV_SRC_ALPHA;	//	��̧�����ނ̔��]���w��.

	blendDesc.RenderTarget[0].BlendOp	//�����޵�߼��.
		= D3D11_BLEND_OP_ADD;			//	ADD:���Z����.

	blendDesc.RenderTarget[0].SrcBlendAlpha	//���f�ނ̱�̧�ɑ΂���ݒ�.
		= D3D11_BLEND_ONE;					//	���̂܂܎g�p.
	blendDesc.RenderTarget[0].DestBlendAlpha//�d�˂�f�ނ̱�̧�ɑ΂���ݒ�.
		= D3D11_BLEND_ZERO;					//	�������Ȃ�.

	blendDesc.RenderTarget[0].BlendOpAlpha	//��̧�������޵�߼��.
		= D3D11_BLEND_OP_ADD;				//	ADD:���Z����.

	blendDesc.RenderTarget[0].RenderTargetWriteMask	//�߸�ٖ��̏�������Ͻ�.
		= D3D11_COLOR_WRITE_ENABLE_ALL;				//	�S�Ă̐���(RGBA)�ւ��ް��̊i�[��������.

	//�����޽ðč쐬.
	if (FAILED(
		m_pDevice11->CreateBlendState(
			&blendDesc, &m_pBlendState)))
	{
		MessageBox(NULL, "�����޽ðč쐬���s", "�װ", MB_OK);
	}

	//�����޽ðĂ̐ݒ�.
	UINT mask = 0xffffffff;	//Ͻ��l.
	m_pDeviceContext11->OMSetBlendState(
		m_pBlendState, NULL, mask);
}
#if 1
//�ʒu�X�V�֐��̏�����.
void clsDX9Mesh::InitPos()
{
	D3DXMatrixRotationY(&m_mYaw, m_fYaw);//Y����].

	m_vecAxisZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	//Z���޸�ق��̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord(
		&m_vecAxisZ,	//(out)
		&m_vecAxisZ,
		&m_mYaw);	//Y����]�s��.
}

//�ʒu�X�V�֐�.
D3DXVECTOR3 clsDX9Mesh::UpdatePos(D3DXVECTOR3 oldPos)
{
	//============================================
	//��׸��ړ�����(׼޺�)
	if (!m_bInitPosFlg)
	{
		InitPos();
		m_bInitPosFlg = true;
	}

	D3DXVECTOR3 Pos;
	Pos = oldPos;

	//�����ɂ���Đi�s������l��ݒ�.
	switch (m_enDir) {
	case enDirection_Foward:	//�O�i.
		//�����������~�i�߂�l(0.1f)
		Pos += m_vecAxisZ * 0.1f;
		break;
	case enDirection_Backward:	//���.
		Pos -= m_vecAxisZ * 0.1f;
		break;
	}
	m_enDir = enDirection_Stop;	//��~.
	m_fYaw = 0.0f;
	return Pos;
	//============================================
}

#endif // 1

//����֐�.
HRESULT clsDX9Mesh::Release()
{
	SAFE_RELEASE(m_pDevice11);//���޲���޼ު��.
	SAFE_RELEASE(m_pDeviceContext11);//���޲���÷��.
	SAFE_RELEASE(m_pVertexShader);//���_�����.
	SAFE_RELEASE(m_pVertexLayout);//���_ڲ���.
	SAFE_RELEASE(m_pConstantBuffer0);//�ݽ����ޯ̧.
	SAFE_RELEASE(m_pConstantBuffer1);//�ݽ����ޯ̧.
	SAFE_RELEASE(m_pVertexBuffer);//���_(�ްï��)�ޯ̧.
	SAFE_RELEASE(m_pSampleLinear);//ø����̻����-:ø����[�Ɋe��̨����������.

	SAFE_RELEASE(m_pBlendState);//�����޽ð�.
	SAFE_DELETE_ARRAY(m_pMaterials)//��رٍ\����.

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

	//////�������[.
	//ZeroMemory(this,sizeof(clsDX9Mesh));

	return S_OK;
}