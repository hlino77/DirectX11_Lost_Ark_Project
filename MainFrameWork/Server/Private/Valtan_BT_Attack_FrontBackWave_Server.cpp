#include "stdafx.h"
#include "Valtan_BT_Attack_FrontBackWave_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan_Server.h>

CValtan_BT_Attack_FrontBackWave_Server::CValtan_BT_Attack_FrontBackWave_Server()
{
}

void CValtan_BT_Attack_FrontBackWave_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
	m_bShoot = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_FrontBackWave_Server::OnUpdate(_float fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 80 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex&&!m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_iCurrAnimation++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex,
			m_vecAnimDesc[m_iCurrAnimation].fChangeTime, m_vecAnimDesc[m_iCurrAnimation].iStartFrame,
			m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 40 && m_bShoot)
	{
		m_bShoot = false;
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		Vec3 vRight = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_RIGHT);
		vRight.Normalize();
		static_cast<CBoss_Valtan_Server*>(m_pGameObject)->BroadCast_Ghost(vPos,vRight);
	}
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_SkipAction() )
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);
		
		return BT_SUCCESS;
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_FrontBackWave_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_FrontBackWave_Server* CValtan_BT_Attack_FrontBackWave_Server::Create(void* pArg)
{
	CValtan_BT_Attack_FrontBackWave_Server* pInstance = new CValtan_BT_Attack_FrontBackWave_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_FrontBackWave_Server::Free()
{
	__super::Free();
}
