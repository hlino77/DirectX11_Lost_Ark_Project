#include "stdafx.h"
#include "Valtan_BT_Phase3_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include <Boss_Server.h>
#include "Transform.h"

CValtan_BT_Phase3_Server::CValtan_BT_Phase3_Server()
{
}

void CValtan_BT_Phase3_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
	static_cast<CBoss_Server*>(m_pGameObject)->Set_Phase(3);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_Hp(529402339);
}

CBT_Node::BT_RETURN CValtan_BT_Phase3_Server::OnUpdate(const _float& fTimeDelta)
{
	m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Phase3_Server::OnEnd()
{
	__super::OnEnd();

	static_cast<CBoss_Server*>(m_pGameObject)->Set_MaxSkillStack(2);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyGauge(static_cast<CBoss_Server*>(m_pGameObject)->Get_MaxGroggyGauge());
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_Groggy(false);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_Countered(false);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_Counter(false);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_Grogginess(false);
}


CValtan_BT_Phase3_Server* CValtan_BT_Phase3_Server::Create(void* pArg)
{
	CValtan_BT_Phase3_Server* pInstance = new CValtan_BT_Phase3_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Phase3_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Phase3_Server::Free()
{
	__super::Free();
}
