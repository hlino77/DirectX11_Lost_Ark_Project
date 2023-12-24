#include "stdafx.h"
#include "Reaper_BT_Attack2.h"
#include "Monster.h"
#include "Model.h"
#include "ColliderSphere.h"

CReaper_BT_Attack2::CReaper_BT_Attack2()
{
}

void CReaper_BT_Attack2::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction();

}

CBT_Node::BT_RETURN CReaper_BT_Attack2::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimIndexFrame[0].iAnimIndex)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimIndexFrame[2].iAnimIndex)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->SetActive(true);

	if (m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimIndexFrame[2].iAnimIndex && 27 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[2].iAnimIndex))
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->SetActive(false);
	return __super::OnUpdate(fTimeDelta);
}

void CReaper_BT_Attack2::OnEnd()
{
	__super::OnEnd();
	dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);

}



CReaper_BT_Attack2* CReaper_BT_Attack2::Create(void* pArg)
{
	CReaper_BT_Attack2* pInstance = new CReaper_BT_Attack2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CReaper_BT_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CReaper_BT_Attack2::Free()
{
	__super::Free();
}
