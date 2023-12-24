#include "stdafx.h"
#include "Golem_BT_Attack_Jump.h"
#include "Monster.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "Monster_Golem.h"
#include <ColliderOBB.h>
CGolem_BT_Attack_Jump::CGolem_BT_Attack_Jump()
{
}

void CGolem_BT_Attack_Jump::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->Set_RootTargetDistance(0.f);
	static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction();

}

CBT_Node::BT_RETURN CGolem_BT_Attack_Jump::OnUpdate(const _float& fTimeDelta)
{
	if (!m_pGameObject->Get_Colider(CMonster_Golem::SKILL1)->IsActive()  && 36 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[0].iAnimIndex))
	{
		CSphereCollider* pCollider = m_pGameObject->Get_Colider(CMonster_Golem::SKILL1);
		pCollider->Set_Radius(2.f);
		pCollider->SetActive(true);
		pCollider->Set_Offset(Vec3(0.0f, 1.f, 0.f));;
		COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(pCollider->Get_Child());
		pChildCollider->Set_Scale(Vec3(1.f, 1.f, 2.f));
		pChildCollider->Set_Offset(Vec3(0.0f, 1.f, 0.f));
		CSphereCollider* pCollider2 = m_pGameObject->Get_Colider(CMonster_Golem::SKILL2);
		pCollider2->Set_Radius(2.f);
		pCollider2->SetActive(true);
		pCollider2->Set_Offset(Vec3(0.f, 1.f, 0.f));;
		COBBCollider* pChildCollider2 = dynamic_cast<COBBCollider*>(pCollider2->Get_Child());
		pChildCollider2->Set_Scale(Vec3(2.f, 1.f, 1.f));
		pChildCollider2->Set_Offset(Vec3(0.0f, 1.f, 0.f));
	}

	if (36 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[0].iAnimIndex))
	{
		Vec3 vScale = dynamic_cast<COBBCollider*>(m_pGameObject->Get_Colider(CMonster_Golem::SKILL1)->Get_Child())->Get_Scale(); 
		vScale.z += 4.f * fTimeDelta;
		dynamic_cast<COBBCollider*>(m_pGameObject->Get_Colider(CMonster_Golem::SKILL1)->Get_Child())->Set_Scale(vScale);

		vScale = dynamic_cast<COBBCollider*>(m_pGameObject->Get_Colider(CMonster_Golem::SKILL2)->Get_Child())->Get_Scale();
		vScale.x += 4.f * fTimeDelta;
		dynamic_cast<COBBCollider*>(m_pGameObject->Get_Colider(CMonster_Golem::SKILL2)->Get_Child())->Set_Scale(vScale);

	}
	return BT_RUNNING;
}

void CGolem_BT_Attack_Jump::OnEnd()
{
	__super::OnEnd();
	m_pGameObject->Get_Colider(CMonster_Golem::SKILL1)->SetActive(false);
	m_pGameObject->Get_Colider(CMonster_Golem::SKILL2)->SetActive(false);
	dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
	static_cast<CMonster*>(m_pGameObject)->Set_RootTargetDistance(static_cast<CMonster*>(m_pGameObject)->Get_AttackRange() * 0.7f);

}



CGolem_BT_Attack_Jump* CGolem_BT_Attack_Jump::Create(void* pArg)
{
	CGolem_BT_Attack_Jump* pInstance = new CGolem_BT_Attack_Jump;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Jump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Jump::Free()
{
	__super::Free();
}
