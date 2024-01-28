#include "stdafx.h"
#include "..\Public\IT_WR_WP_Legend.h"
#include "Player.h"

CIT_WR_WP_Legend::CIT_WR_WP_Legend(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem(pDevice, pContext)
{
}

CIT_WR_WP_Legend::CIT_WR_WP_Legend(const CIT_WR_WP_Legend& rhs)
	: CItem(rhs)
{
}

HRESULT CIT_WR_WP_Legend::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIT_WR_WP_Legend::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iItemGrade = (_uint)GRADE::RED;
	m_iItemType = (_uint)TYPE::EQUIP;
	m_iEquipType = (_uint)PART::WEAPON;

	m_strObjectTag = TEXT("IT_WR_WP_Legend");
	m_strItemName = TEXT("전설의 수박 대검");
	m_strItemDescript = TEXT("");

	m_tStatChangeDesc.iAtkPower = 100;

	return S_OK;
}

void CIT_WR_WP_Legend::Tick(_float fTimeDelta)
{
}

void CIT_WR_WP_Legend::LateTick(_float fTimeDelta)
{
}

HRESULT CIT_WR_WP_Legend::Render()
{
	return S_OK;
}

HRESULT CIT_WR_WP_Legend::Use_Item(CPlayer* pOwner)
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
	Set_EquipWearing(true);
	return S_OK;
}

HRESULT CIT_WR_WP_Legend::Disuse_Item(CPlayer* pOwner, _bool bUseDefault)
{
	if (nullptr == pOwner)
		return E_FAIL;
	Set_EquipWearing(false);
	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iAtkPower -= m_tStatChangeDesc.iAtkPower;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_1))->Change_ModelCom(nullptr);
	pOwner->Set_EquipItem((_uint)PART::WEAPON, nullptr);
	pOwner->Add_Item(m_strObjectTag, this);
	return S_OK;
}

HRESULT CIT_WR_WP_Legend::Ready_Components()
{
	__super::Ready_Components();

	CModel::CHANGECOLOR pColor;
	pColor.vColor_R = Vec4(0.132107f, 0.32043f, 0.0498786f, 1.0f);
	pColor.vColor_G = Vec4(0.795551f, 0.103702f, 0.0948806f, 1.0f);
	pColor.vColor_B = Vec4();

	wstring strComName = L"Prototype_Component_Model_WR_WP_Legend";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom, &pColor)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Slayer_Legend_Weapon"),
		TEXT("Com_Texture"), (CComponent**)&m_pItemTextureCom)))
		return E_FAIL;

	return S_OK;
}

CIT_WR_WP_Legend* CIT_WR_WP_Legend::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIT_WR_WP_Legend* pInstance = new CIT_WR_WP_Legend(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CIT_WR_WP_Legend");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIT_WR_WP_Legend::Clone(void* pArg)
{
	CIT_WR_WP_Legend* pInstance = new CIT_WR_WP_Legend(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CIT_WR_WP_Legend");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIT_WR_WP_Legend::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pItemTextureCom);
}
