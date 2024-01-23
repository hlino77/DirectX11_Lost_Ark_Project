#include "stdafx.h"
#include "..\Public\IT_MG_WP_Mococo.h"
#include "Player.h"

CIT_MG_WP_Mococo::CIT_MG_WP_Mococo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem(pDevice, pContext)
{
}

CIT_MG_WP_Mococo::CIT_MG_WP_Mococo(const CIT_MG_WP_Mococo& rhs)
	: CItem(rhs)
{
}

HRESULT CIT_MG_WP_Mococo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIT_MG_WP_Mococo::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iItemGrade = (_uint)GRADE::PURPLE;
	m_iItemType = (_uint)TYPE::EQUIP;

	m_strObjectTag = TEXT("IT_MG_WP_Mococo");
	m_strItemName = TEXT("모코코 하프");
	m_strItemDescript = TEXT("");

	m_tStatChangeDesc.iAtkPower = 100;

	return S_OK;
}

void CIT_MG_WP_Mococo::Tick(_float fTimeDelta)
{
}

void CIT_MG_WP_Mococo::LateTick(_float fTimeDelta)
{
}

HRESULT CIT_MG_WP_Mococo::Render()
{
	return S_OK;
}

HRESULT CIT_MG_WP_Mococo::Use_Item(CPlayer* pOwner)
{
	if (nullptr == pOwner)
		return E_FAIL;

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iAtkPower += m_tStatChangeDesc.iAtkPower;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_1))->Change_ModelCom(m_pModelCom);

	return S_OK;
}

HRESULT CIT_MG_WP_Mococo::Disuse_Item(CPlayer* pOwner, _bool bUseDefault)
{
	if (nullptr == pOwner)
		return E_FAIL;

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iAtkPower -= m_tStatChangeDesc.iAtkPower;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_1))->Change_ModelCom(nullptr);

	return S_OK;
}

HRESULT CIT_MG_WP_Mococo::Ready_Components()
{
	__super::Ready_Components();

	wstring strComName = L"Prototype_Component_Model_MG_WP_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Bard_Mococo_Weapon"),
		TEXT("Com_Texture"), (CComponent**)&m_pItemTextureCom)))
		return E_FAIL;

	return S_OK;
}

CIT_MG_WP_Mococo* CIT_MG_WP_Mococo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIT_MG_WP_Mococo* pInstance = new CIT_MG_WP_Mococo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CIT_MG_WP_Mococo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIT_MG_WP_Mococo::Clone(void* pArg)
{
	CIT_MG_WP_Mococo* pInstance = new CIT_MG_WP_Mococo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CIT_MG_WP_Mococo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIT_MG_WP_Mococo::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pItemTextureCom);
}
