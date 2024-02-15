#include "stdafx.h"
#include "UI_ChaosDungeon_Clear.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "TextBox.h"
#include "Sound_Manager.h"

CUI_ChaosDungeon_Clear::CUI_ChaosDungeon_Clear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_ChaosDungeon_Clear::CUI_ChaosDungeon_Clear(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ChaosDungeon_Clear::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_ChaosDungeon_Clear::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("ChaosDungeon_Clear");

	m_fSizeX = 268.f;
	m_fSizeY = 268.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	m_pTransform_ClearLine->Set_Scale(Vec3(1492.f, 258.f, 1.f));
	m_pTransform_ClearLine->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 50.f) - g_iWinSizeX * 0.5f, -543.f + g_iWinSizeY * 0.5f, 0.f));

	m_pTransform_ClearShine->Set_Scale(Vec3(186.f, 100.f, 1.f));
	m_pTransform_ClearShine->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -492.f + g_iWinSizeY * 0.5f, 0.f));

	m_fSizeX_Line_L = 29.f;
	m_fX_Line_L = g_iWinSizeX * 0.5f;
	m_pTransform_Line_L->Set_Scale(Vec3(m_fSizeX_Line_L, 14.f, 1.f));//최소 29.f, 14.f 기본 281.f, 14.f 최대 423.f, 14.f
	m_pTransform_Line_L->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX_Line_L - g_iWinSizeX * 0.5f, -492.f + g_iWinSizeY * 0.5f, 0.f));

	m_fSizeX_Line_R = 29.f;
	m_fX_Line_R = g_iWinSizeX * 0.5f;
	m_pTransform_Line_R->Set_Scale(Vec3(m_fSizeX_Line_R, 14.f, 1.f));
	m_pTransform_Line_R->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX_Line_R - g_iWinSizeX * 0.5f, -492.f + g_iWinSizeY * 0.5f, 0.f));

	m_fSizeX_Font = 553.f * 1.2f;//Origin 553.f
	m_fSizeY_Font = 138.f * 1.2f;//Origin 138.f
	m_pTransform_ClearMessage->Set_Scale(Vec3(m_fSizeX_Font, m_fSizeY_Font, 1.f));
	m_pTransform_ClearMessage->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - 50.f)+ g_iWinSizeY * 0.5f, 0.f));

	m_pTransform_ClearBackGround->Set_Scale(Vec3(g_iWinSizeX, g_iWinSizeY, 1.f));
	m_pTransform_ClearBackGround->Set_State(CTransform::STATE_POSITION,
		Vec3((g_iWinSizeX * 0.5f) - g_iWinSizeX * 0.5f, -(g_iWinSizeY * 0.5f) + g_iWinSizeY * 0.5f, 0.f));

	m_pTransform_ClearCheckButton->Set_Scale(Vec3(101.f, 34.f, 1.f));
	m_pTransform_ClearCheckButton->Set_State(CTransform::STATE_POSITION,
		Vec3((g_iWinSizeX * 0.5f) - g_iWinSizeX * 0.5f, -(g_iWinSizeY * 0.5f + 300.f) + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_ChaosDungeon_Clear::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bStart)
	{
		Create_ClearCheckButton();

		if (22.f > m_fFrame)
			m_fFrame += 22.f * fTimeDelta;
		else if (22.f < m_fFrame)
		{
			m_fFrame = 22.f;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Clear.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
	}
}

void CUI_ChaosDungeon_Clear::LateTick(_float fTimeDelta)
{
	if(m_bStart)
		__super::LateTick(fTimeDelta);
	Update_Clear(fTimeDelta);
	Update_ClearCheckButton();
}

HRESULT CUI_ChaosDungeon_Clear::Render()
{
	if (FAILED(Bind_ShaderResources_ClearBackGround()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (m_bClearShine)
	{
		if (FAILED(Bind_ShaderResources_ClearLine()))
			return E_FAIL;
		m_pShaderCom->Begin(0);
		m_pVIBufferCom->Render();

		if (FAILED(Bind_ShaderResources_Line_L()))
			return E_FAIL;
		m_pShaderCom->Begin(0);
		m_pVIBufferCom->Render();

		if (FAILED(Bind_ShaderResources_Line_R()))
			return E_FAIL;
		m_pShaderCom->Begin(0);
		m_pVIBufferCom->Render();

		if (FAILED(Bind_ShaderResources_ClearShine()))
			return E_FAIL;
		m_pShaderCom->Begin(0);
		m_pVIBufferCom->Render();

		if (FAILED(Bind_ShaderResources_ClearFont()))
			return E_FAIL;
		m_pShaderCom->Begin(0);
		m_pVIBufferCom->Render();

		if (FAILED(Bind_ShaderResources_ClearCheckButton()))
			return E_FAIL;
		m_pShaderCom->Begin(0);
		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_ChaosDungeon_Clear::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Clear_Circle"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Clear_Line"),
		TEXT("Com_Texture_ClearLine"), (CComponent**)&m_pTexture_ClearLine)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Clear_Shine"),
		TEXT("Com_Texture_ClearShine"), (CComponent**)&m_pTexture_ClearShine)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Line_L"),
		TEXT("Com_Texture_Line_L"), (CComponent**)&m_pTexture_Line_L)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Line_R"),
		TEXT("Com_Texture_Line_R"), (CComponent**)&m_pTexture_Line_R)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ClearFont"),
		TEXT("Com_Texture_ClearMessage"), (CComponent**)&m_pTexture_ClearMessage)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BackGroundWnd"),
		TEXT("Com_Texture_ClearBackGround"), (CComponent**)&m_pTexture_ClearBackGround)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Clear_CheckButton"),
		TEXT("Com_Texture_ClearCheckButton"), (CComponent**)&m_pTexture_ClearCheckButton)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_ClearLine"), (CComponent**)&m_pTransform_ClearLine)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_ClearShine"), (CComponent**)&m_pTransform_ClearShine)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Line_L"), (CComponent**)&m_pTransform_Line_L)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Line_R"), (CComponent**)&m_pTransform_Line_R)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_ClearMessage"), (CComponent**)&m_pTransform_ClearMessage)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_ClearBackGround"), (CComponent**)&m_pTransform_ClearBackGround)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_ClearCheckButton"), (CComponent**)&m_pTransform_ClearCheckButton)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ChaosDungeon_Clear::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture",(_uint)m_fFrame);

	return S_OK;
}

HRESULT CUI_ChaosDungeon_Clear::Bind_ShaderResources_ClearLine()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ClearLine->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fClearLineAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_ClearLine->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_ChaosDungeon_Clear::Bind_ShaderResources_ClearShine()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ClearShine->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_ClearShine->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_ChaosDungeon_Clear::Bind_ShaderResources_Line_L()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Line_L->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_Line_L->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_ChaosDungeon_Clear::Bind_ShaderResources_Line_R()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Line_R->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_Line_R->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_ChaosDungeon_Clear::Bind_ShaderResources_ClearFont()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ClearMessage->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fClearFontAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_ClearMessage->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_ChaosDungeon_Clear::Bind_ShaderResources_ClearBackGround()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ClearBackGround->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_ClearBackGround->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_ChaosDungeon_Clear::Bind_ShaderResources_ClearCheckButton()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ClearCheckButton->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_ClearCheckButton->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);

	return S_OK;
}

void CUI_ChaosDungeon_Clear::Update_Clear(_float fTimeDelta)
{
	if (19.f <= m_fFrame)
		m_bClearShine = true;

	if (true == m_bClearShine)
		Update_Line(fTimeDelta);

	Update_ClearLine(fTimeDelta);
	Update_ClearFont(fTimeDelta);
}

void CUI_ChaosDungeon_Clear::Update_Line(_float fTimeDelta)
{
	if (!m_bDecreaseLine)
	{
		if (411.f > m_fSizeX_Line_L)
		{
			m_fSizeX_Line_L += 400.f * fTimeDelta;
			m_pTransform_Line_L->Set_Scale(Vec3(m_fSizeX_Line_L, 14.f, 1.f));
			m_pTransform_Line_L->Set_State(CTransform::STATE_POSITION,
				Vec3((m_fX_Line_L - ((400.f * fTimeDelta )*0.5f)) - g_iWinSizeX * 0.5f, -492.f + g_iWinSizeY * 0.5f, 0.f));
		}

		if (411.f > m_fSizeX_Line_R)
		{
			m_fSizeX_Line_R += 400.f * fTimeDelta;
			m_pTransform_Line_R->Set_Scale(Vec3(m_fSizeX_Line_R, 14.f, 1.f));
			m_pTransform_Line_R->Set_State(CTransform::STATE_POSITION,
				Vec3((m_fX_Line_R + ((400.f * fTimeDelta) * 0.5f)) - g_iWinSizeX * 0.5f, -492.f + g_iWinSizeY * 0.5f, 0.f));
		}

		if ((411.f <= m_fSizeX_Line_L) || (411.f <= m_fSizeX_Line_R))
		{
			m_bShowFont = true;
			m_bDecreaseLine = true;
			m_bClearLine = true;
		}
	}
	else
	{
		if(0.1f < m_fSizeX_Line_L)
		{
			m_fSizeX_Line_L -= 400.f * fTimeDelta;
			m_pTransform_Line_L->Set_Scale(Vec3(m_fSizeX_Line_L, 14.f, 1.f));
			m_pTransform_Line_L->Set_State(CTransform::STATE_POSITION,
			Vec3((m_fX_Line_L -= ((400.f * fTimeDelta)*0.5f)) - g_iWinSizeX * 0.5f, -492.f + g_iWinSizeY * 0.5f, 0.f));
		}
		else
		{
			m_fSizeX_Line_L = 0.f;
			m_pTransform_Line_L->Set_Scale(Vec3(m_fSizeX_Line_L, 14.f, 1.f));
			m_pTransform_Line_L->Set_State(CTransform::STATE_POSITION,
				Vec3(m_fX_Line_L - g_iWinSizeX * 0.5f, -492.f + g_iWinSizeY * 0.5f, 0.f));
		}

		if (0.1f < m_fSizeX_Line_R)
		{
			m_fSizeX_Line_R -= 400.f * fTimeDelta;
			m_pTransform_Line_R->Set_Scale(Vec3(m_fSizeX_Line_R, 14.f, 1.f));
			m_pTransform_Line_R->Set_State(CTransform::STATE_POSITION,
				Vec3((m_fX_Line_R += ((400.f * fTimeDelta) * 0.5f)) - g_iWinSizeX * 0.5f, -492.f + g_iWinSizeY * 0.5f, 0.f));
		}
		else
		{
			m_fSizeX_Line_R = 0.f;
			m_pTransform_Line_R->Set_Scale(Vec3(m_fSizeX_Line_R, 14.f, 1.f));
			m_pTransform_Line_R->Set_State(CTransform::STATE_POSITION,
				Vec3(m_fX_Line_R - g_iWinSizeX * 0.5f, -492.f + g_iWinSizeY * 0.5f, 0.f));
		}
		
	}
}

void CUI_ChaosDungeon_Clear::Update_ClearLine(_float fTimeDelta)
{
	if (!m_bClearLine)
		return;

	if (1.f > m_fClearLineAlpha)
		m_fClearLineAlpha += 4.f * fTimeDelta;
	else if (1.f < m_fClearLineAlpha)
		m_fClearLineAlpha = 1.f;
}

void CUI_ChaosDungeon_Clear::Update_ClearFont(_float fTimeDelta)
{
	if (!m_bShowFont)
		return;

	if (553.f < m_fSizeX_Font)
		m_fSizeX_Font -= ((m_fSizeX_Font * 0.5f) * fTimeDelta);
	if(138.f < m_fSizeY_Font)
		m_fSizeY_Font -= ((m_fSizeY_Font * 0.5f) * fTimeDelta);

	if (1.f > m_fClearFontAlpha)
		m_fClearFontAlpha += 0.5f * fTimeDelta;

	m_pTransform_ClearMessage->Set_Scale(Vec3(m_fSizeX_Font, m_fSizeY_Font, 1.f));

}

void CUI_ChaosDungeon_Clear::Update_ClearCheckButton()
{
	if (!m_bStart)
		return;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);  // 클라이언트 내에 마우스 포인터 가져오기 
	_uint ViewPortIndex = 1;
	CD3D11_VIEWPORT ViewPort;
	m_pContext->RSGetViewports(&ViewPortIndex, &ViewPort); // 뷰포트 가져오기 

	Is_Picking_ClearButton(pt);

	if ((true == m_bPickedCheckButton) && (KEY_TAP(KEY::LBTN)))
	{
		CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"ClickedSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		m_bStart = false;
		//m_bActive = false;
	}
}

void CUI_ChaosDungeon_Clear::Is_Picking_ClearButton(POINT pt)
{
	if (PtInRect(&m_rcClearCheckButton, pt))
	{
		if (!m_bSound)
		{
			m_bSound = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Is_PickingSound.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}
		m_iTextureIndex = 1;
		m_bPickedCheckButton = true;
	}
	else
	{
		m_iTextureIndex = 0;
		m_bPickedCheckButton = false;
		m_bSound = false;
	}
}

void CUI_ChaosDungeon_Clear::Create_ClearCheckButton()
{
	m_rcClearCheckButton.left = LONG((m_fX) - (101.f / 2));;
	m_rcClearCheckButton.top = LONG((m_fY + 300.f) - (34.f / 2));;
	m_rcClearCheckButton.right = LONG((m_fX) + (101.f / 2));;
	m_rcClearCheckButton.bottom = LONG((m_fY + 300.f) + (34.f / 2));;
}

CUI_ChaosDungeon_Clear* CUI_ChaosDungeon_Clear::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ChaosDungeon_Clear* pInstance = new CUI_ChaosDungeon_Clear(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ChaosDungeon_Clear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ChaosDungeon_Clear::Clone(void* pArg)
{
	CUI_ChaosDungeon_Clear* pInstance = new CUI_ChaosDungeon_Clear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ChaosDungeon_Clear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ChaosDungeon_Clear::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	
	Safe_Release(m_pTexture_ClearLine);
	Safe_Release(m_pTexture_ClearShine);
	Safe_Release(m_pTexture_Line_L);
	Safe_Release(m_pTexture_Line_R);
	Safe_Release(m_pTexture_ClearMessage);
	Safe_Release(m_pTexture_ClearBackGround);
	Safe_Release(m_pTexture_ClearCheckButton);

	Safe_Release(m_pTransform_ClearLine);
	Safe_Release(m_pTransform_ClearShine);
	Safe_Release(m_pTransform_Line_L);
	Safe_Release(m_pTransform_Line_R);
	Safe_Release(m_pTransform_ClearMessage);
	Safe_Release(m_pTransform_ClearBackGround);
	Safe_Release(m_pTransform_ClearCheckButton);

}
