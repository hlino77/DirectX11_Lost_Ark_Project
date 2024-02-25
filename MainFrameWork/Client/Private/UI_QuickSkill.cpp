#include "stdafx.h"
#include "UI_QuickSkill.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_SkillIcon_Frame.h"
#include "UI_Skill_MoveIcon.h"

CUI_QuickSkill::CUI_QuickSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}
CUI_QuickSkill::CUI_QuickSkill(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_QuickSkill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_QuickSkill::Initialize(void* pArg)
{

	if (FAILED(UI_Set()))
		return E_FAIL;
	m_fSizeX = 240.f	;
	m_fSizeY = 88.f;
	m_fX = 624.f;
	m_fY = 848.f;

	m_strUITag = TEXT("UI_QuickSlot");

	return S_OK;
}

void CUI_QuickSkill::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Create_Rect();
	Picking_UI();
}

void CUI_QuickSkill::LateTick(_float fTimeDelta)
{
	if ((!m_bPick)&&(true == CUI_Manager::GetInstance()->Is_PickedIcon()))
	{
		if (KEY_AWAY(KEY::LBTN))
		{
			if (CUI_Manager::GetInstance()->Is_PickedIcon())
			{
				CUI_Manager::GetInstance()->Reset_ItemIcon();
				CUI_Manager::GetInstance()->Set_Player_Mouse(true);
			}
		}
	}
}

HRESULT CUI_QuickSkill::Render()
{
	return S_OK;
}

void CUI_QuickSkill::UI_Tick(_float fTimeDelta)
{
}

HRESULT CUI_QuickSkill::UI_Set()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI* pUI = nullptr;

	for (size_t i = 0; i < 8; i++)
	{
		pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_SkillFrame")
		, &i));
		if (nullptr != pUI)
		{
			m_vecUIParts.push_back(pUI);
		}
	}

	for (size_t i = 0; i < 4; i++)
	{
		pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_ItemFrame")
			, &i));
		if (nullptr != pUI)
		{
			m_vecUIParts.push_back(pUI);
		}
	}

	for (size_t i = 0; i < 6; i++)
	{
		pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_ETCFrame")
			, &i));
		if (nullptr != pUI)
		{
			m_vecUIParts.push_back(pUI);
		}
	}

	Load_UIData(TEXT("QuickSlot_Icon_Desc"));

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_SpaceBarIcon")
		));
	if (nullptr != pUI)
	{
		m_vecUIParts.push_back(pUI);
	}

	for (auto& iter : m_vecUIParts)
	{
		iter->Create_Rect();
		iter->Set_Z(0.2f);
	}
	                                                                            
	Safe_Release(pGameInstance);

	return S_OK;
}

CUI_QuickSkill* CUI_QuickSkill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_QuickSkill* pInstance = new CUI_QuickSkill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_QuickSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_QuickSkill::Clone(void* pArg)
{
	CUI_QuickSkill* pInstance = new CUI_QuickSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_QuickSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_QuickSkill::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
