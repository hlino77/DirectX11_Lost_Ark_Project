#include "stdafx.h"
#include "Valtan_BT_Attack_Attack5.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include "GameInstance.h"
#include "ColliderSphere.h"

CValtan_BT_Attack_Attack5::CValtan_BT_Attack_Attack5()
{
}

void CValtan_BT_Attack_Attack5::OnStart()
{
	__super::OnStart();
	m_vDirection = 	static_cast<CMonster*>(m_pGameObject)->Get_TargetPos();
	static_cast<CMonster*>(m_pGameObject)->Set_SetuponCell(false);
	m_iStack = 0;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack5::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(1.5f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(0.46f, 0.f, -1.65f));
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-spine")));
		static_cast<CBoss*>(m_pGameObject)->Set_Atk(20);
		static_cast<CBoss*>(m_pGameObject)->Set_Force(52.f);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_fLoopTime > m_vecAnimDesc[1].fMaxLoopTime && m_iStack < 9)
	{
		m_iStack++;
		m_fLoopTime = 0.f;
		m_vDirection.Normalize();
		Vec3 vPlayerPosition = static_cast<CMonster*>(m_pGameObject)->Get_NearTarget()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		vPlayerPosition += (m_vDirection * 35.f);
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPlayerPosition);
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction();
		m_vDirection = Vec3::TransformNormal(m_vDirection, Matrix::CreateRotationY(XMConvertToRadians(205.f)));
		m_vDirection.Normalize();
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == (m_vecAnimDesc[2].iAnimIndex) && m_iStack==9)
	{
		m_iStack++;
		Vec3 vPlayerPosition = static_cast<CMonster*>(m_pGameObject)->Get_TargetPos();
		static_cast<CMonster*>(m_pGameObject)->Set_SetuponCell(true);
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, static_cast<CBoss*>(m_pGameObject)->Get_SpawnPosition() + m_vDirection * 1.4f);
		static_cast<CMonster*>(m_pGameObject)->Get_TransformCom()->LookAt(static_cast<CBoss*>(m_pGameObject)->Get_SpawnPosition());
	}

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack5::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
	static_cast<CMonster*>(m_pGameObject)->Set_SetuponCell(true);
}



CValtan_BT_Attack_Attack5* CValtan_BT_Attack_Attack5::Create(void* pArg)
{
	CValtan_BT_Attack_Attack5* pInstance = new CValtan_BT_Attack_Attack5;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack5::Free()
{
	__super::Free();
}
