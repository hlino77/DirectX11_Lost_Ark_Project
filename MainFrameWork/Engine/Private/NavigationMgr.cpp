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
	if (m_Navigations.find(iLevel) == m_Navigations.end())
		return;
	m_Navigations[iLevel]->Render();
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





void CNavigationMgr::Free()
{
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
