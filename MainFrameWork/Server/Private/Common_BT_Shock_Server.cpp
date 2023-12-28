#include "stdafx.h"
#include "Common_BT_Shock_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CCommon_BT_Shock_Server::CCommon_BT_Shock_Server()
{
}

void CCommon_BT_Shock_Server::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Shock_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Get_StatusEffect(STATUSEFFECT::SHOCK) < 0.f)
		return BT_SUCCESS;
	

	return BT_RUNNING;
}

void CCommon_BT_Shock_Server::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Shock_Server* CCommon_BT_Shock_Server::Create(void* pArg)
{
	CCommon_BT_Shock_Server* pInstance = new CCommon_BT_Shock_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Shock_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Shock_Server::Free()
{
	__super::Free();
}
