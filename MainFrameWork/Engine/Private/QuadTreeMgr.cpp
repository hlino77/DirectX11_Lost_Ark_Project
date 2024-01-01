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

_uint CQuadTreeMgr::Add_Node(CQuadTreeNode* pNode)
{
	m_Nodes.push_back(pNode);
	return m_Nodes.size() - 1;
}

HRESULT CQuadTreeMgr::Reset_QaudTree()
{
	WRITE_LOCK

	if (nullptr != m_pSuperNode)
	{
		Safe_Delete(m_pSuperNode);

		for (auto& Node : m_Nodes)
		{
			Safe_Delete(Node);
		}
		m_Nodes.clear();
	}


	return S_OK;
}

void CQuadTreeMgr::Set_Object_NodeIndex(CGameObject* pObject)
{
	m_pSuperNode->Set_Object_NodeIndex(pObject);
}


HRESULT CQuadTreeMgr::Add_Object(CGameObject* pObject, _uint iIndex)
{
	if (iIndex >= m_Nodes.size())
		return E_FAIL;

	if (FAILED(m_Nodes[iIndex]->Add_Object(pObject)))
		return E_FAIL;

	return S_OK;
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
