#include "stdafx.h"
#include "Golem_BT_Chase_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"

CGolem_BT_Chase_Server::CGolem_BT_Chase_Server()
{
}

void CGolem_BT_Chase_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Boss_Action();
}

CBT_Node::BT_RETURN CGolem_BT_Chase_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;
	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_FAIL;

	if (static_cast<CMonster_Server*>(m_pGameObject)->Get_NearTargetDistance() < static_cast<CMonster_Server*>(m_pGameObject)->Get_AttackRange())
		return BT_FAIL;
	_float fSpeed = 1.5f;
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == 34)
	{
		_int Animframe = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(34);
		if (30 > Animframe % 30 && Animframe % 30 > 20)
		{
			fSpeed = Vec2::Lerp(Vec2(0.f,0.f),Vec2(0.5f, 0.f), (Animframe % 30 - 20) / 10.f).x;
		}
		else
		{
			fSpeed = 1.5f;
		}
	}
	static_cast<CMonster_Server*>(m_pGameObject)->Move_Dir(static_cast<CMonster_Server*>(m_pGameObject)->Get_Target_Direction(), fSpeed, fTimeDelta);
	return BT_RUNNING;
}

void CGolem_BT_Chase_Server::OnEnd()
{
	__super::OnEnd();
}



CGolem_BT_Chase_Server* CGolem_BT_Chase_Server::Create(void* pArg)
{
	CGolem_BT_Chase_Server* pInstance = new CGolem_BT_Chase_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Chase_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Chase_Server::Free()
{
	__super::Free();
}
