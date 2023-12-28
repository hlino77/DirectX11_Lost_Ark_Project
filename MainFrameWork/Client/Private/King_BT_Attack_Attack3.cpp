#include "stdafx.h"
#include "King_BT_Attack_Attack3.h"
#include "Monster.h"
#include "Model.h"
#include "ColliderSphere.h"

CKing_BT_Attack_Attack3::CKing_BT_Attack_Attack3()
{
}

void CKing_BT_Attack_Attack3::OnStart()
{
	__super::OnStart(0);


}

CBT_Node::BT_RETURN CKing_BT_Attack_Attack3::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta * 5.f);
	else if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex&& m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex)<51)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);

	if (m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && 76 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, false);
	else if (!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && 68 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, true);
	else if (m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && 51 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, false);
	else if (!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && 20 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, true);


	
	return __super::OnUpdate(fTimeDelta);;
}

void CKing_BT_Attack_Attack3::OnEnd()
{
	__super::OnEnd();

}



CKing_BT_Attack_Attack3* CKing_BT_Attack_Attack3::Create(void* pArg)
{
	CKing_BT_Attack_Attack3* pInstance = new CKing_BT_Attack_Attack3;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CKing_BT_Attack_Attack3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKing_BT_Attack_Attack3::Free()
{
	__super::Free();
}
