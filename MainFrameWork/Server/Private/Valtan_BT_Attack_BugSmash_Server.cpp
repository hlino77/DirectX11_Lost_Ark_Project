#include "stdafx.h"
#include "Valtan_BT_Attack_BugSmash_Server.h"
#include "Model.h"
#include "Transform.h"
#include "NavigationMgr.h"
#include <Boss_Valtan_Server.h>

CValtan_BT_Attack_BugSmash_Server::CValtan_BT_Attack_BugSmash_Server()
{
}

void CValtan_BT_Attack_BugSmash_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_BugSmash_Server::OnUpdate(const _float& fTimeDelta)
{
	m_pGameObject->Get_TransformCom()->Set_Up(Vec3(0.f, 1.f, 0.f));
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_Counter())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_Counter(false);
		return BT_SUCCESS;
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Move_to_SpawnPosition();
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[6].iAnimIndex)
	{
		m_pGameObject->Set_Invincible(false);
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[7].iAnimIndex && !m_pGameObject->Get_ModelCom()->IsNext() && m_fLoopTime > 1.f && !static_cast<CBoss_Server*>(m_pGameObject)->Is_CounterSkill())
	{
		static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Set_BugSmash(true);
		static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(true);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[7].iAnimIndex && m_pGameObject->Get_ModelCom()->IsNext() && static_cast<CBoss_Server*>(m_pGameObject)->Is_CounterSkill())
	{
		static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Set_BugSmash(false);
		static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(false);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[8].iAnimIndex && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		if (CNavigationMgr::GetInstance()->Is_Outside(m_pGameObject->Get_CurrLevel(), m_pGameObject, 1.5f))
		{
			m_iCurrAnimation++;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[14].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[14].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[14].iAnimIndex) - 3 && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_iCurrAnimation=10;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
		m_fLoopTime = 0;
		return BT_RUNNING;
	}

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_BugSmash_Server::OnEnd()
{
	__super::OnEnd();
	Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
	vLook.y = 0.f;
	vLook.Normalize();
	m_pGameObject->Get_TransformCom()->LookAt_Dir(vLook);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}

void CValtan_BT_Attack_BugSmash_Server::On_FirstAnimStart()
{
	m_pGameObject->Set_Invincible(true);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(true);
}

void CValtan_BT_Attack_BugSmash_Server::On_LastAnimEnd()
{
	static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(false);
}



CValtan_BT_Attack_BugSmash_Server* CValtan_BT_Attack_BugSmash_Server::Create(void* pArg)
{
	CValtan_BT_Attack_BugSmash_Server* pInstance = new CValtan_BT_Attack_BugSmash_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_BugSmash_Server::Free()
{
	__super::Free();
}
