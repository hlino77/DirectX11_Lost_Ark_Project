#include "stdafx.h"
#include "Common_BT_Dead_Server.h"
#include "Monster_Server.h"
#include "GameInstance.h"
#include "Model.h"
CCommon_BT_Dead_Server::CCommon_BT_Dead_Server()
{
}

void CCommon_BT_Dead_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CCommon_BT_Dead_Server::OnUpdate(const _float& fTimeDelta)
{

	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[0].iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CCommon_BT_Dead_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Left(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_AnimationSpeed(0.f);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Die();
	//if (m_pGameObject->Get_ObjectType() == OBJ_TYPE::MONSTER)
	//	CGameInstance::GetInstance()->Delete_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_MONSTER, m_pGameObject);
	//if (m_pGameObject->Get_ObjectType() == OBJ_TYPE:: BOSS)
	//	CGameInstance::GetInstance()->Delete_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_BOSS, m_pGameObject);
}



CCommon_BT_Dead_Server* CCommon_BT_Dead_Server::Create(void* pArg)
{
	CCommon_BT_Dead_Server* pInstance = new CCommon_BT_Dead_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Dead_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Dead_Server::Free()
{
	__super::Free();
}
