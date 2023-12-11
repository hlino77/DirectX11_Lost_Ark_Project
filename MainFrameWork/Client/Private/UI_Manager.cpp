#include "stdafx.h"
#include "GameInstance.h"
#include "UI_Manager.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{
}

HRESULT CUI_Manager::Tick(_float fTimeDelta)
{
	return E_NOTIMPL;
}

HRESULT CUI_Manager::LateTick(_float fTimeDelta)
{
	return E_NOTIMPL;
}

void CUI_Manager::Clear(_uint iLevelIndex)
{
}

void CUI_Manager::Render()
{
}

HRESULT CUI_Manager::Draw_UI(LEVELID eLevelId)
{
	return E_NOTIMPL;
}

HRESULT CUI_Manager::Ready_NextLevel_UI(_uint iLevelId)
{
	return E_NOTIMPL;
}

void CUI_Manager::Free()
{
}
