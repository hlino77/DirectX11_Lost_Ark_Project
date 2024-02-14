#include "stdafx.h"
#include "UI_ValtanClearWnd.h"
#include "GameInstance.h"
#include "Engine_Defines.h"
#include "Sound_Manager.h"

CUI_ValtanClearWnd::CUI_ValtanClearWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}

CUI_ValtanClearWnd::CUI_ValtanClearWnd(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ValtanClearWnd::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_ValtanClearWnd::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;
	m_fSizeX = 364.f * 2.f;
	m_fSizeY = 390.f * 2.f;

	m_strUITag = TEXT("UI_ValtanClearWnd");

	//m_pTransformCom
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 10.f) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_BackGround
	m_pTransform_BackGround->Set_Scale(Vec3(g_iWinSizeX, g_iWinSizeY, 1.f));
	m_pTransform_BackGround->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_ClearBackGround
	m_pTransform_ClearBackGround->Set_Scale(Vec3(1908.f * 0.6f, 1053.f * 0.6f, 1.f));
	m_pTransform_ClearBackGround->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 20.f) + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_MaskedAnimEffect
	m_pTransform_MaskedAnimEffect->Set_Scale(Vec3(524.f * 0.7f, 524.f * 0.7f, 1.f));
	m_pTransform_MaskedAnimEffect->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY) + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_MaskedAnimShine
	m_pTransform_MaskedAnimShine->Set_Scale(Vec3(500.f, 500.f, 1.f));
	m_pTransform_MaskedAnimShine->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY)+g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_ClearDots
	m_pTransform_ClearDots->Set_Scale(Vec3(774.f * 0.8f, 280.f * 0.8f, 1.f));
	m_pTransform_ClearDots->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY) + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_MaskedCircle
	m_pTransform_MaskedCircle->Set_Scale(Vec3(524.f * 0.8f, 524.f * 0.8f, 1.f));
	m_pTransform_MaskedCircle->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_MaskedShine
	m_pTransform_MaskedShine->Set_Scale(Vec3(1008, 818.f, 1.f));
	m_pTransform_MaskedShine->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY) + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_ClearCircle
	m_pTransform_ClearCircle->Set_Scale(Vec3(550.f * 0.6f, 550.f * 0.6f, 1.f));
	m_pTransform_ClearCircle->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	m_fSizeX_Text = 333.f * 2.f;
	m_fSizeY_Text = 70.f * 2.f;
	//m_pTransform_ClearText
	m_pTransform_ClearText->Set_Scale(Vec3(m_fSizeX_Text, m_fSizeY_Text, 1.f));
	m_pTransform_ClearText->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX) - g_iWinSizeX * 0.5f, -(m_fY + 90.f) + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_ValtanClearWnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_ValtanClearWnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Update_BackGroundAlpha(fTimeDelta);
	Update_ClearBackGround(fTimeDelta);
	Update_MaskedAnimFrame(fTimeDelta);
	Update_MaskedShineAnimFrame(fTimeDelta);
	Update_MaskedShine(fTimeDelta);
	Update_Emblem(fTimeDelta);
	Update_Text(fTimeDelta);
}

HRESULT CUI_ValtanClearWnd::Render()
{
	if (FAILED(Bind_ShaderResources_BackGround()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ClearBackGround()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_MaskedAnimEffect()))
		return E_FAIL;
	m_pShaderCom->Begin(3);
	m_pVIBufferCom->Render();

	if(FAILED(Bind_ShaderResources_MaskedShineAnim()))
		return E_FAIL;
	m_pShaderCom->Begin(18);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_ClearDots()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_MaskedCircle()))
		return E_FAIL;
	m_pShaderCom->Begin(18);
	m_pVIBufferCom->Render();
	 
	if (FAILED(Bind_ShaderResources_MaskedShine()))
		return E_FAIL;
	m_pShaderCom->Begin(18);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResourcese_ClearCircle()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	////클리어 엠블럼
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResourcese_ClearText()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_ValtanClearWnd::Update_BackGroundAlpha(_float fTimeDelta)
{
	if (m_bValtanClear)
	{
		if (1.f > m_fBackGroundAlpha)
			m_fBackGroundAlpha += 1.5f * fTimeDelta;
		if (1.f < m_fBackGroundAlpha)
		{
			m_fBackGroundAlpha = 1.f;
			m_bClearAnim = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"UI", L"Clear.wav", CSound_Manager::GetInstance()->Get_ChannelGroupVolume(TEXT("UI")));
		}

	}
}

void CUI_ValtanClearWnd::Update_ClearBackGround(_float fTimeDelta)
{
	if (!m_bClearAnim)
		return;

	if (1.f > m_fClearAnimAlpha)
		m_fClearAnimAlpha +=  3.f * fTimeDelta;
	if (1.f < m_fClearAnimAlpha)
	{
		m_fClearAnimAlpha = 1.f;
		m_bMaskedShine = true;
	}
}

void CUI_ValtanClearWnd::Update_MaskedAnimFrame(_float fTimeDelta)
{
	if (!m_bTextOn)
		return;

	if (32.f > m_fMaskedAnimFrame)
		m_fMaskedAnimFrame += 32.f * fTimeDelta;
	if (32.f <= m_fMaskedAnimFrame)
		m_fMaskedAnimFrame = 0.f;

	if (1.f > m_fMaskedAnimAlpha)
		m_fMaskedAnimAlpha += fTimeDelta;
	if (1.f < m_fMaskedAnimAlpha)
		m_fMaskedAnimAlpha = 1.f;
}

void CUI_ValtanClearWnd::Update_MaskedShineAnimFrame(_float fTimeDelta)
{
	if (!m_bTextOn)
		return;

	if (40.f > m_fMaskedShineAnimFrame)
		m_fMaskedShineAnimFrame += 20.f * fTimeDelta;
	if (40.f <= m_fMaskedShineAnimFrame)
		m_fMaskedShineAnimFrame = 0.f;
}

void CUI_ValtanClearWnd::Update_MaskedCircle(_float fTimeDelta)
{
	if (!m_bClearAnim)
		return;


}

void CUI_ValtanClearWnd::Update_MaskedShine(_float fTimeDelta)
{
	if (false == m_bMaskedShine)
	{
		m_fMaskedShineAlpha = 0.f;
	}
	else
	{
		m_fMaskedShineAlpha = 1.f;
	}
}

void CUI_ValtanClearWnd::Update_Emblem(_float fTimeDelta)
{
	if (!m_bMaskedShine)
		return;

	if (1.f > m_fEmblemAlpha)
		m_fEmblemAlpha += 1.5f * fTimeDelta;
	if (1.f < m_fEmblemAlpha)
		m_fEmblemAlpha = 1.f;

	if (364.f * 0.8f < m_fSizeX)
	{
		m_fSizeX -= m_fSizeX * 0.1f;
		m_fSizeY -= m_fSizeY * 0.1f;
		m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	}
	if (364.f * 0.8f > m_fSizeX)
	{
		m_fSizeX = 364.f * 0.8f;
		m_fSizeY = 390.f * 0.8f;
		m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
		m_bTextOn = true;
	}
}

void CUI_ValtanClearWnd::Update_Text(_float fTimeDelta)
{
	if (!m_bTextOn)
		return;

	if (1.f > m_fAlpha_Text)
		m_fAlpha_Text += 1.5f * fTimeDelta;
	if (1.f < m_fAlpha_Text)
		m_fAlpha_Text = 1.f;

	if (333.f * 0.6f < m_fSizeX_Text)
	{
		m_fSizeX_Text -= m_fSizeX_Text * 0.15f;
		m_fSizeY_Text -= m_fSizeY_Text * 0.15f;
		m_pTransform_ClearText->Set_Scale(Vec3(m_fSizeX_Text, m_fSizeY_Text, 1.f));
	}
	if (333.f * 0.6f > m_fSizeX_Text)
	{
		m_fSizeX_Text = 333.f * 0.6f;
		m_fSizeY_Text = 70.f * 0.6f;
		m_pTransform_ClearText->Set_Scale(Vec3(m_fSizeX_Text, m_fSizeY_Text, 1.f));
		m_bTextOn = true;
	}
	if (KEY_TAP(KEY::LBTN))
		m_bActive = false;
}

HRESULT CUI_ValtanClearWnd::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	//m_pTexture_PartyHPWnd
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanClear_ClearEmblem"),
		TEXT("Com_Texture_Emblem"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	//m_pTexture_BackGround
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanClear_BackGroundWnd"),
		TEXT("Com_Texture_BackGround"), (CComponent**)&m_pTexture_BackGround)))
		return E_FAIL;
	//m_pTexture_ClearBackGround
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanClear_ClearBackGround"),
		TEXT("Com_Texture_ClearBackGround"), (CComponent**)&m_pTexture_ClearBackGround)))
		return E_FAIL;
	//m_pTexture_MaskedAnimEffect
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanClear_Masked_Anim"),
		TEXT("Com_Texture_MaskedAnimEffect"), (CComponent**)&m_pTexture_MaskedAnimEffect)))
		return E_FAIL;
	//m_pTexture_MaskedAnimShine
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanClear_MaskedShine_Anim"),
		TEXT("Com_Texture_MaskedAnimShine"), (CComponent**)&m_pTexture_MaskedAnimShine)))
		return E_FAIL;
	//m_pTexture_ClearDots
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanClear_ClearDots"),
		TEXT("Com_Texture_ClearDots"), (CComponent**)&m_pTexture_ClearDots)))
		return E_FAIL;
	//m_pTexture_MaskedCircle
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanClear_ClearMasked"),
		TEXT("Com_Texture_MaskedCircle"), (CComponent**)&m_pTexture_MaskedCircle)))
		return E_FAIL;
	//m_pTexture_MaskedShine
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanClear_MaskedBackGround"),
		TEXT("Com_Texture_MaskedShine"), (CComponent**)&m_pTexture_MaskedShine)))
		return E_FAIL;
	//m_pTexture_ClearCircle
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanClear_ClearCircle"),
		TEXT("Com_Texture_ClearCircle"), (CComponent**)&m_pTexture_ClearCircle)))
		return E_FAIL;
	//m_pTexture_ClearText
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanClear_ClearText"),
		TEXT("Com_Texture_ClearText"), (CComponent**)&m_pTexture_ClearText)))
		return E_FAIL;

	//m_pTransform_BackGround
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BackGround"), (CComponent**)&m_pTransform_BackGround)))
		return E_FAIL;
	//m_pTransform_ClearBackGround
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_ClearBackGround"), (CComponent**)&m_pTransform_ClearBackGround)))
		return E_FAIL;
	//m_pTransform_MaskedAnimEffect
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_MaskedAnimEffect"), (CComponent**)&m_pTransform_MaskedAnimEffect)))
		return E_FAIL;
	//m_pTransform_MaskedAnimShine
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_MaskedAnimShine"), (CComponent**)&m_pTransform_MaskedAnimShine)))
		return E_FAIL;
	//m_pTransform_ClearDots
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_ClearDots"), (CComponent**)&m_pTransform_ClearDots)))
		return E_FAIL;
	//m_pTransform_MaskedCircle
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Masked"), (CComponent**)&m_pTransform_MaskedCircle)))
		return E_FAIL;
	//m_pTransform_MaskedShine
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_MaskedShine"), (CComponent**)&m_pTransform_MaskedShine)))
		return E_FAIL;
	//m_pTransform_ClearCircle
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_ClearCircle"), (CComponent**)&m_pTransform_ClearCircle)))
		return E_FAIL;
	//m_pTransform_ClearText
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_ClearText"), (CComponent**)&m_pTransform_ClearText)))
		return E_FAIL;
	return S_OK;
}

HRESULT CUI_ValtanClearWnd::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fEmblemAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_ValtanClearWnd::Bind_ShaderResources_BackGround()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_BackGround->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fBackGroundAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_BackGround->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_ValtanClearWnd::Bind_ShaderResources_ClearBackGround()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ClearBackGround->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fClearAnimAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_ClearBackGround->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_ValtanClearWnd::Bind_ShaderResources_MaskedAnimEffect()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_MaskedAnimEffect->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fMaskedAnimAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_MaskedAnimEffect->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fMaskedAnimFrame);
	return S_OK;
}

HRESULT CUI_ValtanClearWnd::Bind_ShaderResources_MaskedShineAnim()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_MaskedAnimShine->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fMaskedAnimAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_MaskedAnimShine->Set_SRV(m_pShaderCom, "g_MaskTexture", (_uint)m_fMaskedShineAnimFrame);
	return S_OK;
}

HRESULT CUI_ValtanClearWnd::Bind_ShaderResources_ClearDots()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ClearDots->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fMaskedShineAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_ClearDots->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_ValtanClearWnd::Bind_ShaderResources_MaskedCircle()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_MaskedCircle->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fClearAnimAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_MaskedCircle->Set_SRV(m_pShaderCom, "g_MaskTexture");
	return S_OK;
}

HRESULT CUI_ValtanClearWnd::Bind_ShaderResources_MaskedShine()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_MaskedShine->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fMaskedShineAlpha, sizeof(_float))))
		return E_FAIL;
	//Vec4	vColor = Vec4(5.f, 5.f, 5.f, 1.f);	러프최대값
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vMaskedColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_MaskedShine->Set_SRV(m_pShaderCom, "g_MaskTexture");

	return S_OK;
}

HRESULT CUI_ValtanClearWnd::Bind_ShaderResourcese_ClearCircle()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ClearCircle->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fClearAnimAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_ClearCircle->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_ValtanClearWnd::Bind_ShaderResourcese_ClearText()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ClearText->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha_Text, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_ClearText->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

CUI_ValtanClearWnd* CUI_ValtanClearWnd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ValtanClearWnd* pInstance = new CUI_ValtanClearWnd(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ValtanClearWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ValtanClearWnd::Clone(void* pArg)
{
	CUI_ValtanClearWnd* pInstance = new CUI_ValtanClearWnd(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ValtanClearWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ValtanClearWnd::Free()
{
	__super::Free();
	
	Safe_Release(m_pTexture_BackGround);
	Safe_Release(m_pTexture_ClearBackGround);
	Safe_Release(m_pTexture_MaskedAnimEffect);
	Safe_Release(m_pTexture_MaskedAnimShine);
	Safe_Release(m_pTexture_ClearDots);
	Safe_Release(m_pTexture_MaskedCircle);
	Safe_Release(m_pTexture_MaskedShine);
	Safe_Release(m_pTexture_ClearCircle);
	Safe_Release(m_pTexture_ClearText);

	Safe_Release(m_pTransform_BackGround);
	Safe_Release(m_pTransform_ClearBackGround);
	Safe_Release(m_pTransform_MaskedAnimEffect);
	Safe_Release(m_pTransform_MaskedAnimShine);
	Safe_Release(m_pTransform_ClearDots);
	Safe_Release(m_pTransform_MaskedCircle);
	Safe_Release(m_pTransform_MaskedShine);
	Safe_Release(m_pTransform_ClearCircle);
	Safe_Release(m_pTransform_ClearText);
}
