#include "stdafx.h"
#include "Valtan_BT_Attack_Attack5_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Server.h>
#include "GameInstance.h"

CValtan_BT_Attack_Attack5_Server::CValtan_BT_Attack_Attack5_Server()
{
}

void CValtan_BT_Attack_Attack5_Server::OnStart()
{
	__super::OnStart(0);
	m_vDirection = Vec3(CGameInstance::GetInstance()->Get_RandomFloat(-1.f,1.f), 0.f, CGameInstance::GetInstance()->Get_RandomFloat(-1.f, 1.f));
	m_vDirection.Normalize();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_TargetPos(m_vDirection);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
	m_iStack = 0;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack5_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_Counter() || static_cast<CBoss_Server*>(m_pGameObject)->Get_Grogginess())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_Counter(false);
		static_cast<CBoss_Server*>(m_pGameObject)->Set_Grogginess(false);
		return BT_SUCCESS;
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_fLoopTime > m_vecAnimDesc[1].fMaxLoopTime && m_iStack<5)
	{
		
		m_iStack++;
		m_fLoopTime = 0.f;
		static_cast<CMonster_Server*>(m_pGameObject)->Set_SetuponCell(false);
		Vec3 vPlayerPosition = static_cast<CMonster_Server*>(m_pGameObject)->Get_NearTarget()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPlayerPosition + m_vDirection * 1.5f);
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction();
		m_vDirection = Vec3::TransformNormal(m_vDirection, Matrix::CreateRotationY(XMConvertToRadians(195.f)));
		m_vDirection.Normalize();
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == (m_vecAnimDesc[2].iAnimIndex) && m_iStack == 5)
	{
		static_cast<CMonster_Server*>(m_pGameObject)->Set_SetuponCell(true);
		m_iStack++;
		Vec3 vPlayerPosition = static_cast<CMonster_Server*>(m_pGameObject)->Get_TargetPos();
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, static_cast<CBoss_Server*>(m_pGameObject)->Get_SpawnPosition() + m_vDirection * 1.5f);
		static_cast<CMonster_Server*>(m_pGameObject)->Get_TransformCom()->LookAt(static_cast<CBoss_Server*>(m_pGameObject)->Get_SpawnPosition());
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack5_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_Attack5_Server* CValtan_BT_Attack_Attack5_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack5_Server* pInstance = new CValtan_BT_Attack_Attack5_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack5_Server::Free()
{
	__super::Free();
}
