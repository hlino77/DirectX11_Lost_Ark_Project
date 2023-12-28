#include "stdafx.h"
#include "Golem_BT_Attack_Charge_Punch.h"
#include "Monster.h"
#include "Model.h"
#include "ColliderSphere.h"
#include <ColliderOBB.h>
#include "Boss_Golem.h"

CGolem_BT_Attack_Charge_Punch::CGolem_BT_Attack_Charge_Punch()
{
}

void CGolem_BT_Attack_Charge_Punch::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CGolem_BT_Attack_Charge_Punch::OnUpdate(const _float& fTimeDelta)
{
	if ( m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimDesc[0].iAnimIndex)
	static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
 	if (!m_pGameObject->Get_Colider(CBoss_Golem::SKILL1)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && 5 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex))
	{
		CSphereCollider* pCollider = m_pGameObject->Get_Colider(CBoss_Golem::SKILL1);
		pCollider->Set_Radius(3.f);
		pCollider->SetActive(true);
		pCollider->Set_Offset(Vec3(0.0f, 1.f, 3.f));;
		COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(pCollider->Get_Child());
		pChildCollider->Set_Scale(Vec3(1.5f, 1.f, 3.f));
		pChildCollider->Set_Offset(Vec3(0.0f, 1.f, 3.f));
	}
	if (m_pGameObject->Get_Colider(CBoss_Golem::SKILL1)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && 20 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex))
	{
		CSphereCollider* pCollider = m_pGameObject->Get_Colider(CBoss_Golem::SKILL1);
		pCollider->SetActive(false);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && !static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(true);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);
	return __super::OnUpdate(fTimeDelta);
}

void CGolem_BT_Attack_Charge_Punch::OnEnd()
{
	dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
	__super::OnEnd();

}



CGolem_BT_Attack_Charge_Punch* CGolem_BT_Attack_Charge_Punch::Create(void* pArg)
{
	CGolem_BT_Attack_Charge_Punch* pInstance = new CGolem_BT_Attack_Charge_Punch;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Charge_Punch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Charge_Punch::Free()
{
	__super::Free();
}
