#include "stdafx.h"
#include "Item_TestItem.h"
#include "Player.h"
#include "GameInstance.h"

CItem_TestItem::CItem_TestItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem(pDevice, pContext)
{
}

CItem_TestItem::CItem_TestItem(const CItem& rhs)
	: CItem(rhs)
{
}

HRESULT CItem_TestItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CItem_TestItem::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_iItemGrade = (_uint)GRADE::RED;
	m_iItemType = (_uint)TYPE::CONSUM;

	m_strObjectTag = TEXT("IT_TestItem");
	m_strItemName = TEXT("테스트용 아이템");
	m_strItemDescript = TEXT("이것은 테스트용 아이템입니다. \n 테스트용");

	m_tStatChangeDesc.iHp = 10000;
	m_tStatChangeDesc.iAtkPower = 10000;

	return S_OK;
}

void CItem_TestItem::Tick(_float fTimeDelta)
{
}

void CItem_TestItem::LateTick(_float fTimeDelta)
{
}

HRESULT CItem_TestItem::Use_Item(CPlayer* pOwner)
{
	if (nullptr == pOwner)
		return E_FAIL;

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iMaxHp += m_tStatChangeDesc.iHp;
	tPcStat.iCurHp += m_tStatChangeDesc.iHp;

	tPcStat.iAtkPower += m_tStatChangeDesc.iAtkPower;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	return S_OK;
}

HRESULT CItem_TestItem::Disuse_Item(CPlayer* pOwner, _bool bUseDefault)
{
	return S_OK;
}

HRESULT CItem_TestItem::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Test_Item"),
		TEXT("Com_Texture"), (CComponent**)&m_pItemTextureCom)))
		return E_FAIL;

	return S_OK;
}

CItem_TestItem* CItem_TestItem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItem_TestItem* pInstance = new CItem_TestItem(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CItem_TestItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_TestItem::Clone(void* pArg)
{
	CItem_TestItem* pInstance = new CItem_TestItem(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CItem_TestItem");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_TestItem::Free()
{
	__super::Free();
	Safe_Release(m_pItemTextureCom);
}
