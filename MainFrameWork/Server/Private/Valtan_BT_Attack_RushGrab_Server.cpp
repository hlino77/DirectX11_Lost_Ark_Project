#include "stdafx.h"
#include "Valtan_BT_Attack_RushGrab_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Server.h>
#include <Boss_Valtan_Server.h>

CValtan_BT_Attack_RushGrab_Server::CValtan_BT_Attack_RushGrab_Server()
{
}

void CValtan_BT_Attack_RushGrab_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_RushGrab_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_SkipAction() )
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);
		
		return BT_SUCCESS;
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex&& m_fLoopTime < m_vecAnimDesc[m_iCurrAnimation].fMaxLoopTime - 0.5f)
		static_cast<CBoss_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() ==m_vecAnimDesc[3].iAnimIndex&& m_pGameObject->Get_ModelCom()->IsNext())
		m_pGameObject->Get_TransformCom()->Turn_Speed(m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_UP),XMConvertToRadians(280.f) ,fTimeDelta);
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_RushGrab_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Reset_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_RushGrab_Server* CValtan_BT_Attack_RushGrab_Server::Create(void* pArg)
{
	CValtan_BT_Attack_RushGrab_Server* pInstance = new CValtan_BT_Attack_RushGrab_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_RushGrab_Server::Free()
{
	__super::Free();
}
