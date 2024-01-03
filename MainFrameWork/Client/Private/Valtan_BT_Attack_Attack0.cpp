#include "stdafx.h"
#include "Valtan_BT_Attack_Attack0.h"
#include "Boss.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack0::CValtan_BT_Attack_Attack0()
{
}

void CValtan_BT_Attack_Attack0::OnStart()
{
	__super::OnStart(0);
	Vec3 vDirection = Vec3(-2.f, 0.f, -0.25f);
	vDirection.Normalize();
	m_vLandPosition = static_cast<CBoss*>(m_pGameObject)->Get_SpawnPosition() + vDirection * 3.f;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack0::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vLandPosition);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex)
		static_cast<CMonster*>(m_pGameObject)->Move_Dir(static_cast<CBoss*>(m_pGameObject)->Get_SpawnPosition()- m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION)
		,0.5f* static_cast<CMonster*>(m_pGameObject)->Get_MoveSpeed(),fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[10].iAnimIndex, m_fLoopTime < 1.5f)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack0::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack0* CValtan_BT_Attack_Attack0::Create(void* pArg)
{
	CValtan_BT_Attack_Attack0* pInstance = new CValtan_BT_Attack_Attack0;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack0::Free()
{
	__super::Free();
}
