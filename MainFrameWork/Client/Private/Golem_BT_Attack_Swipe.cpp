#include "stdafx.h"
#include "Golem_BT_Attack_Swipe.h"
#include "Monster.h"
#include "Model.h"
CGolem_BT_Attack_Swipe::CGolem_BT_Attack_Swipe()
{
}

void CGolem_BT_Attack_Swipe::OnStart()
{
	__super::OnStart(0);



}

CBT_Node::BT_RETURN CGolem_BT_Attack_Swipe::OnUpdate(const _float& fTimeDelta)
{

	return BT_RUNNING;
}

void CGolem_BT_Attack_Swipe::OnEnd()
{
	__super::OnEnd();

}



CGolem_BT_Attack_Swipe* CGolem_BT_Attack_Swipe::Create(void* pArg)
{
	CGolem_BT_Attack_Swipe* pInstance = new CGolem_BT_Attack_Swipe;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Swipe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Swipe::Free()
{
	__super::Free();
}
