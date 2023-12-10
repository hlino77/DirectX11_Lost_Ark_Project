#include "QuadTreeMgr.h"
#include "GameObject.h"
#include "QuadTreeNode.h"
#include "ColliderSphere.h"

IMPLEMENT_SINGLETON(CQuadTreeMgr);


CQuadTreeMgr::CQuadTreeMgr()
{
}


HRESULT CQuadTreeMgr::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	Safe_AddRef(m_pDevice);

	m_pContext = pContext;
	Safe_AddRef(m_pContext);


	return S_OK;
}

HRESULT CQuadTreeMgr::Make_QaudTree(Vec3 vPos, Vec3 vScale, _uint iMaxDepth)
{
	WRITE_LOCK
	m_pSuperNode = new CQuadTreeNode(0, vPos, vScale);

	m_pSuperNode->Make_Child(iMaxDepth);

	return S_OK;
}

HRESULT CQuadTreeMgr::Reset_QaudTree()
{
	WRITE_LOCK
	Safe_Delete(m_pSuperNode);
	return S_OK;
}

_bool CQuadTreeMgr::Add_Object(CSphereCollider* pCollider)
{
	return m_pSuperNode->Add_Object(pCollider);
}

void CQuadTreeMgr::Tick(_float fTimeDelta)
{
	READ_LOCK
	if(m_pSuperNode)
		m_pSuperNode->Tick(fTimeDelta);
}



void CQuadTreeMgr::Free()
{
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	Safe_Delete(m_pSuperNode);
}
