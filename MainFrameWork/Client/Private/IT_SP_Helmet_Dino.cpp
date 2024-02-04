#include "stdafx.h"
#include "..\Public\IT_SP_Helmet_Dino.h"
#include "Player.h"

CIT_SP_Helmet_Dino::CIT_SP_Helmet_Dino(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem(pDevice, pContext)
{
}

CIT_SP_Helmet_Dino::CIT_SP_Helmet_Dino(const CIT_SP_Helmet_Dino& rhs)
	: CItem(rhs)
{
}

HRESULT CIT_SP_Helmet_Dino::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIT_SP_Helmet_Dino::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iItemGrade = (_uint)GRADE::PURPLE;
	m_iItemType = (_uint)TYPE::EQUIP;

	m_strObjectTag = TEXT("IT_SP_Helmet_Dino");
	m_strItemName = TEXT("°ø·æ ¸Ó¸®");
	m_strItemDescript = TEXT("");

	m_iEquipType = (_uint)PART::HELMET;

	m_vecUseEquipSlot.push_back((_uint)PART::HELMET);

	m_tStatChangeDesc.iHp = 100;

	return S_OK;
}

void CIT_SP_Helmet_Dino::Tick(_float fTimeDelta)
{
}

void CIT_SP_Helmet_Dino::LateTick(_float fTimeDelta)
{
}

HRESULT CIT_SP_Helmet_Dino::Render()
{
	return S_OK;
}

HRESULT CIT_SP_Helmet_Dino::Use_Item(CPlayer* pOwner)
{
	if (nullptr == pOwner)
		return E_FAIL;

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iMaxHp += m_tStatChangeDesc.iHp;
	tPcStat.iCurHp += m_tStatChangeDesc.iHp;

	pOwner->Set_PlayerStat_Desc(tPcStat);
	if (nullptr != pOwner->Get_EquipItem(m_vecUseEquipSlot.front()))
	{
		pOwner->Get_EquipItem(m_vecUseEquipSlot.front())->Disuse_Item(pOwner, false);
	}
	for (auto& i : m_vecUseEquipSlot)
	{
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

HRESULT CIT_SP_Helmet_Dino::Disuse_Item(CPlayer* pOwner, _bool bUseDefault)
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
	if (pOwner->Is_Control())
		pOwner->Add_Item(m_strObjectTag, this);
	return S_OK;
}

HRESULT CIT_SP_Helmet_Dino::Ready_Components()
{
	__super::Ready_Components();

	CModel::CHANGECOLOR pChangeColor;
	pChangeColor.vColor_R = Vec4(0.089888f * 0.679775f, 1.f * 0.679775f, 0.673617f * 0.679775f, 1.f);
	pChangeColor.vColor_G = Vec4(0.981382f, 0.2061f, 0.171599f, 1.f);
	pChangeColor.vColor_B = Vec4(1.44715f, 0.268293f, 0.34582f, 1.f);

	wstring strComName = L"Prototype_Component_Model_SP_Helmet_Dino";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom, &pChangeColor)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Artist_Dinyong_Head"),
		TEXT("Com_Texture"), (CComponent**)&m_pItemTextureCom)))
		return E_FAIL;

	return S_OK;
}

CIT_SP_Helmet_Dino* CIT_SP_Helmet_Dino::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIT_SP_Helmet_Dino* pInstance = new CIT_SP_Helmet_Dino(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CIT_SP_Helmet_Dino");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIT_SP_Helmet_Dino::Clone(void* pArg)
{
	CIT_SP_Helmet_Dino* pInstance = new CIT_SP_Helmet_Dino(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CIT_SP_Helmet_Dino");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIT_SP_Helmet_Dino::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pItemTextureCom);
}
