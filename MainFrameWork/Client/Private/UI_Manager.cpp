#include "stdafx.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{
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
	m_pUIList[iLevelIndex].clear();
}

void CUI_Manager::Render()
{

}

HRESULT CUI_Manager::Add_UI(LEVELID eLevelIndex, CUI* pUI)
{
	m_pUIList[eLevelIndex].push_back(pUI);

	return S_OK;
}

HRESULT CUI_Manager::Loading_UI(LEVELID eLevelIndex, const _uint& iLayerType, const wstring& UITag)
{
	CUI* pUI = Find_UI(eLevelIndex, iLayerType, UITag);
	if (nullptr == pUI)
		return E_FAIL;

	return S_OK;
}

CUI* CUI_Manager::Find_UI(LEVELID eLevelIndex, const _uint & iLayerType,const wstring& UITag)
{
	CUI* pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Find_GameObejct(eLevelIndex, iLayerType,UITag));
	if (nullptr != pUI)
	{
		auto iter = find(m_pUIList[eLevelIndex].begin(), m_pUIList[eLevelIndex].end(), pUI);
		if (nullptr != *iter)
			return *iter;
	}

	return nullptr;
}

void CUI_Manager::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pUIList);
}
