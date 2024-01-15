#include "stdafx.h"
#include "King_BT_Attack_Attack2.h"
#include "Model.h"
#include "Transform.h"
#include "ColliderSphere.h"
#include <Boss.h>

CKing_BT_Attack_Attack2::CKing_BT_Attack_Attack2()
{
}

void CKing_BT_Attack_Attack2::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CKing_BT_Attack_Attack2::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction();

	if (!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && 20 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex))
	{
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Atk(18);
		dynamic_cast<CBoss*>(m_pGameObject)->Set_Force(12.f);
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, true);
	}
	if (m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && 30 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, false);

	return __super::OnUpdate(fTimeDelta);
}

void CKing_BT_Attack_Attack2::OnEnd()
{
	__super::OnEnd();
}



CKing_BT_Attack_Attack2* CKing_BT_Attack_Attack2::Create(void* pArg)
{
	CKing_BT_Attack_Attack2* pInstance = new CKing_BT_Attack_Attack2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CKing_BT_Attack_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKing_BT_Attack_Attack2::Free()
{
	__super::Free();
}
