#include "stdafx.h"
#include "Ghoul_BT_Attack_3.h"
#include "Monster.h"
#include "Transform.h"
#include "Model.h"
CGhoul_BT_Attack_3::CGhoul_BT_Attack_3()
{
}

void CGhoul_BT_Attack_3::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->Set_Action(m_strActionName);

}

CBT_Node::BT_RETURN CGhoul_BT_Attack_3::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_SUCCESS;
	return BT_RUNNING;
}

void CGhoul_BT_Attack_3::OnEnd()
{
	__super::OnEnd();

	static_cast<CMonster*>(m_pGameObject)->Set_AttackRange(0);

}



CGhoul_BT_Attack_3* CGhoul_BT_Attack_3::Create(void* pArg)
{
	CGhoul_BT_Attack_3* pInstance = new CGhoul_BT_Attack_3;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGhoul_BT_Attack_3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhoul_BT_Attack_3::Free()
{
	__super::Free();
}
