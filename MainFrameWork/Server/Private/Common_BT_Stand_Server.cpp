#include "stdafx.h"
#include "Common_BT_Stand_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "BehaviorTree.h"
Common_BT_Stand_Server::Common_BT_Stand_Server()
{
}

void Common_BT_Stand_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN Common_BT_Stand_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_SUCCESS;
	return __super::OnUpdate(fTimeDelta);
}

void Common_BT_Stand_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Downed(false);
}



Common_BT_Stand_Server* Common_BT_Stand_Server::Create(void* pArg)
{
	Common_BT_Stand_Server* pInstance = new Common_BT_Stand_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Common_BT_Stand_Server::Free()
{
	__super::Free();
}
