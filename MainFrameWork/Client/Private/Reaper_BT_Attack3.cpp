#include "stdafx.h"
#include "Reaper_BT_Attack3.h"
#include "Monster.h"
#include "Model.h"
#include "ColliderSphere.h"

CReaper_BT_Attack3::CReaper_BT_Attack3()
{
}

void CReaper_BT_Attack3::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CReaper_BT_Attack3::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->SetActive(true);

	if (m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && 14 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex))
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->SetActive(false);
	if (m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta * 5.f);
	return __super::OnUpdate(fTimeDelta);
}

void CReaper_BT_Attack3::OnEnd()
{
	__super::OnEnd();
	dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
}



CReaper_BT_Attack3* CReaper_BT_Attack3::Create(void* pArg)
{
	CReaper_BT_Attack3* pInstance = new CReaper_BT_Attack3;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CReaper_BT_Attack3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CReaper_BT_Attack3::Free()
{
	__super::Free();
}
