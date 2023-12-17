#include "stdafx.h"
#include "Ghoul_BT_Attack_2.h"
#include "Monster.h"
#include "Transform.h"
#include "Model.h"
CGhoul_BT_Attack_2::CGhoul_BT_Attack_2()
{
}

void CGhoul_BT_Attack_2::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CGhoul_BT_Attack_2::OnUpdate(const _float& fTimeDelta)
{

	return BT_RUNNING;
}

void CGhoul_BT_Attack_2::OnEnd()
{
	__super::OnEnd();

	static_cast<CMonster*>(m_pGameObject)->Set_AttackRange(0);

}



CGhoul_BT_Attack_2* CGhoul_BT_Attack_2::Create(void* pArg)
{
	CGhoul_BT_Attack_2* pInstance = new CGhoul_BT_Attack_2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGhoul_BT_Attack_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhoul_BT_Attack_2::Free()
{
	__super::Free();
}
