#include "stdafx.h"
#include "King_BT_Attack_Erruption_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include <Boss_Server.h>

CKing_BT_Attack_Erruption_Server::CKing_BT_Attack_Erruption_Server()
{
}

void CKing_BT_Attack_Erruption_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CKing_BT_Attack_Erruption_Server::OnUpdate(const _float& fTimeDelta)
{
	if ( static_cast<CBoss_Server*>(m_pGameObject)->Get_Grogginess())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_Grogginess(false);
		return BT_SUCCESS;
	}
	return __super::OnUpdate(fTimeDelta);
}

void CKing_BT_Attack_Erruption_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	static_cast<CMonster_Server*>(m_pGameObject)->Reset_SkillStack();
}



CKing_BT_Attack_Erruption_Server* CKing_BT_Attack_Erruption_Server::Create(void* pArg)
{
	CKing_BT_Attack_Erruption_Server* pInstance = new CKing_BT_Attack_Erruption_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CKing_BT_Attack_Erruption_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKing_BT_Attack_Erruption_Server::Free()
{
	__super::Free();
}
