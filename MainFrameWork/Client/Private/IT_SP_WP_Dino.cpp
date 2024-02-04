#include "stdafx.h"
#include "..\Public\IT_SP_WP_Dino.h"
#include "Player.h"

CIT_SP_WP_Dino::CIT_SP_WP_Dino(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem(pDevice, pContext)
{
}

CIT_SP_WP_Dino::CIT_SP_WP_Dino(const CIT_SP_WP_Dino& rhs)
	: CItem(rhs)
{
}

HRESULT CIT_SP_WP_Dino::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIT_SP_WP_Dino::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iItemGrade = (_uint)GRADE::PURPLE;
	m_iItemType = (_uint)TYPE::EQUIP;
	m_iEquipType = (_uint)PART::WEAPON;

	m_strObjectTag = TEXT("IT_SP_WP_Dino");
	m_strItemName = TEXT("´ÙÀÌ³ë º×");
	m_strItemDescript = TEXT("");

	m_tStatChangeDesc.iAtkPower = 100;

	return S_OK;
}

void CIT_SP_WP_Dino::Tick(_float fTimeDelta)
{
}

void CIT_SP_WP_Dino::LateTick(_float fTimeDelta)
{
}

HRESULT CIT_SP_WP_Dino::Render()
{
	return S_OK;
}

HRESULT CIT_SP_WP_Dino::Use_Item(CPlayer* pOwner)
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

HRESULT CIT_SP_WP_Dino::Disuse_Item(CPlayer* pOwner, _bool bUseDefault)
{
	if (nullptr == pOwner)
		return E_FAIL;
	Set_EquipWearing(false);
	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iAtkPower -= m_tStatChangeDesc.iAtkPower;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	static_cast<CPartObject*>(pOwner->Get_Parts(CPartObject::PARTS::WEAPON_1))->Change_ModelCom(nullptr);
	pOwner->Set_EquipItem((_uint)PART::WEAPON, nullptr);
	if (pOwner->Is_Control())
		pOwner->Add_Item(m_strObjectTag, this);
	return S_OK;
}

HRESULT CIT_SP_WP_Dino::Ready_Components()
{
	__super::Ready_Components();

	CModel::CHANGECOLOR pChangeColor;
	pChangeColor.vColor_R = Vec4(0.541417f, 0.376768f, 0.191453f, 1.f);
	pChangeColor.vColor_G = Vec4(0.132096f, 0.0989472f, 0.0617371f, 1.f);
	pChangeColor.vColor_B = Vec4(1.f, 1.f, 1.f, 1.f);

	wstring strComName = L"Prototype_Component_Model_SP_WP_Dino";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom, &pChangeColor)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Artist_Dinyong_Weapon"),
		TEXT("Com_Texture"), (CComponent**)&m_pItemTextureCom)))
		return E_FAIL;

	return S_OK;
}

CIT_SP_WP_Dino* CIT_SP_WP_Dino::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIT_SP_WP_Dino* pInstance = new CIT_SP_WP_Dino(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CIT_SP_WP_Dino");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIT_SP_WP_Dino::Clone(void* pArg)
{
	CIT_SP_WP_Dino* pInstance = new CIT_SP_WP_Dino(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CIT_SP_WP_Dino");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIT_SP_WP_Dino::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pItemTextureCom);
}
