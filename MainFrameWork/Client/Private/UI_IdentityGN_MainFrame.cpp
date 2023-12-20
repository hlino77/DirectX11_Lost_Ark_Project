#include "stdafx.h"
#include "UI_IdentityGN_MainFrame.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"
#include "ServerSessionManager.h"

CUI_IdentityGN_MainFrame::CUI_IdentityGN_MainFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_IdentityGN_MainFrame::CUI_IdentityGN_MainFrame(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_IdentityGN_MainFrame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_IdentityGN_MainFrame::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("IdentityGN_MainFrame");

	m_fSizeX = 400.f;
	m_fSizeY = 268.f;
	m_fX = 12.f;
	m_fY = 543.f;
	m_fAlpha = 0.5f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f) + (m_fSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(0.f));

	CGameInstance* pInstance = CGameInstance::GetInstance();
	Safe_AddRef(pInstance);
	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	if (nullptr != pPlayer)
	{
		m_iCurrStance = static_cast<CPlayer_Controller_GN*>(static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller())->Get_GN_Identity();
		m_iPreStance = m_iCurrStance;
	}
	
	Safe_Release(pInstance);

	return S_OK;
}

void CUI_IdentityGN_MainFrame::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	CGameInstance* pInstance = CGameInstance::GetInstance();
	Safe_AddRef(pInstance);

	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	if (nullptr != pPlayer)
	{
		CPlayer_Controller_GN::GN_IDENTITY eIDentity = static_cast<CPlayer_Controller_GN*>(static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller())->Get_GN_Identity();
		if (CPlayer_Controller_GN::GN_IDENTITY::HAND == eIDentity)
		{
			m_iCurrStance	= (_uint)CPlayer_Controller_GN::GN_IDENTITY::HAND;
			m_iTextureIndex = (_uint)CPlayer_Controller_GN::GN_IDENTITY::HAND;
		}
		else if (CPlayer_Controller_GN::GN_IDENTITY::SHOT == eIDentity)
		{
			m_iCurrStance = (_uint)CPlayer_Controller_GN::GN_IDENTITY::SHOT;
			m_iTextureIndex = (_uint)CPlayer_Controller_GN::GN_IDENTITY::SHOT;
		}
		else if (CPlayer_Controller_GN::GN_IDENTITY::LONG == eIDentity)
		{
			m_iCurrStance = (_uint)CPlayer_Controller_GN::GN_IDENTITY::LONG;
			m_iTextureIndex = (_uint)CPlayer_Controller_GN::GN_IDENTITY::LONG;
		}
		if ((TEXT("Identity_GN") == pPlayer->Get_State()) || ((TEXT("Identity_GN_Run") == pPlayer->Get_State())))
		{
			if (m_iPreStance != m_iCurrStance)
			{
				m_iPreStance = m_iCurrStance;
				m_bChangeIcon = false;
				m_bTapKey = false;
			}
		}
		else if ((TEXT("Identity_GN_Back") == pPlayer->Get_State()) || ((TEXT("Identity_GN_Run_Back") == pPlayer->Get_State())))
		{
			if (m_iPreStance != m_iCurrStance)
			{
				m_iPreStance = m_iCurrStance;
				m_bChangeIcon = false;
				m_bTapKey = true;
			}
		}
	}
	
	Change_Icon(fTimeDelta);

	Safe_Release(pInstance);
}

void CUI_IdentityGN_MainFrame::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_IdentityGN_MainFrame::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_IdentityGN_MainFrame::UI_Tick(_float fTimeDelta)
{
}

Matrix CUI_IdentityGN_MainFrame::Get_WorldMatrix()
{
	
	return m_pTransformCom->Get_WorldMatrix();
}

void CUI_IdentityGN_MainFrame::Set_Angle(_float fAngle)
{
	m_fAngle = fAngle;
	m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(m_fAngle));
}

void CUI_IdentityGN_MainFrame::Change_Icon(_float fTimeDelta)
{
	if (!m_bChangeIcon)
	{
		if (!m_bTapKey)
		{
			if ((380.f > m_fAngle) && (!m_bReturn))
			{
				m_fAngle += 2500.f * fTimeDelta;
				m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(m_fAngle));
			}
			else if ((380.f <= m_fAngle) && (!m_bReturn))
			{
				m_bReturn = true;
				m_bEventOn = true;
			}
			if ((m_bReturn) && (360.f < m_fAngle))
			{
				m_fAngle -= 500.f * fTimeDelta;
				m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(m_fAngle));
			}
			else if ((m_bReturn) && (360.f >= m_fAngle))
			{
				m_bEventOn = false;
				m_bReturn = false;
				m_bChangeIcon = true;
				m_fAngle = 0.f;
				m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(0.f));
			}
		}
		else if (m_bTapKey)
		{
			if ((380.f > m_fAngle) && (!m_bReturn))
			{
				m_fAngle += 2500.f *fTimeDelta;
				m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(-m_fAngle));
			}
			else if ((380.f <= m_fAngle) && (!m_bReturn))
			{
				m_bReturn = true;
				m_bEventOn = true;
			}
			if ((m_bReturn) && (360.f < m_fAngle))
			{
				m_fAngle -= 500.f * fTimeDelta;
				m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(-m_fAngle));
			}
			else if ((m_bReturn) && (360.f >= m_fAngle))
			{
				m_bEventOn = false;
				m_bReturn = false;
				m_bChangeIcon = true;
				m_fAngle = 0.f;
				m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(0.f));
			}
		}
	}
}

HRESULT CUI_IdentityGN_MainFrame::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_MainFrame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_IdentityGN_MainFrame::Bind_ShaderResources()
{
	if (__super::Bind_ShaderResources())
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);

	return S_OK;
}

CUI_IdentityGN_MainFrame* CUI_IdentityGN_MainFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentityGN_MainFrame* pInstance = new CUI_IdentityGN_MainFrame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentityGN_MainFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentityGN_MainFrame::Clone(void* pArg)
{
	CUI_IdentityGN_MainFrame* pInstance = new CUI_IdentityGN_MainFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentityGN_MainFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentityGN_MainFrame::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
