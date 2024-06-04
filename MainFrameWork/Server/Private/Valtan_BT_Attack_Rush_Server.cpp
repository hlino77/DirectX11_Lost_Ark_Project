#include "stdafx.h"
#include "Valtan_BT_Attack_Rush_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include "NavigationMgr.h"
#include <Boss_Valtan_Server.h>

CValtan_BT_Attack_Rush_Server::CValtan_BT_Attack_Rush_Server()
{
}

void CValtan_BT_Attack_Rush_Server::OnStart()
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Is_Dummy())
		__super::OnStart(1);
	else
		__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
	m_iLoop = 0;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Rush_Server::OnUpdate(_float fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_SkipAction())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);

		return BT_SUCCESS;
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() ==m_vecAnimDesc[0].iAnimIndex && m_iLoop < 2)
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex&& m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex)>48 && m_iLoop < 2)
	{
		m_iLoop++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_iLoop == 2 )
	{
		if (static_cast<CBoss_Server*>(m_pGameObject)->Get_Armor() < 1 || static_cast<CBoss_Server*>(m_pGameObject)->Get_Phase() > 1)
			static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(true);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 48 && m_iLoop < 3)
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(false);
		static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Set_Rush(true);
		m_iLoop++;
		m_iCurrAnimation = 1;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[1].iAnimIndex, m_vecAnimDesc[1].fChangeTime,	m_vecAnimDesc[1].iStartFrame, m_vecAnimDesc[1].iChangeFrame);
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_iCurrAnimation == 1)
	{
		m_pGameObject->Get_TransformCom()->Go_Straight(static_cast<CMonster_Server*>(m_pGameObject)->Get_MoveSpeed() * 3.f, fTimeDelta);
		_float fOffset = 1.1f;
		if (static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Get_Phase() == 1)
			fOffset = 1.1f;
		else
			fOffset = 2.f;

		if (CNavigationMgr::GetInstance()->Is_Outside(m_pGameObject->Get_CurrLevel(), m_pGameObject, fOffset) && m_fLoopTime >= 0.1f)
		{

			static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Set_Rush(false);
			m_iCurrAnimation=2;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[2].iAnimIndex, m_vecAnimDesc[2].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[2].iChangeFrame);
		}
	}


	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Rush_Server::OnEnd()
{
	__super::OnEnd();
	m_iLoop = 0;
	static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Set_Rush(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_Rush_Server* CValtan_BT_Attack_Rush_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Rush_Server* pInstance = new CValtan_BT_Attack_Rush_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Rush_Server::Free()
{
	__super::Free();
}
