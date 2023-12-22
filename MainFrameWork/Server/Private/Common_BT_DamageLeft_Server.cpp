#include "stdafx.h"
#include "Common_BT_DamageLeft_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CCommon_BT_DamageLeft_Server::CCommon_BT_DamageLeft_Server()
{
}

void CCommon_BT_DamageLeft_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CCommon_BT_DamageLeft_Server::OnUpdate(const _float& fTimeDelta)
{
	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CCommon_BT_DamageLeft_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Left(false);

}



CCommon_BT_DamageLeft_Server* CCommon_BT_DamageLeft_Server::Create(void* pArg)
{
	CCommon_BT_DamageLeft_Server* pInstance = new CCommon_BT_DamageLeft_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_DamageLeft_Server::Free()
{
	__super::Free();
}
