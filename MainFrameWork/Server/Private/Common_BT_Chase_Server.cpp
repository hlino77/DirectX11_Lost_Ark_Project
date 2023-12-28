#include "stdafx.h"
#include "Common_BT_Chase_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"

CCommon_BT_Chase_Server::CCommon_BT_Chase_Server()
{
}

void CCommon_BT_Chase_Server::OnStart()
{
	__super::OnStart(0);
	if(m_pGameObject->Get_ObjectType()==OBJ_TYPE::MONSTER)
		static_cast<CMonster_Server*>(m_pGameObject)->Set_RandomPosition(static_cast<CMonster_Server*>(m_pGameObject)->Get_AttackRange());
	else
		static_cast<CMonster_Server*>(m_pGameObject)->Set_RandomPosition(0.f);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CCommon_BT_Chase_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Skill())
		return BT_FAIL;
	if (static_cast<CMonster_Server*>(m_pGameObject)->Get_NearTargetDistance() < static_cast<CMonster_Server*>(m_pGameObject)->Get_AttackRange())
		return BT_FAIL;	
	_float fSpeed = 1.5f * static_cast<CMonster_Server*>(m_pGameObject)->Get_MoveSpeed();
	static_cast<CMonster_Server*>(m_pGameObject)->Move_Dir(static_cast<CMonster_Server*>(m_pGameObject)->Get_Target_Direction(), fSpeed, fTimeDelta);
	return BT_RUNNING;
}

void CCommon_BT_Chase_Server::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Chase_Server* CCommon_BT_Chase_Server::Create(void* pArg)
{
	CCommon_BT_Chase_Server* pInstance = new CCommon_BT_Chase_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Chase_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}
 
void CCommon_BT_Chase_Server::Free()
{
	__super::Free();
}
