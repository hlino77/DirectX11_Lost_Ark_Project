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



void CNavigationMgr::Render()
{
	WRITE_LOCK
	if (m_pNavigation)
		m_pNavigation->Render();
}

void CNavigationMgr::Add_Navigation(const wstring& szFileName)
{
	WRITE_LOCK
	m_pNavigation = CNavigation::Create(m_pDevice, m_pContext);

	m_pNavigation->Load_Navigation(szFileName);
}

void CNavigationMgr::SetUp_OnCell(CGameObject* pObject)
{
	if (m_pNavigation)
		m_pNavigation->SetUp_OnCell(pObject);
}

void CNavigationMgr::Find_FirstCell(CGameObject* pObject)
{
	if (m_pNavigation)
		m_pNavigation->Find_FirstCell(pObject);
}

_int CNavigationMgr::Check_Pos_InCell(Vec3 vPos)
{
	if (m_pNavigation == nullptr)
		return -1;

	return m_pNavigation->Check_Pos_InCell(vPos);
}

void CNavigationMgr::Reset_Navigation()
{
	WRITE_LOCK;
	Safe_Release(m_pNavigation);
	m_pNavigation = nullptr;
}





void CNavigationMgr::Free()
{
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
