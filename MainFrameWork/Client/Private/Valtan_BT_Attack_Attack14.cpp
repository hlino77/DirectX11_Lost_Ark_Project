#include "stdafx.h"
#include "Valtan_BT_Attack_Attack14.h"
#include "Boss.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack14::CValtan_BT_Attack_Attack14()
{
}

void CValtan_BT_Attack_Attack14::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack14::OnUpdate(const _float& fTimeDelta)
{
	if ( m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack14::OnEnd()
{
	__super::OnEnd();
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
