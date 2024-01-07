#include "stdafx.h"
#include "Valtan_BT_Attack_Attack17_2.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include <Boss.h>

CValtan_BT_Attack_Attack17_2::CValtan_BT_Attack_Attack17_2()
{
}

void CValtan_BT_Attack_Attack17_2::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack17_2::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_fLoopTime > 1.8f && !static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(true);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->IsNext() && static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack17_2::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack17_2* CValtan_BT_Attack_Attack17_2::Create(void* pArg)
{
	CValtan_BT_Attack_Attack17_2* pInstance = new CValtan_BT_Attack_Attack17_2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack17_2::Free()
{
	__super::Free();
}
