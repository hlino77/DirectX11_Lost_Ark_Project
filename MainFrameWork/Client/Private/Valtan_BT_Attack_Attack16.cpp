#include "stdafx.h"
#include "Valtan_BT_Attack_Attack16.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>


CValtan_BT_Attack_Attack16::CValtan_BT_Attack_Attack16()
{
}

void CValtan_BT_Attack_Attack16::OnStart()
{
	__super::OnStart();
	m_vLandPosition = Vec3(95.5f, 0.f, 95.5f);
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack16::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		m_pGameObject->Get_TransformCom()->LookAt_Dir(XMVector3Normalize(m_vLandPosition - static_cast<CBoss*>(m_pGameObject)->Get_SpawnPosition()));
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vLandPosition);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex)
	{
		m_pGameObject->Get_TransformCom()->LookAt_ForLandObject(static_cast<CBoss*>(m_pGameObject)->Get_SpawnPosition());
		m_pGameObject->Get_TransformCom()->Go_Straight(static_cast<CBoss*>(m_pGameObject)->Get_MoveSpeed() * 1.1f, fTimeDelta);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[10].iAnimIndex&& m_fLoopTime < 1.5f)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack16::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack16* CValtan_BT_Attack_Attack16::Create(void* pArg)
{
	CValtan_BT_Attack_Attack16* pInstance = new CValtan_BT_Attack_Attack16;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack16::Free()
{
	__super::Free();
}
