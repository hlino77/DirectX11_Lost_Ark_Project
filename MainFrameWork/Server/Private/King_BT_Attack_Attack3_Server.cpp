#include "stdafx.h"
#include "King_BT_Attack_Attack3_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CKing_BT_Attack_Attack3_Server::CKing_BT_Attack_Attack3_Server()
{
}

void CKing_BT_Attack_Attack3_Server::OnStart()
{
	__super::OnStart(0);

	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Boss_Action();

}

CBT_Node::BT_RETURN CKing_BT_Attack_Attack3_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta * 5.f);
	else if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[2].iAnimIndex) < 51)
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	return __super::OnUpdate(fTimeDelta);;
}

void CKing_BT_Attack_Attack3_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CKing_BT_Attack_Attack3_Server* CKing_BT_Attack_Attack3_Server::Create(void* pArg)
{
	CKing_BT_Attack_Attack3_Server* pInstance = new CKing_BT_Attack_Attack3_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CKing_BT_Attack_Attack3_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKing_BT_Attack_Attack3_Server::Free()
{
	__super::Free();
}
