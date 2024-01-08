#include "stdafx.h"
#include "UI_Lobby_NickName.h"
#include "GameInstance.h"
#include "TextBox.h"

CUI_Lobby_NickName::CUI_Lobby_NickName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Lobby_NickName::CUI_Lobby_NickName(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Lobby_NickName::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Lobby_NickName::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		_uint* strIndex = static_cast<_uint*>(pArg);
		m_iClassIndex = *strIndex;
		m_strUITag = TEXT("Lobby_NickName");
		m_strUITag += to_wstring(m_iClassIndex + 1);
	}
	else
		m_strUITag = TEXT("Lobby_NickName");

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));
	
	m_pTransformCom_Emblem->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom_Emblem->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_Lobby_NickName::Tick(_float fTimeDelta)
{
	InitialIze_EmblemPos();
	__super::Tick(fTimeDelta);
	Moving_Rect();
	Picking_UI();
}

void CUI_Lobby_NickName::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Update_NickNameFrame();
}

HRESULT CUI_Lobby_NickName::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	m_pTextureCom_NickNameShine->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Emblem()))
		return E_FAIL;
	m_pTextureCom_Emblem->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iClassIndex);
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Lobby_NickName::Ready_Components()
{
	__super::Ready_Components();

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Emblem"), (CComponent**)&m_pTransformCom_Emblem)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NickName_Frame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NickName_ShineEffect"),
		TEXT("Com_Texture_Shine"), (CComponent**)&m_pTextureCom_NickNameShine)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Emblem"),
		TEXT("Com_Texture_Emblem"), (CComponent**)&m_pTextureCom_Emblem)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CUI_Lobby_NickName::Bind_ShaderResources()
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


	return S_OK;
}

HRESULT CUI_Lobby_NickName::Bind_ShaderResources_Emblem()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom_Emblem->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	return S_OK;
}

void CUI_Lobby_NickName::Update_NickNameFrame()
{
	if (!m_bPick)
		return;

	if ((m_bPick) && ((KEY_HOLD(KEY::LBTN)) || KEY_TAP(KEY::LBTN)))
	{
		m_iTextureIndex = 1;
	}
	
	if ((!m_bPick) && ((KEY_HOLD(KEY::RBTN)) || KEY_TAP(KEY::RBTN)))
	m_iTextureIndex = 0;
}

void CUI_Lobby_NickName::InitialIze_EmblemPos()
{
	if (m_bInitializeEmblem)
		return;

	Vec3 vPos = {};
	vPos.x = (m_fX - (m_fSizeX * 0.40f) - g_iWinSizeX * 0.5f);
	vPos.y = (-(m_fY - 10.f)) + (g_iWinSizeY * 0.5f);
	vPos.z = 0.2f;

	m_pTransformCom_Emblem->Set_Scale(Vec3(32.f, 32.f, 0.f));
	m_pTransformCom_Emblem->Set_State(CTransform::STATE_POSITION, vPos);

	m_bInitializeEmblem = true;
}

void CUI_Lobby_NickName::Update_Equipment_Level()
{
}

void CUI_Lobby_NickName::Print_NickName()
{

}

void CUI_Lobby_NickName::Set_Active(_bool bActive)
{
}

HRESULT CUI_Lobby_NickName::Ready_TextBox()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	if (nullptr == m_pNickNameWnd)
	{
		CTextBox::TEXTBOXDESC tTextDesc;
		tTextDesc.szTextBoxTag = TEXT("NickNameWnd");
		tTextDesc.vSize = Vec2(228.f, 53.0f);
		m_pNickNameWnd = static_cast<CTextBox*>(pGameInstance->
			Add_GameObject(LEVELID::LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_TextBox"), &tTextDesc));
	
		if (nullptr == m_pNickNameWnd)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		m_pNickNameWnd->Set_ScaleUV(Vec2(1.0f, 1.0f));
		m_pNickNameWnd->Set_Pos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);
	}


	Safe_Release(pGameInstance);
	return S_OK;
}

void CUI_Lobby_NickName::Set_NickName(const wstring& strStageName)
{

}

CUI_Lobby_NickName* CUI_Lobby_NickName::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Lobby_NickName* pInstance = new CUI_Lobby_NickName(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Lobby_NickName");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Lobby_NickName::Clone(void* pArg)
{
	CUI_Lobby_NickName* pInstance = new CUI_Lobby_NickName(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Lobby_NickName");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Lobby_NickName::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom_NickNameShine);
	Safe_Release(m_pTextureCom_Emblem);
	Safe_Release(m_pTextureCom_Equipment);
	Safe_Release(m_pTransformCom_Emblem);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
