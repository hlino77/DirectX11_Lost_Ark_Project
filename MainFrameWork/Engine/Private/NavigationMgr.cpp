#include "NavigationMgr.h"
#include "Navigation.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CNavigationMgr);


CNavigationMgr::CNavigationMgr()
{
}


HRESULT CNavigationMgr::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	Safe_AddRef(m_pDevice);

	m_pContext = pContext;
	Safe_AddRef(m_pContext);


	return S_OK;
}



void CNavigationMgr::Render(_uint iLevel)
{
	WRITE_LOCK
	if (m_bRender)
	{
		if (m_Navigations.find(iLevel) == m_Navigations.end())
			return;
		m_Navigations[iLevel]->Render();
	}
}

void CNavigationMgr::Add_Navigation(_uint iLevel, const wstring& szFileName)
{
	WRITE_LOCK
	if (m_Navigations.find(iLevel) == m_Navigations.end())
	{

		CNavigation* pNavigation = CNavigation::Create(m_pDevice, m_pContext);

		pNavigation->Load_Navigation(szFileName);

		m_Navigations.emplace(iLevel, pNavigation);

	}
	
}

void CNavigationMgr::SetUp_OnCell(_uint iLevel, CGameObject* pObject)
{
	if (m_Navigations.find(iLevel) == m_Navigations.end())
		return;

	m_Navigations[iLevel]->SetUp_OnCell(pObject);
}

_float CNavigationMgr::Get_Height_OnCell(_uint iLevel, CGameObject* pObject)
{
	if (m_Navigations.find(iLevel) == m_Navigations.end())
		return 0.f;

	return	m_Navigations[iLevel]->Get_Height_OnCell(pObject);
}

_bool CNavigationMgr::Is_Outside(_uint iLevel, CGameObject* pObject, _float fOffset)
{
	if (m_Navigations.find(iLevel) == m_Navigations.end())
		return false;

	return	m_Navigations[iLevel]->Is_Outside(pObject,fOffset);
}

void CNavigationMgr::Find_FirstCell(_uint iLevel, CGameObject* pObject)
{
	if (m_Navigations.find(iLevel) == m_Navigations.end())
		return;

	m_Navigations[iLevel]->Find_FirstCell(pObject);
}

_bool CNavigationMgr::Picking_Cell(_uint iLevel, Vec3 vRayPos, Vec3 vRayDir, Vec3& vResultPos)
{
	if (m_Navigations.find(iLevel) == m_Navigations.end())
		return false;

	vRayDir.Normalize();

	_bool bPick = false;
	_float fDist = -1.0f;

	if (m_Navigations[iLevel]->Picking_Cell(vRayPos, vRayDir, fDist))
	{
		vResultPos = vRayPos + (vRayDir * fDist);
		bPick = true;
	}
	return bPick;
}

void CNavigationMgr::Set_NaviCell_Active(_uint LevelIndex, _uint CellIndex, _bool Active)
{
	m_Navigations[LevelIndex]->Set_Active_Specific_Cell(CellIndex, Active);
}

CCell* CNavigationMgr::Get_CurCell(_uint iLevel, CGameObject* pObject)
{
	return m_Navigations[iLevel]->Find_Cell(pObject->Get_CurrCell());
}

_bool CNavigationMgr::Is_NeighborActive(_uint iLevel, CGameObject* pObject)
{
	return m_Navigations[iLevel]->Is_NeighborActive(pObject);
}

Vec3 CNavigationMgr::Find_CloseCell_Middle(_uint iLevel, CGameObject* pObject)
{
	return m_Navigations[iLevel]->Find_CloseCell_Middle(pObject);
}

void CNavigationMgr::Set_AllNaviCell_Active(_uint LevelIndex, _bool bActive)
{
	m_Navigations[LevelIndex]->Set_Active_All_Cell(bActive);
}


void CNavigationMgr::Free()
{
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
