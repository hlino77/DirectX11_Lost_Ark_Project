#include "stdafx.h"
#include "Valtan_BT_Attack_Attack18_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_BT_IF_ArmorBreak_Server.h>
#include "NavigationMgr.h"

CValtan_BT_Attack_Attack18_Server::CValtan_BT_Attack_Attack18_Server()
{
}

void CValtan_BT_Attack_Attack18_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
	 m_iLoop = 0;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack18_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_Counter() || static_cast<CBoss_Server*>(m_pGameObject)->Get_Grogginess())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_Counter(false);
		static_cast<CBoss_Server*>(m_pGameObject)->Set_Grogginess(false);
		return BT_SUCCESS;
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_iLoop < 2)
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) > 48 && m_iLoop < 2)
	{
		m_iLoop++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_iLoop == 2 && static_cast<CBoss_Server*>(m_pGameObject)->Get_Phase() == 2)
		static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(true);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) > 48 && m_iLoop < 3)
	{
			static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(false);
		m_iLoop++;
		m_iCurrAnimation = 2;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[2].iAnimIndex, m_vecAnimDesc[2].fChangeTime, m_vecAnimDesc[2].iStartFrame, m_vecAnimDesc[2].iChangeFrame);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_iCurrAnimation == 2)
	{
		m_pGameObject->Get_TransformCom()->Go_Straight(static_cast<CMonster_Server*>(m_pGameObject)->Get_MoveSpeed() * 3.f, fTimeDelta);
		if (CNavigationMgr::GetInstance()->Is_Outside(m_pGameObject->Get_CurrLevel(), m_pGameObject, 1.f))
		{
			m_iCurrAnimation = 3;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[3].iAnimIndex, m_vecAnimDesc[3].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[3].iChangeFrame);
		}
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack18_Server::OnEnd()
{
	__super::OnEnd();
	m_iLoop = 0.f;
	static_cast<CMonster_Server*>(m_pGameObject)->Reset_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_Attack18_Server* CValtan_BT_Attack_Attack18_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack18_Server* pInstance = new CValtan_BT_Attack_Attack18_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack18_Server::Free()
{
	__super::Free();
}
