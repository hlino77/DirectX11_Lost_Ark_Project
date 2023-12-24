#include "stdafx.h"
#include "Zombie_BT_Attack2.h"
#include "Monster.h"
#include "Model.h"
CZombie_BT_Attack2::CZombie_BT_Attack2()
{
}

void CZombie_BT_Attack2::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction();
}

CBT_Node::BT_RETURN CZombie_BT_Attack2::OnUpdate(const _float& fTimeDelta)
{

	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].iAnimIndex))
		return BT_SUCCESS;
	if (45 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[m_iCurrAnimation].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, true);

	if (97 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[m_iCurrAnimation].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);


	return BT_RUNNING;
}

void CZombie_BT_Attack2::OnEnd()
{
	__super::OnEnd();
	dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
}



CZombie_BT_Attack2* CZombie_BT_Attack2::Create(void* pArg)
{
	CZombie_BT_Attack2* pInstance = new CZombie_BT_Attack2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_Attack2::Free()
{
	__super::Free();
}
