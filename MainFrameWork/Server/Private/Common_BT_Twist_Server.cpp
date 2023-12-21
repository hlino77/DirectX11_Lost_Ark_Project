#include "stdafx.h"
#include "Common_BT_Twist_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CCommon_BT_Twist_Server::CCommon_BT_Twist_Server()
{
}

void CCommon_BT_Twist_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CCommon_BT_Twist_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit() && m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrameRatio(m_vecAnimIndexFrame[m_iCurrAnimation].iAnimIndex) > 0.8f)
		return BT_SUCCESS;

	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_Twist_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Twist(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);

	static_cast<CMonster_Server*>(m_pGameObject)->Set_TwistLanding(true);
}



CCommon_BT_Twist_Server* CCommon_BT_Twist_Server::Create(void* pArg)
{
	CCommon_BT_Twist_Server* pInstance = new CCommon_BT_Twist_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Twist_Server::Free()
{
	__super::Free();
}
