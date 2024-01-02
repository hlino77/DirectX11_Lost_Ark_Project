#include "stdafx.h"
#include "Valtan_BT_Attack_Attack1_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack1_Server::CValtan_BT_Attack_Attack1_Server()
{
}

void CValtan_BT_Attack_Attack1_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
	m_iLoop = 0;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack1_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() ==m_vecAnimDesc[0].iAnimIndex && m_iLoop < 2)
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex&& m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex)>48 && m_iLoop < 3)
	{
		m_iLoop++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack1_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_Attack1_Server* CValtan_BT_Attack_Attack1_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack1_Server* pInstance = new CValtan_BT_Attack_Attack1_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack1_Server::Free()
{
	__super::Free();
}
