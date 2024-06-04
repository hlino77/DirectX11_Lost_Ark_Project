#include "stdafx.h"
#include "Valtan_BT_Dead_Server.h"
#include "Monster_Server.h"
#include "GameInstance.h"
#include "Model.h"
CValtan_BT_Dead_Server::CValtan_BT_Dead_Server()
{
}

void CValtan_BT_Dead_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CValtan_BT_Dead_Server::OnUpdate(_float fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[5].iAnimIndex) >= 34 && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_iCurrAnimation++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
	}

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Dead_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Left(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_AnimationSpeed(0.f);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Die();
}



CValtan_BT_Dead_Server* CValtan_BT_Dead_Server::Create(void* pArg)
{
	CValtan_BT_Dead_Server* pInstance = new CValtan_BT_Dead_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Dead_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Dead_Server::Free()
{
	__super::Free();
}
