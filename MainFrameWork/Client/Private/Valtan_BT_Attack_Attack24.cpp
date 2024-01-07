#include "stdafx.h"
#include "Valtan_BT_Attack_Attack24.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack24::CValtan_BT_Attack_Attack24()
{
}

void CValtan_BT_Attack_Attack24::OnStart()
{
	__super::OnStart(0);
	m_bTurn = false;
	m_fRotation = 0.f;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack24::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim()==m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_pGameObject->Get_TransformCom()->Turn_Speed(m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_UP),XMConvertToRadians(930.f),fTimeDelta);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack24::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack24* CValtan_BT_Attack_Attack24::Create(void* pArg)
{
	CValtan_BT_Attack_Attack24* pInstance = new CValtan_BT_Attack_Attack24;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack24::Free()
{
	__super::Free();
}
