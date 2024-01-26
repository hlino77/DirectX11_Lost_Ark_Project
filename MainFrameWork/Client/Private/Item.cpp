#include "stdafx.h"
#include "..\Public\Item.h"
#include "GameInstance.h"

CItem::CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Item", OBJ_TYPE::ITEM)
{
}

CItem::CItem(const CItem& rhs)
	: CGameObject(rhs)
{
}

HRESULT CItem::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CItem::Tick(_float fTimeDelta)
{
}

void CItem::LateTick(_float fTimeDelta)
{
}

HRESULT CItem::Render()
{
	return S_OK;
}

void CItem::Upgrade_Item()
{
}


CTexture* CItem::Get_ItemTexture()
{
	if (nullptr != m_pItemTextureCom)
		return m_pItemTextureCom;
	else
		return nullptr;
}

void CItem::Add_UpgradeGauge(_float fGauge)
{
	if (100.f >= fGauge)
		m_fUpgradeGauge += fGauge;
	
	if (100.f < m_fUpgradeGauge)
		m_fUpgradeGauge = 100.f;
}

HRESULT CItem::Ready_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CItem::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
}
