#include "stdafx.h"
#include "Common_BT_Attack1_Server.h"
#include "Monster_Server.h"
#include "Model.h"

CCommon_BT_Attack1_Server::CCommon_BT_Attack1_Server()
{
}

void CCommon_BT_Attack1_Server::OnStart()
{
	__super::OnStart(0);

	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Boss_Action();
}

CBT_Node::BT_RETURN CCommon_BT_Attack1_Server::OnUpdate(const _float& fTimeDelta)
{

	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;

	if(m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CCommon_BT_Attack1_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
}



CCommon_BT_Attack1_Server* CCommon_BT_Attack1_Server::Create(void* pArg)
{
	CCommon_BT_Attack1_Server* pInstance = new CCommon_BT_Attack1_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Attack1_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Attack1_Server::Free()
{
	__super::Free();
}
