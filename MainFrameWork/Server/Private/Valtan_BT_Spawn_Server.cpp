#include "stdafx.h"
#include "Valtan_BT_Spawn_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Spawn_Server::CValtan_BT_Spawn_Server()
{
}

void CValtan_BT_Spawn_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Find_NearTarget(1.f);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CValtan_BT_Spawn_Server::OnUpdate(_float fTimeDelta)
{
	if (m_iCurrAnimation == 4 || m_iCurrAnimation == 3)
	{
		m_pGameObject->Get_TransformCom()->LookAt_Lerp_ForLand(Vec3(0.f, 0.f, -1.f), 3.f, fTimeDelta);
	}

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Spawn_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Spawn(false);
}


CValtan_BT_Spawn_Server* CValtan_BT_Spawn_Server::Create(void* pArg)
{
	CValtan_BT_Spawn_Server* pInstance = new CValtan_BT_Spawn_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Spawn_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Spawn_Server::Free()
{
	__super::Free();
}
