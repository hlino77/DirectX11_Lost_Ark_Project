#include "BehaviorTree.h"
#include "BT_Composite.h"
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
}
