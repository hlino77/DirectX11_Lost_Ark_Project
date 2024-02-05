#include "stdafx.h"
#include "Valtan_BT_Attack_CounterAttack.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include "ColliderSphere.h"

CValtan_BT_Attack_CounterAttack::CValtan_BT_Attack_CounterAttack()
{
}

void CValtan_BT_Attack_CounterAttack::OnStart()
{
	__super::OnStart();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(m_vecAnimDesc[0].strAnimName, m_vecAnimDesc[0].fChangeTime, m_vecAnimDesc[0].iStartFrame, m_vecAnimDesc[0].iChangeFrame, m_vecAnimDesc[0].fAnimSpeed);
	static_cast<CBoss*>(m_pGameObject)->Set_RimLight(0.1f, 0.7f);
	static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.1f, 0.7f);
}

CBT_Node::BT_RETURN CValtan_BT_Attack_CounterAttack::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.1f, 0.7f);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex )
	{
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.1f, 0.7f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)-> SetActive(true);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)-> Set_Radius(2.5f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)-> Set_Offset(Vec3(1.42f, -0.8536f, -0.3f));
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)-> Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("b_wp_r_01")));
		static_cast<CBoss*>(m_pGameObject)->Set_Atk(30);
		static_cast<CBoss*>(m_pGameObject)->Set_Force(19.f);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) >= 10 )
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	}
	if (m_vecAnimDesc[m_iCurrAnimation].bIsLoop && m_vecAnimDesc[m_iCurrAnimation].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
	{
		if (m_fLoopTime > m_vecAnimDesc[m_iCurrAnimation].fMaxLoopTime && m_vecAnimDesc[m_iCurrAnimation].IsEndInstant)
		{
			m_iCurrAnimation++;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
			m_fLoopTime = 0;
		}
		else if (m_fLoopTime > m_vecAnimDesc[m_iCurrAnimation].fMaxLoopTime && !m_vecAnimDesc[m_iCurrAnimation].IsEndInstant && m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
		{
			m_iCurrAnimation++;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
			m_fLoopTime = 0;
		}
		m_fLoopTime += fTimeDelta;
	}
	else if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) - 3&& !m_pGameObject->Get_ModelCom()->IsNext())
	{
		if (m_iCurrAnimation == m_iMaxAnimation - 1)
			return BT_SUCCESS;
		else
		{
			m_iCurrAnimation++;

			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
			m_pGameObject->Get_ModelCom()->Set_Anim_Speed(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fAnimSpeed);
			static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(m_vecAnimDesc[m_iCurrAnimation].strAnimName, m_vecAnimDesc[m_iCurrAnimation].fChangeTime, m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fAnimSpeed);
		}
	}
	return BT_RUNNING;
}

void CValtan_BT_Attack_CounterAttack::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_CounterAttack* CValtan_BT_Attack_CounterAttack::Create(void* pArg)
{
	CValtan_BT_Attack_CounterAttack* pInstance = new CValtan_BT_Attack_CounterAttack;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_CounterAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_CounterAttack::Free()
{
	__super::Free();
}
