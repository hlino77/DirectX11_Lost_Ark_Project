#include "stdafx.h"
#include "Ghoul_BT_Attack_3.h"
#include "Monster.h"
#include "Transform.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"

CGhoul_BT_Attack_3::CGhoul_BT_Attack_3()
{
}

void CGhoul_BT_Attack_3::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CGhoul_BT_Attack_3::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[0].iAnimIndex))
		return BT_SUCCESS;
	static_cast<CMonster*>(m_pGameObject)->Set_AttackRange(0);
	if (25 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
	{
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, true);
		CSphereCollider* pCollider = m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER);
		pCollider->Set_Radius(0.5f);
		pCollider->SetActive(true);
		pCollider->Set_Offset(Vec3(0.0f, 0.7f, 0.3f));
		pCollider->Get_Child()->SetActive(false);
		COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(pCollider->Get_Child());
		pChildCollider->Set_Scale(Vec3(0.4f, 0.4f, 0.4f));
		pChildCollider->Set_Offset(Vec3(0.0f, 0.7f, 0.3f));
	}
	if (59 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
	if (m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta * 5.f);
	return BT_RUNNING;
}

void CGhoul_BT_Attack_3::OnEnd()
{
	__super::OnEnd();
	dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
	static_cast<CMonster*>(m_pGameObject)->Set_AttackRange(0);

}



CGhoul_BT_Attack_3* CGhoul_BT_Attack_3::Create(void* pArg)
{
	CGhoul_BT_Attack_3* pInstance = new CGhoul_BT_Attack_3;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGhoul_BT_Attack_3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhoul_BT_Attack_3::Free()
{
	__super::Free();
}
