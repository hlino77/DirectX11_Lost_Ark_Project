#include "stdafx.h"
#include "Common_BT_Maz_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CCommon_BT_Maz_Server::CCommon_BT_Maz_Server()
{
}

void CCommon_BT_Maz_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Boss_Action();
}

CBT_Node::BT_RETURN CCommon_BT_Maz_Server::OnUpdate(const _float& fTimeDelta)
{
	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CCommon_BT_Maz_Server::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Maz_Server* CCommon_BT_Maz_Server::Create(void* pArg)
{
	CCommon_BT_Maz_Server* pInstance = new CCommon_BT_Maz_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Maz_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Maz_Server::Free()
{
	__super::Free();
}
