#include "BT_Decorator.h"

CBT_Decorator::CBT_Decorator()
{
}

CBT_Decorator::CBT_Decorator(const CBT_Decorator& rhs)
{
}

HRESULT CBT_Decorator::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	DECORATOR_DESC* pDecoratorDesc = static_cast<DECORATOR_DESC*> (pArg);
	if (pDecoratorDesc == nullptr)
		return E_FAIL;
	m_eDecoratorType = pDecoratorDesc->eDecoratorType;
	if (m_eDecoratorType == DecoratorType::TYPE_END)
		return E_FAIL;
	else if (m_eDecoratorType == DecoratorType::REPEAT && pDecoratorDesc->iRepeatCount > 1)
	{
		m_iRepeatCount = pDecoratorDesc->iRepeatCount;
	}
	else if (m_eDecoratorType == DecoratorType::REPEAT)
		return E_FAIL;
	m_bCondition = true;

	m_eNodeType = BT_NODETYPE::DECORATOR;
	return S_OK;
}

void CBT_Decorator::OnStart()
{
	__super::OnStart();
	if (m_eDecoratorType == DecoratorType::REPEAT)
		m_iCounter = m_iRepeatCount;
}

CBT_Node::BT_RETURN CBT_Decorator::OnUpdate(const _float& fTimeDelta)
{	
	switch (m_eDecoratorType)
	{
	case DecoratorType::IF:
	{
		if(true == m_bCondition)
			return m_vecChildren.front()->Tick(fTimeDelta);
		else
			return BT_FAIL;
	}
	break;
	case DecoratorType::WHILE:
	{
		if (true == m_bCondition)
		{
			m_vecChildren.front()->Tick(fTimeDelta);
			return BT_RUNNING;
		}
		else
			return BT_FAIL;
	}
	break;
	case DecoratorType::REPEAT:
	{
		if (0 < m_iCounter&&m_bCondition)
		{
			if (BT_SUCCESS == m_vecChildren[0]->Tick(fTimeDelta))
				m_iCounter--;

			m_eReturn = BT_RUNNING;
			return BT_RUNNING;
		}
		else
			return BT_SUCCESS;
	}
	break;
	}
	return BT_SUCCESS;
}

void CBT_Decorator::OnEnd()
{
	__super::OnEnd();
}




CBT_Decorator* CBT_Decorator::Create(void* pArg)
{
	CBT_Decorator* pInstance = new CBT_Decorator;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBT_Decorator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBT_Decorator::Free()
{
	__super::Free();
}
