#include "stdafx.h"
#include "Valtan_BT_Attack_Attack14.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>

CValtan_BT_Attack_Attack14::CValtan_BT_Attack_Attack14()
{
}

void CValtan_BT_Attack_Attack14::OnStart()
{
	__super::OnStart();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack14::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 1)
	{
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
	}
	else if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 2)
	{
		static_cast<CBoss*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, static_cast<CBoss*>(m_pGameObject)->Get_NearTarget_Position());
	}
	if (m_fLoopTime > m_vecAnimDesc[1].fMaxLoopTime)
	{
		static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(m_vecAnimDesc[2].strAnimName, m_vecAnimDesc[2].fChangeTime, m_vecAnimDesc[2].iStartFrame, m_vecAnimDesc[2].iChangeFrame, m_vecAnimDesc[2].fAnimSpeed);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack14::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack14* CValtan_BT_Attack_Attack14::Create(void* pArg)
{
	CValtan_BT_Attack_Attack14* pInstance = new CValtan_BT_Attack_Attack14;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack14::Free()
{
	__super::Free();
}
