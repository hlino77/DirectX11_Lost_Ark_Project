#include "stdafx.h"
#include "..\Public\IT_SP_Leg_Legend.h"
#include "Player.h"

CIT_SP_Leg_Legend::CIT_SP_Leg_Legend(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem(pDevice, pContext)
{
}

CIT_SP_Leg_Legend::CIT_SP_Leg_Legend(const CIT_SP_Leg_Legend& rhs)
	: CItem(rhs)
{
}

HRESULT CIT_SP_Leg_Legend::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIT_SP_Leg_Legend::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iItemGrade = (_uint)GRADE::RED;
	m_iItemType = (_uint)TYPE::EQUIP;

	m_strObjectTag = TEXT("IT_SP_Leg_Legend");
	m_strItemName = TEXT("전설의 하의");
	m_strItemDescript = TEXT("");

	m_iEquipType = (_uint)PART::LEG;

	m_vecUseEquipSlot.push_back((_uint)PART::LEG);

	m_tStatChangeDesc.iHp = 100;

	return S_OK;
}

void CIT_SP_Leg_Legend::Tick(_float fTimeDelta)
{
}

void CIT_SP_Leg_Legend::LateTick(_float fTimeDelta)
{
}

HRESULT CIT_SP_Leg_Legend::Render()
{
	return S_OK;
}

HRESULT CIT_SP_Leg_Legend::Use_Item(CPlayer* pOwner)
{
	if (nullptr == pOwner)
		return E_FAIL;

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iMaxHp += m_tStatChangeDesc.iHp;
	tPcStat.iCurHp += m_tStatChangeDesc.iHp;

	pOwner->Set_PlayerStat_Desc(tPcStat);
	/*if (nullptr != pOwner->Get_EquipItem(m_vecUseEquipSlot.front()))
	{
		pOwner->Get_EquipItem(m_vecUseEquipSlot.front())->Disuse_Item(pOwner, false);
	}*/
	for (auto& i : m_vecUseEquipSlot)
	{
		if (nullptr != pOwner->Get_EquipItem(i))
		{
			pOwner->Get_EquipItem(i)->Disuse_Item(pOwner, false);
		}
		pOwner->Set_EquipItem(i, this);

		if (i == m_iEquipType)
		{
			pOwner->Set_ModelPart(i, m_pModelCom);
		}
		else
		{
			pOwner->Set_ModelPart(i, nullptr);
		}
	}
	Set_EquipWearing(true);
	return S_OK;
}

HRESULT CIT_SP_Leg_Legend::Disuse_Item(CPlayer* pOwner, _bool bUseDefault)
{
	if (nullptr == pOwner)
		return E_FAIL;
	Set_EquipWearing(false);
	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iMaxHp -= m_tStatChangeDesc.iHp;
	tPcStat.iCurHp -= m_tStatChangeDesc.iHp;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	for (auto& i : m_vecUseEquipSlot)
	{
		pOwner->Set_ModelPart(i, pOwner->Get_DefaultPart(i));
		pOwner->Set_EquipItem(i, nullptr);
	}
	pOwner->Add_Item(m_strObjectTag, this);
	return S_OK;
}

HRESULT CIT_SP_Leg_Legend::Ready_Components()
{
	__super::Ready_Components();

	CModel::CHANGECOLOR pChangeColor;
	pChangeColor.vColor_R = Vec4(0.701758f, 0.701758f, 0.701758f, 1.f);
	pChangeColor.vColor_G = Vec4(0.331479f, 0.281819f, 0.254578f, 1.f);
	pChangeColor.vColor_B = Vec4(0.914331f, 0.914331f, 0.914331f, 1.f);

	wstring strComName = L"Prototype_Component_Model_SP_Leg_Legend";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom, &pChangeColor)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Gunslinger_Legend_Leg"),
		TEXT("Com_Texture"), (CComponent**)&m_pItemTextureCom)))
		return E_FAIL;

	return S_OK;
}

CIT_SP_Leg_Legend* CIT_SP_Leg_Legend::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIT_SP_Leg_Legend* pInstance = new CIT_SP_Leg_Legend(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CIT_SP_Leg_Legend");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIT_SP_Leg_Legend::Clone(void* pArg)
{
	CIT_SP_Leg_Legend* pInstance = new CIT_SP_Leg_Legend(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CIT_SP_Leg_Legend");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIT_SP_Leg_Legend::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pItemTextureCom);
}
