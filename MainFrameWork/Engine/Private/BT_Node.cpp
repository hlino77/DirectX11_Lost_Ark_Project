#include "BT_Node.h"
#include "GameObject.h"

CBT_Node::CBT_Node()
{
}

CBT_Node::CBT_Node(const CBT_Node& rhs)
:CBase()
{
}

HRESULT CBT_Node::Initialize(void* pArg)
{	

	NODE_DESC* pNodeDesc = static_cast<NODE_DESC*>(pArg);
	if (pNodeDesc == nullptr)
		return E_FAIL;
	m_pBehaviorTree = pNodeDesc->pBehaviorTree;
	m_pGameObject = pNodeDesc->pGameObject;

	return S_OK;
}

CBT_Node::BT_RETURN CBT_Node::Tick(const _float& fTimeDelta)
{
	if (BT_RETURN::BT_RUNNING != m_eReturn)
		OnStart();

	m_eReturn = OnUpdate(fTimeDelta);

	if (BT_RETURN::BT_RUNNING != m_eReturn)
		OnEnd();

	return m_eReturn;
}



HRESULT CBT_Node::AddChild(CBT_Node* pChild)
{
	switch (m_eNodeType)
	{
	case Engine::CBT_Node::BT_NODETYPE::COMPOSITE:
		m_vecChildren.push_back(pChild);
		return S_OK;
	case Engine::CBT_Node::BT_NODETYPE::DECORATOR:
		if (m_vecChildren.empty())
		{
			m_vecChildren.push_back(pChild);
			return S_OK;
		}
		else
			return E_FAIL;
	case Engine::CBT_Node::BT_NODETYPE::ACTION:
		return E_FAIL; 
	default:
		return E_FAIL;
	}
	return S_OK;
}

void CBT_Node::Free()
{
	for (auto& iter : m_vecChildren)
		Safe_Release(iter);
	m_vecChildren.clear();

	__super::Free();
}
