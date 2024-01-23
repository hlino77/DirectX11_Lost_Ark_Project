#include "stdafx.h"
#include "..\Public\IT_GN_WP_Mococo.h"
#include "Player.h"

CIT_GN_WP_Mococo::CIT_GN_WP_Mococo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem(pDevice, pContext)
{
}

CIT_GN_WP_Mococo::CIT_GN_WP_Mococo(const CIT_GN_WP_Mococo& rhs)
	: CItem(rhs)
{
}

HRESULT CIT_GN_WP_Mococo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIT_GN_WP_Mococo::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iItemGrade = (_uint)GRADE::PURPLE;
	m_iItemType = (_uint)TYPE::EQUIP;

	m_strObjectTag = TEXT("IT_GN_WP_Mococo");
	m_strItemName = TEXT("¸ðÄÚÄÚ ÃÑ");
	m_strItemDescript = TEXT("");

	m_tStatChangeDesc.iAtkPower = 100;

	return S_OK;
}

void CIT_GN_WP_Mococo::Tick(_float fTimeDelta)
{
}

void CIT_GN_WP_Mococo::LateTick(_float fTimeDelta)
{
}

HRESULT CIT_GN_WP_Mococo::Render()
{
	return S_OK;
}

HRESULT CIT_GN_WP_Mococo::Use_Item(CPlayer* pOwner)
{
	if (nullptr == pOwner)
		return E_FAIL;

	if (nullptr != pOwner->Get_EquipItem((_uint)PART::WEAPON))
		pOwner->Get_EquipItem((_uint)PART::WEAPON)->Disuse_Item(pOwner, false);

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iAtkPower += m_tStatChangeDesc.iAtkPower;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_1))->Change_ModelCom(m_pModel_H);
	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_2))->Change_ModelCom(m_pModel_H);
	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_3))->Change_ModelCom(m_pModel_L);
	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_4))->Change_ModelCom(m_pModel_S);
	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_5))->Change_ModelCom(m_pModel_S);

	pOwner->Set_EquipItem((_uint)PART::WEAPON, this);

	return S_OK;
}

HRESULT CIT_GN_WP_Mococo::Disuse_Item(CPlayer* pOwner, _bool bUseDefault)
{
	if (nullptr == pOwner)
		return E_FAIL;

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iAtkPower -= m_tStatChangeDesc.iAtkPower;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_1))->Change_ModelCom(nullptr);
	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_2))->Change_ModelCom(nullptr);
	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_3))->Change_ModelCom(nullptr);
	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_4))->Change_ModelCom(nullptr);
	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_5))->Change_ModelCom(nullptr);

	pOwner->Set_EquipItem((_uint)PART::WEAPON, nullptr);

	return S_OK;
}

HRESULT CIT_GN_WP_Mococo::Ready_Components()
{
	__super::Ready_Components();

	wstring strComName = L"Prototype_Component_Model_GN_WP_Hand_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Hand"), (CComponent**)&m_pModel_H)))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_GN_WP_Long_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Long"), (CComponent**)&m_pModel_L)))
		return E_FAIL;

	strComName = L"Prototype_Component_Model_GN_WP_Shot_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Shot"), (CComponent**)&m_pModel_S)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Gunslinger_Mococo_Weapon"),
		TEXT("Com_Texture"), (CComponent**)&m_pItemTextureCom)))
		return E_FAIL;

	return S_OK;
}

CIT_GN_WP_Mococo* CIT_GN_WP_Mococo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIT_GN_WP_Mococo* pInstance = new CIT_GN_WP_Mococo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CIT_GN_WP_Mococo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIT_GN_WP_Mococo::Clone(void* pArg)
{
	CIT_GN_WP_Mococo* pInstance = new CIT_GN_WP_Mococo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CIT_GN_WP_Mococo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIT_GN_WP_Mococo::Free()
{
	__super::Free();

	Safe_Release(m_pModel_H);
	Safe_Release(m_pModel_S);
	Safe_Release(m_pModel_L);
	Safe_Release(m_pItemTextureCom);
}
