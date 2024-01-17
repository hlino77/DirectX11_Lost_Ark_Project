#include "stdafx.h"
#include "Valtan_BT_Phase2_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Server.h>
CValtan_BT_Phase2_Server::CValtan_BT_Phase2_Server()
{
}

void CValtan_BT_Phase2_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CValtan_BT_Phase2_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		static_cast<CBoss_Server*>(m_pGameObject)->Move_to_SpawnPosition();
	}
	if (!static_cast<CBoss_Server*>(m_pGameObject)->Is_GroggyLock() && m_vecAnimDesc[0].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(true);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[4].iAnimIndex) - 3)
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(false);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Phase2_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Server*>(m_pGameObject)->Reset_SkillStack();
	static_cast<CBoss_Server*>(m_pGameObject)->Set_Counter(false);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_Grogginess(false);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_Phase(2);
}


CValtan_BT_Phase2_Server* CValtan_BT_Phase2_Server::Create(void* pArg)
{
	CValtan_BT_Phase2_Server* pInstance = new CValtan_BT_Phase2_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Phase2_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Phase2_Server::Free()
{
	__super::Free();
}
