#include "stdafx.h"
#include "King_BT_Attack_Charge_Swing.h"
#include "Monster.h"
#include "Transform.h"
#include "Model.h"
CKing_BT_Attack_Charge_Swing::CKing_BT_Attack_Charge_Swing()
{
}

void CKing_BT_Attack_Charge_Swing::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CKing_BT_Attack_Charge_Swing::OnUpdate(const _float& fTimeDelta)
{


	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimIndexFrame[1].first.iAnimIndex)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);

	return  __super::OnUpdate(fTimeDelta);
}

void CKing_BT_Attack_Charge_Swing::OnEnd()
{
	__super::OnEnd();

}



CKing_BT_Attack_Charge_Swing* CKing_BT_Attack_Charge_Swing::Create(void* pArg)
{
	CKing_BT_Attack_Charge_Swing* pInstance = new CKing_BT_Attack_Charge_Swing;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CKing_BT_Attack_Charge_Swing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKing_BT_Attack_Charge_Swing::Free()
{
	__super::Free();
}
