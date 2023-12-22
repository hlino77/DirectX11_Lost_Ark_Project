#include "stdafx.h"
#include "Common_BT_Damage2_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CCommon_BT_Damage2_Server::CCommon_BT_Damage2_Server()
{
}

void CCommon_BT_Damage2_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
	m_fDuration = 0.6f;
}

CBT_Node::BT_RETURN CCommon_BT_Damage2_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_fDuration < 0.4f && static_cast<CMonster_Server*>(m_pGameObject)->Is_SecondHit())
		return BT_FAIL;

	if (m_fDuration < 0.f)
	{
		static_cast<CMonster_Server*>(m_pGameObject)->Set_SecondHit(false);
		static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);
		return BT_SUCCESS;
	}
	m_fDuration -= fTimeDelta;
	return BT_RUNNING;
}

void CCommon_BT_Damage2_Server::OnEnd()
{
	__super::OnEnd();

}



CCommon_BT_Damage2_Server* CCommon_BT_Damage2_Server::Create(void* pArg)
{
	CCommon_BT_Damage2_Server* pInstance = new CCommon_BT_Damage2_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Damage2_Server::Free()
{
	__super::Free();
}
