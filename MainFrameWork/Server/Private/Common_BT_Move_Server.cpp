#include "stdafx.h"
#include "Common_BT_Move_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "NavigationMgr.h"

CCommon_BT_Move_Server::CCommon_BT_Move_Server()
{
}

void CCommon_BT_Move_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_RandomPosition();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CCommon_BT_Move_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;

	if (static_cast<CMonster_Server*>(m_pGameObject)->Get_NearTargetDistance() < static_cast<CMonster_Server*>(m_pGameObject)->Get_NoticeRange() && !static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;

	if(static_cast<CMonster_Server*>(m_pGameObject)->Is_Close_To_RandomPosition())
		return BT_FAIL;

	static_cast<CMonster_Server*>(m_pGameObject)->Move_to_RandomPosition(fTimeDelta);

	if(CNavigationMgr::GetInstance()->Is_Outside(m_pGameObject->Get_CurrLevel(), m_pGameObject, 0.2f))
		return BT_FAIL;

	return BT_RUNNING;
}

void CCommon_BT_Move_Server::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Move_Server* CCommon_BT_Move_Server::Create(void* pArg)
{
	CCommon_BT_Move_Server* pInstance = new CCommon_BT_Move_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Move_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Move_Server::Free()
{
	__super::Free();
}
