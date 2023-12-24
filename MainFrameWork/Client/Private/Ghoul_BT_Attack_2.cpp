#include "stdafx.h"
#include "Ghoul_BT_Attack_2.h"
#include "Monster.h"
#include "Transform.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"

CGhoul_BT_Attack_2::CGhoul_BT_Attack_2()
{
}

void CGhoul_BT_Attack_2::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction();

}

CBT_Node::BT_RETURN CGhoul_BT_Attack_2::OnUpdate(const _float& fTimeDelta)
{
	if (33 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[m_iCurrAnimation].iAnimIndex))
	{
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, true);
		CSphereCollider* pCollider = m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER);
		pCollider->Set_Radius(0.5f);
		pCollider->SetActive(true);
		pCollider->Set_Offset(Vec3(0.0f, 0.3f, 0.3f));
		COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(pCollider->Get_Child());
		pChildCollider->Set_Scale(Vec3(0.4f, 0.4f, 0.4f));
		pChildCollider->Set_Offset(Vec3(0.0f, 0.3f, 0.3f)); 
	}
	if (46 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[m_iCurrAnimation].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);

	return BT_RUNNING;
}

void CGhoul_BT_Attack_2::OnEnd()
{
	__super::OnEnd();

	static_cast<CMonster*>(m_pGameObject)->Set_AttackRange(0);
	dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
}



CGhoul_BT_Attack_2* CGhoul_BT_Attack_2::Create(void* pArg)
{
	CGhoul_BT_Attack_2* pInstance = new CGhoul_BT_Attack_2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGhoul_BT_Attack_2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhoul_BT_Attack_2::Free()
{
	__super::Free();
}
