#include "stdafx.h"
#include "Golem_BT_Attack_Jump.h"
#include "Monster.h"
#include "Model.h"
#
CGolem_BT_Attack_Jump::CGolem_BT_Attack_Jump()
{
}

void CGolem_BT_Attack_Jump::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CGolem_BT_Attack_Jump::OnUpdate(const _float& fTimeDelta)
{
	return BT_RUNNING;
}

void CGolem_BT_Attack_Jump::OnEnd()
{
	__super::OnEnd();

}



CGolem_BT_Attack_Jump* CGolem_BT_Attack_Jump::Create(void* pArg)
{
	CGolem_BT_Attack_Jump* pInstance = new CGolem_BT_Attack_Jump;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Jump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Jump::Free()
{
	__super::Free();
}
