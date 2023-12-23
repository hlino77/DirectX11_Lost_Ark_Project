#include "BehaviorTree.h"
#include "BT_Composite.h"
#include "BT_Action.h"

CBehaviorTree::CBehaviorTree(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CBehaviorTree::CBehaviorTree(const CComponent& rhs)
	:CComponent(rhs)
{
}

HRESULT CBehaviorTree::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBehaviorTree::Initialize(void* pArg)
{
	m_PreviousAction = m_hashActions.end();
	return S_OK;
}

void CBehaviorTree::Tick(const _float& fTimeDelta)
{
	m_pRootNode->Tick(fTimeDelta);
}

void CBehaviorTree::LateTick(const _float& fTimeDelta)
{
}

void CBehaviorTree::DebugRender()
{
}

HRESULT CBehaviorTree::Add_Action(wstring strActionName, CBT_Action* pAction)
{
	if (m_hashActions.count(strActionName) == 0)
		m_hashActions.emplace(strActionName, pAction);
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CBehaviorTree::Tick_Action(wstring strActionName, const _float& fTimeDelta)
{
	const auto& iter = m_hashActions.find(strActionName);
	if (iter == m_hashActions.end())
		return E_FAIL;
	if( iter != m_PreviousAction)
	{
		m_PreviousAction->second->OnEnd();
		m_PreviousAction->second->Reset();
		m_PreviousAction = iter;
	}
	iter->second->Tick(fTimeDelta);


	return S_OK;
}

HRESULT CBehaviorTree::Init_PreviousAction(wstring strAction)
{
	const auto& iter = m_hashActions.find(strAction);
	if (iter == m_hashActions.end())
		return E_FAIL;
	m_PreviousAction = iter;
	m_PreviousAction->second->OnStart();
	return S_OK;
}

void CBehaviorTree::ResetAll()
{ 
	m_pRootNode->Reset();
}


CBehaviorTree* CBehaviorTree::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBehaviorTree* pInstance = new CBehaviorTree(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBehaviorTree");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CBehaviorTree::Clone(CGameObject* pGameObject, void* pArg)
{
	CBehaviorTree* pInstance = new CBehaviorTree(*this);
	pInstance->m_pOwner = pGameObject;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBehaviorTree");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBehaviorTree::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
	for (Actions::iterator iter = m_hashActions.begin(); iter != m_hashActions.end(); iter++ )
		Safe_Release(iter->second);
	m_hashActions.clear();
}
