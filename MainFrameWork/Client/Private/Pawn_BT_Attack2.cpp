#include "stdafx.h"
#include "Pawn_BT_Attack2.h"
#include "Monster.h"
#include "Model.h"
CPawn_BT_Attack2::CPawn_BT_Attack2()
{
}

void CPawn_BT_Attack2::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CPawn_BT_Attack2::OnUpdate(const _float& fTimeDelta)
{

	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[0].iAnimIndex))
		return BT_SUCCESS;
	if (16 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, true);

	if (26 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);


	return BT_RUNNING;
}

void CPawn_BT_Attack2::OnEnd()
{
	__super::OnEnd();
	dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
}



CPawn_BT_Attack2* CPawn_BT_Attack2::Create(void* pArg)
{
	CPawn_BT_Attack2* pInstance = new CPawn_BT_Attack2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CPawn_BT_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPawn_BT_Attack2::Free()
{
	__super::Free();
}
