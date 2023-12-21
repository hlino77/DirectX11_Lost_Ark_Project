#include "stdafx.h"
#include "Golem_BT_Attack_Charge_Punch.h"
#include "Monster.h"
#include "Model.h"



CGolem_BT_Attack_Charge_Punch::CGolem_BT_Attack_Charge_Punch()
{
}

void CGolem_BT_Attack_Charge_Punch::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CGolem_BT_Attack_Charge_Punch::OnUpdate(const _float& fTimeDelta)
{
	if ( m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimIndexFrame[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimIndexFrame[0].iAnimIndex)
	static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);

	return __super::OnUpdate(fTimeDelta);
}

void CGolem_BT_Attack_Charge_Punch::OnEnd()
{
	__super::OnEnd();

}



CGolem_BT_Attack_Charge_Punch* CGolem_BT_Attack_Charge_Punch::Create(void* pArg)
{
	CGolem_BT_Attack_Charge_Punch* pInstance = new CGolem_BT_Attack_Charge_Punch;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Charge_Punch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Charge_Punch::Free()
{
	__super::Free();
}
