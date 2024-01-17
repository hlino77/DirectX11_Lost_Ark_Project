#include "stdafx.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI.h"
#include "UI_Loading.h"
#include "UI_Boss_Hp.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{
}

HRESULT CUI_Manager::Reserve_Manager()
{
	m_pUIList = new  list<class CUI*>[(_uint)LEVELID::LEVEL_END];

	return S_OK;
}

HRESULT CUI_Manager::Tick(_float fTimeDelta)
{
	return  S_OK;
}

HRESULT CUI_Manager::LateTick(_float fTimeDelta)
{
	return  S_OK;
}

void CUI_Manager::Clear(LEVELID iLevelIndex)
{
	WRITE_LOCK
	m_pUIList[iLevelIndex].clear();
}

void CUI_Manager::Render()
{

}

HRESULT CUI_Manager::Add_UI(LEVELID eLevelIndex, CUI* pUI)
{
	WRITE_LOCK
	auto iter = find(m_pUIList[eLevelIndex].begin(), m_pUIList[eLevelIndex].end(), pUI);
	if (iter != m_pUIList[eLevelIndex].end())
		return E_FAIL;
	
	m_pUIList[eLevelIndex].push_back(pUI);

	return S_OK;
}

HRESULT CUI_Manager::ObjectManager_to_UIManager(LEVELID eLevelIndex)
{
	WRITE_LOCK
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	m_pUIList[eLevelIndex].clear();

	vector<CGameObject*>pUIList = pGameInstance->Find_GameObjects(eLevelIndex, (_uint)LAYER_TYPE::LAYER_UI);
	for (auto iter : pUIList)
	{
		auto pUI = find(m_pUIList[eLevelIndex].begin(), m_pUIList[eLevelIndex].end(), static_cast<CUI*>(iter));
		if (pUI == m_pUIList[eLevelIndex].end())
			m_pUIList[eLevelIndex].push_back(static_cast<CUI*>(iter));
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CUI_Manager::Loading_UI(_float fSizeX)
{
	CUI* pUI = Find_UI(LEVEL_LOADING, TEXT("UI_Loading"));
	if (nullptr == pUI)
		return E_FAIL;
	static_cast<CUI_Loading*>(pUI)->Change_LoadingPer(fSizeX);

	return S_OK;
}

HRESULT CUI_Manager::Set_MaxFiles(_uint iFilesNum)
{
	CUI* pUI = Find_UI(LEVEL_LOADING, TEXT("UI_Loading"));
	if (nullptr == pUI)
		return E_FAIL;
	static_cast<CUI_Loading*>(pUI)->Set_MaxFiles(iFilesNum);

	return S_OK;
}

HRESULT CUI_Manager::Add_CurrFile()
{
	CUI* pUI = Find_UI(LEVEL_LOADING, TEXT("UI_Loading"));
	if (nullptr == pUI)
		return E_FAIL;
	static_cast<CUI_Loading*>(pUI)->Add_CurrFile();

	return S_OK;
}

HRESULT CUI_Manager::Add_CurrFiles(_uint iFilesNum)
{
	CUI* pUI = Find_UI(LEVEL_LOADING, TEXT("UI_Loading"));
	if (nullptr == pUI)
		return E_FAIL;
	static_cast<CUI_Loading*>(pUI)->Add_CurrFiles(iFilesNum);

	return S_OK;
}

CUI* CUI_Manager::Find_UI(LEVELID eLevelIndex, const wstring& UITag)
{
	WRITE_LOCK
	for (auto& iter : m_pUIList[eLevelIndex])
	{
		if (UITag == iter->Get_UITag())
				return iter;
	}

	return nullptr;
}

list<class CUI*>* CUI_Manager::Get_UIList(LEVELID eLevelIndex)
{
	return &m_pUIList[eLevelIndex];
}

vector<CUI*> CUI_Manager::Find_UIParts(LEVELID eLevelIndex, const wstring& UITag)
{
	CUI* pUI = 	Find_UI(eLevelIndex, UITag);
	pUI->Get_UIParts();

	return pUI->Get_UIParts();
}

CUI* CUI_Manager::Find_UIPart(LEVELID eLevelIndex, const wstring& UITag, const wstring& PartTag)
{
	CUI* pUI = Find_UI(eLevelIndex, UITag);
	if (nullptr == pUI)
		return nullptr;

	return pUI->Get_UIPart(PartTag);
}

HRESULT CUI_Manager::Delete_UI(LEVELID eLevelIndex, const wstring& UITag)
{
	WRITE_LOCK
	CUI* pUI = Find_UI(eLevelIndex, UITag);
	if (nullptr == pUI)
		return E_FAIL;

	Safe_Release(pUI);

	return S_OK;
}

HRESULT CUI_Manager::Delete_UIPart(LEVELID eLevelIndex, const wstring& UITag, const wstring& PartTag)
{
	WRITE_LOCK
	CUI* pUI = Find_UI(eLevelIndex, UITag);
	if (nullptr != pUI)
	{
		CUI* pPart = pUI->Get_UIPart(PartTag);
		if (nullptr != pPart)
			Safe_Release(pPart);

		return S_OK;
	}
	else
		return E_FAIL;
}

void CUI_Manager::Sorting_UI()
{
}

void CUI_Manager::Set_UIState(LEVELID iLevelIndex, CUI::UISTATE eState)
{
	WRITE_LOCK
	for (auto iter : m_pUIList[iLevelIndex])
	{
		iter->Set_State(eState);
	}
}

void CUI_Manager::Set_UIState(LEVELID iLevelIndex, const wstring& strUITag, CUI::UISTATE eState)
{
	WRITE_LOCK
	for (auto iter : m_pUIList[iLevelIndex])
	{
		if (strUITag == iter->Get_UITag())
		{
			iter->Set_State(eState);
		}
	}

}

void CUI_Manager::Set_CurrHPUI(CUI* pUI)
{
	if (pUI == m_pCurrentBossHpUI)
		return;
	
	WRITE_LOCK

		if (pUI != m_pCurrentBossHpUI)
		{
			if(nullptr != m_pCurrentBossHpUI)
				static_cast<CUI_Boss_Hp*>(m_pCurrentBossHpUI)->Set_Active(false);//m_pCurrentBossHpUI->Set_Render(false);
			static_cast<CUI_Boss_Hp*>(pUI)->Set_Active(true);
			static_cast<CUI_Boss_Hp*>(pUI)->Print_BossHp();
			m_pCurrentBossHpUI = pUI;
		}
}

void CUI_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < (_uint)LEVEL_END; i++)
	{
		m_pUIList[i].clear();
	}
	Safe_Delete_Array(m_pUIList);
}
