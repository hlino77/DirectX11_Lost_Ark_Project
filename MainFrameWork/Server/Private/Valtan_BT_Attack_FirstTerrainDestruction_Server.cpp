#include "stdafx.h"
#include "Valtan_BT_Attack_FirstTerrainDestruction_Server.h"
#include "Boss_Server.h"
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include <AsUtils.h>
#include "GameSessionManager.h"

CValtan_BT_Attack_FirstTerrainDestruction_Server::CValtan_BT_Attack_FirstTerrainDestruction_Server()
{
}

void CValtan_BT_Attack_FirstTerrainDestruction_Server::OnStart()
{
	__super::OnStart(0);
	m_vLandPosition = Vec3(104.5f, 0.f, 104.5f);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_FirstTerrainDestruction_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		m_pGameObject->Get_TransformCom()->LookAt_Dir(XMVector3Normalize(m_vLandPosition - static_cast<CBoss_Server*>(m_pGameObject)->Get_SpawnPosition()));
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vLandPosition);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex)
	{
		m_pGameObject->Get_TransformCom()->LookAt_ForLandObject(static_cast<CBoss_Server*>(m_pGameObject)->Get_SpawnPosition());
		m_pGameObject->Get_TransformCom()->Go_Straight(static_cast<CMonster_Server*>(m_pGameObject)->Get_MoveSpeed() * 1.1f, fTimeDelta);
	}
	if (m_iCurrAnimation == 12 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[12].iAnimIndex && m_fLoopTime < 1.5f)
	{
		if (!static_cast<CBoss_Server*>(m_pGameObject)->Is_TargetLock())
		{
			static_cast<CBoss_Server*>(m_pGameObject)->Set_TargetLock(true);
		}
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	}
	if (m_iCurrAnimation == 13 && static_cast<CBoss_Server*>(m_pGameObject)->Is_TargetLock())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_TargetLock(false);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_FirstTerrainDestruction_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Server*>(m_pGameObject)->Set_TargetLock(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}

void CValtan_BT_Attack_FirstTerrainDestruction_Server::On_FirstAnimStart()
{
	static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(true);
}

void CValtan_BT_Attack_FirstTerrainDestruction_Server::On_LastAnimEnd()
{
	static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(false);
}


CValtan_BT_Attack_FirstTerrainDestruction_Server* CValtan_BT_Attack_FirstTerrainDestruction_Server::Create(void* pArg)
{
	CValtan_BT_Attack_FirstTerrainDestruction_Server* pInstance = new CValtan_BT_Attack_FirstTerrainDestruction_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_FirstTerrainDestruction_Server::Free()
{
	__super::Free();
}
