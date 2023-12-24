#include "stdafx.h"
#include "Golem_BT_Attack_Swipe.h"
#include "Monster.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Monster_Golem.h"
CGolem_BT_Attack_Swipe::CGolem_BT_Attack_Swipe()
{
}

void CGolem_BT_Attack_Swipe::OnStart()
{
	__super::OnStart(0);

	static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction();


}

CBT_Node::BT_RETURN CGolem_BT_Attack_Swipe::OnUpdate(const _float& fTimeDelta)
{
	if (18 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[m_iCurrAnimation].iAnimIndex))
	{
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, true);
	}
	if (26 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[m_iCurrAnimation].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
	if (43 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[m_iCurrAnimation].iAnimIndex)&& !m_pGameObject->Get_Colider(CMonster_Golem::SKILL3)->IsActive())
	{
		CSphereCollider* pCollider = m_pGameObject->Get_Colider(CMonster_Golem::SKILL3);
		pCollider->Set_Radius(1.8f);
		pCollider->SetActive(true);
		pCollider->Set_Offset(Vec3(-0.4f, 1.f, 0.7f));;
		COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(pCollider->Get_Child());
		pChildCollider->Set_Orientation(Quaternion::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians( -35.f)));
		pChildCollider->Set_Scale(Vec3(0.5f, 1.f, 1.f));
		pChildCollider->Set_Offset(Vec3(-1.f, 1.f, 1.2f));
	}
	if (43 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[m_iCurrAnimation].iAnimIndex))
	{
		Vec3 vScale = dynamic_cast<COBBCollider*>(m_pGameObject->Get_Colider(CMonster_Golem::SKILL3)->Get_Child())->Get_Scale();
		vScale.x +=  fTimeDelta;		
		dynamic_cast<COBBCollider*>(m_pGameObject->Get_Colider(CMonster_Golem::SKILL3)->Get_Child())->Set_Scale(vScale);
	}
	return BT_RUNNING;
}

void CGolem_BT_Attack_Swipe::OnEnd()
{
	dynamic_cast<CMonster*>(m_pGameObject)->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->SetActive(false);
	m_pGameObject->Get_Colider(CMonster_Golem::SKILL3)->SetActive(false);
	__super::OnEnd();

}



CGolem_BT_Attack_Swipe* CGolem_BT_Attack_Swipe::Create(void* pArg)
{
	CGolem_BT_Attack_Swipe* pInstance = new CGolem_BT_Attack_Swipe;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Swipe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Swipe::Free()
{
	__super::Free();
}
