#include "stdafx.h"
#include "UI_Monster_Hp.h"
#include "GameInstance.h"
#include "PipeLine.h"
#include "TextBox.h"
#include "ServerSessionManager.h"
#include "Monster.h"

CUI_Monster_Hp::CUI_Monster_Hp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Monster_Hp::CUI_Monster_Hp(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Monster_Hp::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Monster_Hp::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("Monster_HpUI");

	m_fSizeX = 87.f;
	m_fSizeY = 12.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.3f));

	_float fSizeX = 80.f;
	_float fSizeY = 7.0f;
	m_pTransform_Hp->Set_Scale(Vec3(fSizeX, fSizeY, 1.f));
	m_pTransform_Hp->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.3f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if (nullptr != pArg)
	{
		m_pOwner = static_cast<CGameObject*>(pArg);
		m_iMaxHp = (_uint)dynamic_cast<CMonster*>(m_pOwner)->Get_MaxHp();
		m_iCurrHp = (_int)dynamic_cast<CMonster*>(m_pOwner)->Get_Hp();
		m_fHpRatio = (_float)m_iCurrHp / (_float)m_iMaxHp;

		if (nullptr != m_pOwner)
		{
			Vec3 vHostPos = m_pOwner->Get_EffectPos();

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				Vec3(vHostPos.x * g_iWinSizeX * 0.5f, vHostPos.y * g_iWinSizeY * 0.5f, 0.3f));

			m_pTransform_Hp->Set_State(CTransform::STATE_POSITION,
				Vec3(vHostPos.x * g_iWinSizeX * 0.5f, vHostPos.y * g_iWinSizeY * 0.5f, 0.3f));
		}
	}

	return S_OK;
}

void CUI_Monster_Hp::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Monster_Hp::LateTick(_float fTimeDelta)
{
	m_iCurrHp = (_int)dynamic_cast<CMonster*>(m_pOwner)->Get_Hp();
	m_fHpRatio = (_float)m_iCurrHp / (_float)m_iMaxHp;
	if ((static_cast<CMonster*>(m_pOwner)->Is_Render()) && (!static_cast<CMonster*>(m_pOwner)->Is_Culled()))
	{
		if (0.f < m_fHpRatio)
			m_bRender = true;
		else
			m_bRender = false;
	}
	Update_Postion();
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Monster_Hp::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	if (TEXT("Monster_Prison") != m_pOwner->Get_ObjectTag())
	{
		if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom_PrisonWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Hp()))
		return E_FAIL;
	if (TEXT("Monster_Prison") != m_pOwner->Get_ObjectTag())
	{
		if (FAILED(m_pTextureCom_Hp->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom_PrisonGauge->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	m_pShaderCom->Begin(16);
	m_pVIBufferCom->Render();
	
	return S_OK;
}

HRESULT CUI_Monster_Hp::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;
	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_HpFrame"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Hp"),
		TEXT("Com_TextureHp"), (CComponent**)&m_pTextureCom_Hp)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_GroggyWnd"),
		TEXT("Com_TexturePrisonWnd"), (CComponent**)&m_pTextureCom_PrisonWnd)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_GroggyFill"),
		TEXT("Com_PrisonGauge"), (CComponent**)&m_pTextureCom_PrisonGauge)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_TransformHp"), (CComponent**)&m_pTransform_Hp)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_Hp::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Monster_Hp::Bind_ShaderResources_Hp()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Hp->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fHpRatio, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CUI_Monster_Hp::Update_Postion()
{
	if (nullptr != m_pOwner)
	{
		Vec3 vHostPos = m_pOwner->Get_EffectPos();

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(vHostPos.x * g_iWinSizeX * 0.5f, vHostPos.y * g_iWinSizeY * 0.5f, 0.3f));

		m_pTransform_Hp->Set_State(CTransform::STATE_POSITION,
			Vec3(vHostPos.x * g_iWinSizeX * 0.5f, vHostPos.y * g_iWinSizeY * 0.5f, 0.3f));

		if (((-1.f <= vHostPos.x) && (1.f >= vHostPos.x)) && ((-1.f <= vHostPos.y) && (1.f >= vHostPos.y)) && ((0.f <= vHostPos.z) && (1.f >= vHostPos.z)))
			m_bRender = true;
		else
			m_bRender = false;
	}
}

CUI_Monster_Hp* CUI_Monster_Hp::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Monster_Hp* pInstance = new CUI_Monster_Hp(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Monster_Hp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Monster_Hp::Clone(void* pArg)
{
	CUI_Monster_Hp* pInstance = new CUI_Monster_Hp(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Monster_Hp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Monster_Hp::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	
	Safe_Release(m_pTextureCom_Hp);
	Safe_Release(m_pTextureCom_PrisonWnd);
	Safe_Release(m_pTextureCom_PrisonGauge);
}
