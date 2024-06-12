#include "stdafx.h"
#include "King_BT_Attack_Attack4.h"
#include "Monster.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "Transform.h"
#include <Boss.h>

CKing_BT_Attack_Attack4::CKing_BT_Attack_Attack4()
{
}

void CKing_BT_Attack_Attack4::OnStart()
{
	__super::OnStart(0);
	m_IsTeleport = true;
}

CBT_Node::BT_RETURN CKing_BT_Attack_Attack4::OnUpdate(_float fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->IsNext())	
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta*5.f);
	if (m_IsTeleport && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		Vec3 vTargetPos = static_cast<CMonster*>(m_pGameObject)->Get_NearTarget_Position();
		Vec3 vDir = static_cast<CMonster*>(m_pGameObject)->Get_Target_Direction();
		vDir.Normalize();
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vTargetPos - vDir * 0.75f);
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction();
		Add_Sound(L"Effect", L"KING_23");
		Add_Sound(L"Effect", L"KING_26");
		m_IsTeleport = false;
	}

	if (!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && 38 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) && 51 > m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex))
	{
		Add_Sound(0, 2, L"Effect", L"KING_50");
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Atk(22);
		dynamic_cast<CBoss*>(m_pGameObject)->Set_Force(12.f);
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, true);
	}
	else if (m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && 51 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, false);

	if (!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && 28 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex))
	{
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Atk(24);
		dynamic_cast<CBoss*>(m_pGameObject)->Set_Force(12.f);
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, true);
	}
	if (m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && 36 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, false);
	Add_Sound(0, 0, L"Effect", L"KING_73");
	Add_Sound(0, 1, L"Effect", L"KING_29");

	return  __super::OnUpdate(fTimeDelta);;
}

void CKing_BT_Attack_Attack4::OnEnd()
{
	__super::OnEnd();
	dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, false);
}



CKing_BT_Attack_Attack4* CKing_BT_Attack_Attack4::Create(void* pArg)
{
	CKing_BT_Attack_Attack4* pInstance = new CKing_BT_Attack_Attack4;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CKing_BT_Attack_Attack4");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKing_BT_Attack_Attack4::Free()
{
	__super::Free();
}
