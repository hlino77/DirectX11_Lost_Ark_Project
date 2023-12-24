#include "DamageBox.h"
#include "GameInstance.h"
#include "Target_Manager.h"
#include "LockFree_Transform.h"
#include "ThreadManager.h"
#include "VIBuffer_Point.h"

CDamageBox::CDamageBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"TextBox", -1)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CDamageBox::CDamageBox(const CDamageBox& rhs)
	: CGameObject(rhs)
	, m_pTextInstanceBuffer(rhs.m_pTextInstanceBuffer)
	, m_pTextInstanceShader(rhs.m_pTextInstanceShader)
	, m_pTextInstanceValue(rhs.m_pTextInstanceValue)
	, m_iTextMaxInstanceCount(rhs.m_iTextMaxInstanceCount)
{
}

HRESULT CDamageBox::Initialize_Prototype()
{
	m_iTextMaxInstanceCount = new unordered_map<wstring, _uint>();
	m_pTextInstanceBuffer = new unordered_map<wstring, ID3D11Buffer*>();
	m_pTextInstanceValue = new unordered_map<wstring, tagTypeLess*>();
	m_pTextInstanceShader = new unordered_map<wstring, CShader*>();

	return S_OK;
}

HRESULT CDamageBox::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	TEXTBOXDESC* tTextBoxDesc = static_cast<TEXTBOXDESC*>(pArg);

	m_vSize = tTextBoxDesc->vSize;
	m_strObjectTag = tTextBoxDesc->szTextBoxTag;
	m_bInstance = tTextBoxDesc->m_bInstance;
	m_szModelName = tTextBoxDesc->szTextBoxName;

	if (m_bInstance)
	{
		if (m_pTextInstanceBuffer->find(m_szModelName) != m_pTextInstanceBuffer->end())
		{
			if (FAILED(Ready_Proto_InstanceBuffer()))
				return E_FAIL;
		}
	}
	

	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);


	m_fWinSizeX = ViewportDesc.Width;
	m_fWinSizeY = ViewportDesc.Height;

	m_pTransformCom->Set_Scale(Vec3(m_vSize.x, m_vSize.y, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(0.0f, 0.0f, 0.f));


	if (FAILED(Ready_RenderTarget()))
		return E_FAIL;


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(m_fWinSizeX, m_fWinSizeY, 0.f, 1.f));

	m_vUV = Vec2(1.0f, 1.0f);

	m_bActive = false;

	m_fAlpha = 1.f;

	return S_OK;
}

void CDamageBox::Tick(_float fTimeDelta)
{
}

void CDamageBox::LateTick(_float fTimeDelta)
{
	if (m_bUpdate)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::UPDATE_TEXTBOX, this);
		m_bUpdate = false;
	}
	
	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CDamageBox::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vScaleUV", &m_vUV, sizeof(Vec2))))
		return E_FAIL;


	//Vec4 fColor = Vec4(1.0f, 5.f, 5.f, 1.f);
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &fColor, sizeof(Vec4))))
	//	return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Bind_SRV(m_pShaderCom, m_szTargetTag, "g_TextBoxTexture")))
		return E_FAIL;


	m_pShaderCom->Begin(5);

	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CDamageBox::Render_MakeSRV()
{
	if (m_TextList.empty())
		return S_OK;

	CTarget_Manager::GetInstance()->Begin_MRT(m_pContext, m_szMRTTag);

	for (auto& Text : m_TextList)
	{
		CGameInstance::GetInstance()->DrawFont(Text.second.szFont, Text.second.szText, Text.second.vTextPos, Text.second.vTextColor, Text.second.fRotation, Text.second.vOrigin, Text.second.vTextScale);
	}

	CTarget_Manager::GetInstance()->End_MRT(m_pContext);
	
	return S_OK;
}

HRESULT CDamageBox::Render_Instance(_uint iSize)
{
	if (FAILED((*m_pTextInstanceShader)[m_szModelName]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED((*m_pTextInstanceShader)[m_szModelName]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Bind_SRV((*m_pTextInstanceShader)[m_szModelName], m_szTargetTag, "g_TextBoxTexture")))
		return E_FAIL;



	(*m_pTextInstanceShader)[m_szModelName]->Begin(5);

	m_pInstanceVIBufferCom->Render_Instance((*m_pTextInstanceBuffer)[m_szModelName], iSize, sizeof(VTXINSTANCE_RECT));

	return S_OK;
}

void CDamageBox::Add_InstanceData(_uint iSize, _uint& iIndex)
{
	vector<Vec4>* pInstanceValue = static_cast<vector<Vec4>*>((*m_pTextInstanceValue)[m_szModelName]->GetValue());

	(*pInstanceValue)[iIndex++] = Vec4(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	
	(*pInstanceValue)[iIndex] = Vec4(m_pTransformCom->Get_Scale());

	if ((iSize * 2) - 1 == iIndex)
	{
		ThreadManager::GetInstance()->EnqueueJob([=]()
			{
				Ready_Instance_For_Render(iSize);
			});
	}
	else
		++iIndex;
}


void CDamageBox::Set_Pos(_float fX, _float fY)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(fX - m_fWinSizeX * 0.5f, -fY + m_fWinSizeY * 0.5f, 0.f));
}



void CDamageBox::Set_Text(const wstring& szTextTag, const wstring& szFont, const wstring& szText, Vec2 vTextPos, Vec2 vScale, Vec2 vOrigin, _float fRotation, Vec4 vColor)
{
	TEXTDESC& tText = m_TextList[szTextTag];

	tText.szFont = szFont;
	tText.szText = szText;
	tText.vTextPos = vTextPos;
	tText.vTextScale = vScale;
	tText.vOrigin = vOrigin;
	tText.fRotation = fRotation;
	tText.vTextColor = vColor;

	m_bUpdate = true;
}



HRESULT CDamageBox::Ready_Proto_InstanceBuffer()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Shader_StaticModelInstace"),
		TEXT("Com_InstanceShader"), (CComponent**)&(*m_pTextInstanceShader)[m_szModelName])))
		return E_FAIL;

	(*m_iTextMaxInstanceCount)[m_szModelName] = 500;

	(*m_pTextInstanceValue)[m_szModelName] = new tagTypeLessData<vector<Vec4>>();

	vector<Vec4>* pInstanceValue = static_cast<vector<Vec4>*>((*m_pTextInstanceValue)[m_szModelName]->GetValue());

	pInstanceValue->resize(m_iMaxInstanceCount, Vec4());

	{
		D3D11_BUFFER_DESC			BufferDesc;

		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

		// m_BufferDesc.ByteWidth = 정점하나의 크기(Byte) * 정점의 갯수;
		BufferDesc.ByteWidth = sizeof(VTXINSTANCE_RECT) * (*m_iTextMaxInstanceCount)[m_szModelName];
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* 정적버퍼로 할당한다. (Lock, unLock 호출 불가)*/
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;
		BufferDesc.StructureByteStride = sizeof(VTXINSTANCE_RECT);

		D3D11_SUBRESOURCE_DATA		InitialData;

		InitialData.pSysMem = pInstanceValue->data();

		if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &InitialData, &(*m_pTextInstanceBuffer)[m_szModelName])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDamageBox::Ready_Instance_For_Render(_uint iSize)
{
	vector<Vec4>* pInstanceValue = static_cast<vector<Vec4>*>((*m_pTextInstanceValue)[m_szModelName]->GetValue());

	D3D11_MAPPED_SUBRESOURCE		SubResource = {};

	ID3D11DeviceContext* pContext = CGameInstance::GetInstance()->Get_BeforeRenderContext();

	if (FAILED(pContext->Map((*m_pTextInstanceBuffer)[m_szModelName], 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource)))
		return E_FAIL;

	memcpy(SubResource.pData, pInstanceValue->data(), sizeof(VTXINSTANCE_RECT) * iSize);

	pContext->Unmap((*m_pTextInstanceBuffer)[m_szModelName], 0);

	CGameInstance::GetInstance()->Release_BeforeRenderContext(pContext);

	return S_OK;
}

HRESULT CDamageBox::Ready_Components()
{
	m_pTransformCom = CLockFree_Transform::Create(m_pDevice, m_pContext);

	/* Com_Renderer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_Shader_VtxTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(0, TEXT("Prototype_Component_VIBuffer_Point"),
		TEXT("Com_InstanceVIBuffer"), (CComponent**)&m_pInstanceVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDamageBox::Ready_RenderTarget()
{
	m_szTargetTag = L"Target_" + m_strObjectTag;
	m_szMRTTag = L"MRT_" + m_strObjectTag;

	/* For. Target_TextBox*/
	if (FAILED(CTarget_Manager::GetInstance()->Add_RenderTarget(m_pDevice, m_pContext, m_szTargetTag,
		m_vSize.x, m_vSize.y, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(CTarget_Manager::GetInstance()->Add_MRT(m_szMRTTag, m_szTargetTag)))
		return E_FAIL;


	return S_OK;
}

CDamageBox* CDamageBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDamageBox* pInstance = new CDamageBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTextBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDamageBox::Clone(void* pArg)
{
	CDamageBox* pInstance = new CDamageBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTextBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDamageBox::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTransformCom);

	for (auto& Value : *m_pTextInstanceValue)
	{
		Safe_Delete(Value.second);
	}
	
	for (auto& Buffer : *m_pTextInstanceBuffer)
	{
		Safe_Release(Buffer.second);
	}

	for (auto& Shader : *m_pTextInstanceShader)
	{
		Safe_Release(Shader.second);
	}
}
