#include "stdafx.h"
#include "Valtan_BT_Attack_Attack22_Server.h"
#include "Boss_Server.h"
#include "Model.h"
#include "Transform.h"


CValtan_BT_Attack_Attack22_Server::CValtan_BT_Attack_Attack22_Server()
{
}

void CValtan_BT_Attack_Attack22_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack22_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_fLoopTime < 0.f && m_vecAnimDesc[1].bIsLoop && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		m_pGameObject->Set_Invincible(true);
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));		
		static_cast<CBoss_Server*>(m_pGameObject)->Move_to_SpawnPosition();
	}
	else if(m_pGameObject->Is_Invincible())
		m_pGameObject->Set_Invincible(false);
	if ( m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex)
	{
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	}
	if(m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex&& m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrameRatio(m_vecAnimDesc[5].iAnimIndex)>0.7f && !static_cast<CBoss_Server*>(m_pGameObject)->Is_CounterSkill())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(true);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex && m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[5].iAnimIndex)&& static_cast<CBoss_Server*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(false);
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack22_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_Attack22_Server* CValtan_BT_Attack_Attack22_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack22_Server* pInstance = new CValtan_BT_Attack_Attack22_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack22_Server::Free()
{
	__super::Free();
}
