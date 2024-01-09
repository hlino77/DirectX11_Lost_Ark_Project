#include "stdafx.h"
#include "Valtan_BT_Attack_Attack5.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include <Boss.h>
#include "GameInstance.h"

CValtan_BT_Attack_Attack5::CValtan_BT_Attack_Attack5()
{
}

void CValtan_BT_Attack_Attack5::OnStart()
{
	__super::OnStart(0);
	m_vDirection = 	static_cast<CMonster*>(m_pGameObject)->Get_TargetPos();
	static_cast<CMonster*>(m_pGameObject)->Set_SetuponCell(false);
	m_iStack = 0;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack5::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_fLoopTime > m_vecAnimDesc[1].fMaxLoopTime && m_iStack < 9)
	{
		m_iStack++;
		m_fLoopTime = 0.f;
		Vec3 vPlayerPosition = static_cast<CMonster*>(m_pGameObject)->Get_NearTarget()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPlayerPosition + m_vDirection * 2.f);
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction();
		m_vDirection = Vec3::TransformNormal(m_vDirection, Matrix::CreateRotationY(XMConvertToRadians(195.f)));
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
