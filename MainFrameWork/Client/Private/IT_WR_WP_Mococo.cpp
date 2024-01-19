#include "stdafx.h"
#include "..\Public\IT_WR_WP_Mococo.h"
#include "Player.h"

CIT_WR_WP_Mococo::CIT_WR_WP_Mococo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem(pDevice, pContext)
{
}

CIT_WR_WP_Mococo::CIT_WR_WP_Mococo(const CIT_WR_WP_Mococo& rhs)
	: CItem(rhs)
{
}

HRESULT CIT_WR_WP_Mococo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIT_WR_WP_Mococo::Initialize(void* pArg)
{
	m_iItemGrade = (_uint)GRADE::PURPLE;
	m_iItemType = (_uint)TYPE::EQUIP;

	m_strObjectTag = TEXT("IT_GN_Body_Mococo");
	m_strItemName = TEXT("¸ðÄÚÄÚ ¸öÅë");
	m_strItemDescript = TEXT("");


	m_tStatChangeDesc.iHp = 100;

	return S_OK;
}

void CIT_WR_WP_Mococo::Tick(_float fTimeDelta)
{
}

void CIT_WR_WP_Mococo::LateTick(_float fTimeDelta)
{
}

HRESULT CIT_WR_WP_Mococo::Render()
{
	return S_OK;
}

HRESULT CIT_WR_WP_Mococo::Use_Item(CPlayer* pOwner)
{
	if (nullptr == pOwner)
		return E_FAIL;

	if (nullptr != pOwner->Get_EquipItem((_uint)PART::BODY))
	{
		pOwner->Get_EquipItem((_uint)PART::BODY)->Disuse_Item(pOwner, false);
	}

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iMaxHp += m_tStatChangeDesc.iHp;
	tPcStat.iCurHp += m_tStatChangeDesc.iHp;

	pOwner->Set_PlayerStat_Desc(tPcStat);
	pOwner->Set_ModelPart((_uint)PART::BODY, m_pModelCom);


	pOwner->Set_EquipItem((_uint)PART::BODY, this);

	if (nullptr != pOwner->Get_EquipItem((_uint)PART::SHOULDER))
	{
		pOwner->Get_EquipItem((_uint)PART::SHOULDER)->Disuse_Item(pOwner, false);
	}
	if (nullptr != pOwner->Get_EquipItem((_uint)PART::ARM))
	{
		pOwner->Get_EquipItem((_uint)PART::ARM)->Disuse_Item(pOwner, false);
	}
	if (nullptr != pOwner->Get_EquipItem((_uint)PART::LEG))
	{
		pOwner->Get_EquipItem((_uint)PART::LEG)->Disuse_Item(pOwner, false);
	}

	return S_OK;
}

HRESULT CIT_WR_WP_Mococo::Disuse_Item(CPlayer* pOwner, _bool bUseDefault)
{
	if (nullptr == pOwner)
		return E_FAIL;

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iMaxHp -= m_tStatChangeDesc.iHp;
	tPcStat.iCurHp -= m_tStatChangeDesc.iHp;

	pOwner->Set_PlayerStat_Desc(tPcStat);
	pOwner->Set_ModelPart((_uint)PART::BODY, nullptr);

	if (true == bUseDefault)
		pOwner->Set_ModelPart((_uint)PART::BODY, pOwner->Get_DefaultPart((_uint)PART::BODY));


	pOwner->Set_EquipItem((_uint)PART::BODY, nullptr);

	return S_OK;
}

HRESULT CIT_WR_WP_Mococo::Ready_Components()
{
	__super::Ready_Components();

	wstring strComName = L"Prototype_Component_Model_WR_Head_Mococo";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Helmet"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/*if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GN_Body_Mococo"),
		TEXT("Com_Texture"), (CComponent**)&m_pItemTextureCom)))
		return E_FAIL;*/

	return S_OK;
}

CIT_WR_WP_Mococo* CIT_WR_WP_Mococo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

CGameObject* CIT_WR_WP_Mococo::Clone(void* pArg)
{
	return nullptr;
}

void CIT_WR_WP_Mococo::Free()
{
}
