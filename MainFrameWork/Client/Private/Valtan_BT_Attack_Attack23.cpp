#include "stdafx.h"
#include "Valtan_BT_Attack_Attack23.h"
#include "Boss.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack23::CValtan_BT_Attack_Attack23()
{
}

void CValtan_BT_Attack_Attack23::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack23::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex)
	{
		static_cast<CBoss*>(m_pGameObject)->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
	}
	if (m_iCurrAnimation == 5 && m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CBoss*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (m_iCurrAnimation == m_iMaxAnimation - 1)
		m_pGameObject->Set_Invincible(true);
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack23::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack23* CValtan_BT_Attack_Attack23::Create(void* pArg)
{
	CValtan_BT_Attack_Attack23* pInstance = new CValtan_BT_Attack_Attack23;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack23::Free()
{
	__super::Free();
}
