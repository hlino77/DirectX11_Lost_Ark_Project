#include "stdafx.h"
#include "Golem_BT_Attack_Dash.h"
#include "Model.h"
#include "Transform.h"
#include "ColliderSphere.h"
#include <Boss.h>

CGolem_BT_Attack_Dash::CGolem_BT_Attack_Dash()
{
}

void CGolem_BT_Attack_Dash::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->Set_RootTargetDistance(0.f);

	m_iMaxSound = 3;
	for (size_t i = 0; i < m_iMaxSound; i++)
	{
		m_bSoundOn[i] = false;
	}
}

CBT_Node::BT_RETURN CGolem_BT_Attack_Dash::OnUpdate(const _float& fTimeDelta)
{
	Add_Sound(0, 0, TEXT("Effect"), TEXT("Rook_37"), 0, 0.5f);
	Add_Sound(0, 1, TEXT("Effect"), TEXT("Rook_8"), 15, 1.f);
	Add_Sound(0, 2, TEXT("Effect"), TEXT("Rook_40"), 15, 1.f);

	if (m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[0].iAnimIndex))
		return BT_SUCCESS;
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex)<10)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && 33 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex))
	{
		dynamic_cast<CMonster*>(m_pGameObject)->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
		dynamic_cast<CMonster*>(m_pGameObject)->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(1.6f);
		dynamic_cast<CMonster*>(m_pGameObject)->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(-0.f, 1.3f, 0.f));
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Atk(20);
		dynamic_cast<CBoss*>(m_pGameObject)->Set_Force(50.5f);
	}
	if (m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && 47 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, false);
	return BT_RUNNING;
}

void CGolem_BT_Attack_Dash::OnEnd()
{
	__super::OnEnd();
	dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, false);
	static_cast<CMonster*>(m_pGameObject)->Set_RootTargetDistance(static_cast<CMonster*>(m_pGameObject)->Get_AttackRange() * 0.7f);
	static_cast<CMonster*>(m_pGameObject)->Set_AttackRange(0);
	dynamic_cast<CMonster*>(m_pGameObject)->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	dynamic_cast<CMonster*>(m_pGameObject)->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(1.8f);
	dynamic_cast<CMonster*>(m_pGameObject)->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(-0.f, 1.3f, 0.7f));
}



CGolem_BT_Attack_Dash* CGolem_BT_Attack_Dash::Create(void* pArg)
{
	CGolem_BT_Attack_Dash* pInstance = new CGolem_BT_Attack_Dash;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Dash::Free()
{
	__super::Free();
}
