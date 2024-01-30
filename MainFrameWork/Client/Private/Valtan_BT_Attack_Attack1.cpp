#include "stdafx.h"
#include "Valtan_BT_Attack_Attack1.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include "NavigationMgr.h"
#include "ColliderSphere.h"
#include <Boss_Valtan.h>

CValtan_BT_Attack_Attack1::CValtan_BT_Attack_Attack1()
{
}

void CValtan_BT_Attack_Attack1::OnStart()
{
	if (static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
	{
		__super::OnStart(1);
		m_pGameObject->Set_Render(true);
	}
	else
		__super::OnStart();
	m_iLoop = 0;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack1::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_iLoop < 2)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 48 && m_iLoop < 2)
	{
		m_iLoop++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_iLoop == 2 && static_cast<CBoss*>(m_pGameObject)->Get_Armor() < 1)
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(true);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 48 && m_iLoop < 3)
	{
		m_iLoop++;
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);
		if (!static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
		{
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(1.7f);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(0.46f, 0.f, -1.65f));
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-spine")));
			static_cast<CBoss*>(m_pGameObject)->Set_Atk(20);
			static_cast<CBoss*>(m_pGameObject)->Set_Force(52.f);
		}
		m_iCurrAnimation = 1;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime, m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_iCurrAnimation == 1)
	{
		m_pGameObject->Get_TransformCom()->Go_Straight(static_cast<CMonster*>(m_pGameObject)->Get_MoveSpeed() * 3.f, fTimeDelta);
		if (CNavigationMgr::GetInstance()->Is_Outside(m_pGameObject->Get_CurrLevel(), m_pGameObject, 1.3f))
		{
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(1.5f);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(1.42f, -0.8536f, -0.3f));
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("b_wp_r_01")));
			m_iCurrAnimation = 2;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
		}
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack1::OnEnd()
{
	__super::OnEnd();
	m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(1.5f);
	m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(1.42f, -0.8536f, -0.3f));
	m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("b_wp_r_01")));
	m_eReturn = RETURN_END;
	m_iLoop = 0;
	if (!static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
		static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack1* CValtan_BT_Attack_Attack1::Create(void* pArg)
{
	CValtan_BT_Attack_Attack1* pInstance = new CValtan_BT_Attack_Attack1;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack1::Free()
{
	__super::Free();
}
