#include "stdafx.h"
#include "..\Public\IT_WDR_WP_Mococo.h"
#include "Player.h"

CIT_WDR_WP_Mococo::CIT_WDR_WP_Mococo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem(pDevice, pContext)
{
}

CIT_WDR_WP_Mococo::CIT_WDR_WP_Mococo(const CIT_WDR_WP_Mococo& rhs)
	: CItem(rhs)
{
}

HRESULT CIT_WDR_WP_Mococo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIT_WDR_WP_Mococo::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iItemGrade = (_uint)GRADE::PURPLE;
	m_iItemType = (_uint)TYPE::EQUIP;

	m_strObjectTag = TEXT("IT_WDR_WP_Mococo");
	m_strItemName = TEXT("¸ðÄÚÄÚ ¸ÁÄ¡");
	m_strItemDescript = TEXT("");

	m_tStatChangeDesc.iAtkPower = 100;

	return S_OK;
}

void CIT_WDR_WP_Mococo::Tick(_float fTimeDelta)
{
}

void CIT_WDR_WP_Mococo::LateTick(_float fTimeDelta)
{
}

HRESULT CIT_WDR_WP_Mococo::Render()
{
	return S_OK;
}

HRESULT CIT_WDR_WP_Mococo::Use_Item(CPlayer* pOwner)
{
	if (nullptr == pOwner)
		return E_FAIL;

	if (nullptr != pOwner->Get_EquipItem((_uint)PART::WEAPON))
		pOwner->Get_EquipItem((_uint)PART::WEAPON)->Disuse_Item(pOwner, false);

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iAtkPower += m_tStatChangeDesc.iAtkPower;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_1))->Change_ModelCom(m_pModelCom);
	pOwner->Set_EquipItem((_uint)PART::WEAPON, this);

	return S_OK;
}

HRESULT CIT_WDR_WP_Mococo::Disuse_Item(CPlayer* pOwner, _bool bUseDefault)
{
	if (nullptr == pOwner)
		return E_FAIL;

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iAtkPower -= m_tStatChangeDesc.iAtkPower;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_1))->Change_ModelCom(nullptr);
	pOwner->Set_EquipItem((_uint)PART::WEAPON, nullptr);

	return S_OK;
}

HRESULT CIT_WDR_WP_Mococo::Ready_Components()
{
	__super::Ready_Components();

	/* For.Com_Model */
	CModel::CHANGECOLOR pColor;
	pColor.vColor_R = Vec4(1.97778f, 0.99757f, 0.127675f, 0.724719f);
	pColor.vColor_G = Vec4(0.423077f, 1.36923f, 1.24686f, 0.825843f);
	pColor.vColor_B = Vec4();

	wstring strComName = L"Prototype_Component_Model_WDR_WP_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom, &pColor)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Desrtoyer_Mococo_Weaopn"),
		TEXT("Com_Texture"), (CComponent**)&m_pItemTextureCom)))
		return E_FAIL;

	return S_OK;
}

CIT_WDR_WP_Mococo* CIT_WDR_WP_Mococo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIT_WDR_WP_Mococo* pInstance = new CIT_WDR_WP_Mococo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CIT_WDR_WP_Mococo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIT_WDR_WP_Mococo::Clone(void* pArg)
{
	CIT_WDR_WP_Mococo* pInstance = new CIT_WDR_WP_Mococo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CIT_WDR_WP_Mococo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIT_WDR_WP_Mococo::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pItemTextureCom);
}
