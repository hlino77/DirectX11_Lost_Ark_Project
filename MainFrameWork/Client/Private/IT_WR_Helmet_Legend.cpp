#include "stdafx.h"
#include "..\Public\IT_WR_Helmet_Legend.h"
#include "Player.h"

CIT_WR_Helmet_Legend::CIT_WR_Helmet_Legend(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CItem(pDevice, pContext)
{
}

CIT_WR_Helmet_Legend::CIT_WR_Helmet_Legend(const CIT_WR_Helmet_Legend& rhs)
	: CItem(rhs)
{
}

HRESULT CIT_WR_Helmet_Legend::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIT_WR_Helmet_Legend::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iItemGrade = (_uint)GRADE::RED;
	m_iItemType = (_uint)TYPE::EQUIP;

	m_strObjectTag = TEXT("IT_WR_Helmet_Legend");
	m_strItemName = TEXT("전설의 모자");
	m_strItemDescript = TEXT("");

	m_iEquipType = (_uint)PART::HELMET;

	m_vecUseEquipSlot.push_back((_uint)PART::HELMET);

	m_tStatChangeDesc.iHp = 100;

	return S_OK;
}

void CIT_WR_Helmet_Legend::Tick(_float fTimeDelta)
{
}

void CIT_WR_Helmet_Legend::LateTick(_float fTimeDelta)
{
}

HRESULT CIT_WR_Helmet_Legend::Render()
{
	return S_OK;
}

HRESULT CIT_WR_Helmet_Legend::Use_Item(CPlayer* pOwner)
{
	if (nullptr == pOwner)
		return E_FAIL;

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iMaxHp += m_tStatChangeDesc.iHp;
	tPcStat.iCurHp += m_tStatChangeDesc.iHp;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	for (auto& i : m_vecUseEquipSlot)
	{
		if (nullptr != pOwner->Get_EquipItem(i))
			pOwner->Get_EquipItem(i)->Disuse_Item(pOwner, false);

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

	return S_OK;
}

HRESULT CIT_WR_Helmet_Legend::Disuse_Item(CPlayer* pOwner, _bool bUseDefault)
{
	if (nullptr == pOwner)
		return E_FAIL;

	CPlayer::STATDESC tPcStat = pOwner->Get_PlayerStat_Desc();

	tPcStat.iMaxHp -= m_tStatChangeDesc.iHp;
	tPcStat.iCurHp -= m_tStatChangeDesc.iHp;

	pOwner->Set_PlayerStat_Desc(tPcStat);

	for (auto& i : m_vecUseEquipSlot)
	{
		pOwner->Set_ModelPart(i, pOwner->Get_DefaultPart(i));
		pOwner->Set_EquipItem(i, nullptr);
	}

	return S_OK;
}

HRESULT CIT_WR_Helmet_Legend::Ready_Components()
{
	__super::Ready_Components();

	CModel::CHANGECOLOR pColor;
	pColor.vColor_R = Vec4(0.62f, 0.77f, 0.99f, 1.0f);
	pColor.vColor_G = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pColor.vColor_B = Vec4(0.62f, 0.77f, 0.99f, 1.0f);

	wstring strComName = L"Prototype_Component_Model_WR_Helmet_Legend";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom, &pColor)))
		return E_FAIL;

	/*if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GN_Body_Mococo"),
		TEXT("Com_Texture"), (CComponent**)&m_pItemTextureCom)))
		return E_FAIL;*/

	return S_OK;
}

CIT_WR_Helmet_Legend* CIT_WR_Helmet_Legend::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIT_WR_Helmet_Legend* pInstance = new CIT_WR_Helmet_Legend(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CIT_WR_Helmet_Legend");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIT_WR_Helmet_Legend::Clone(void* pArg)
{
	CIT_WR_Helmet_Legend* pInstance = new CIT_WR_Helmet_Legend(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CIT_WR_Helmet_Legend");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIT_WR_Helmet_Legend::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pItemTextureCom);
}
