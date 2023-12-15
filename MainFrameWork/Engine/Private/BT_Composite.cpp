#include "BT_Composite.h"

CBT_Composite::CBT_Composite()
{
}

CBT_Composite::CBT_Composite(const CBT_Composite& rhs)
{
}

HRESULT CBT_Composite::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	COMPOSITE_DESC* pCompositeDesc = static_cast<COMPOSITE_DESC*> (pArg);
	if(pCompositeDesc == nullptr)
		return E_FAIL;
	m_eCompositeType = pCompositeDesc->eCompositeType;
	m_eNodeType = BT_NODETYPE::COMPOSITE;

	return S_OK;
}

void CBT_Composite::OnStart()
{
	__super::OnStart();
	m_RunningChild = m_vecChildren.begin();
}

CBT_Node::BT_RETURN CBT_Composite::OnUpdate(const _float& fTimeDelta)
{
	while (true)
	{
		m_eReturn = (*m_RunningChild)->Tick(fTimeDelta);

		switch (m_eCompositeType)
		{
		case CompositeType::SELECTOR:
		{
			if (BT_FAIL != m_eReturn)
				return m_eReturn;

			if (++m_RunningChild == m_vecChildren.end())
				return BT_FAIL;
		}
		break;
		case CompositeType::SEQUENCE:
		{
			if (BT_SUCCESS != m_eReturn)
				return m_eReturn;

			if (++m_RunningChild == m_vecChildren.end())
				return BT_SUCCESS;
		}
		break;
		}
	}
}

void CBT_Composite::OnEnd()
{
	__super::OnEnd();
}

void CBT_Composite::Reset()
{

	for (auto& iter : m_vecChildren)
	{
		iter->Reset();
	}
}

CBT_Composite* CBT_Composite::Create(void* pArg)
{
	CBT_Composite* pInstance = new CBT_Composite;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBT_Composite");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBT_Composite::Free()
{
	__super::Free();
}
