#include "stdafx.h"
#include "King_BT_Attack_Attack4.h"
#include "Monster.h"
#include "Model.h"
CKing_BT_Attack_Attack4::CKing_BT_Attack_Attack4()
{
}

void CKing_BT_Attack_Attack4::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CKing_BT_Attack_Attack4::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->IsNext())	
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta*5.f);

	return  __super::OnUpdate(fTimeDelta);;
}

void CKing_BT_Attack_Attack4::OnEnd()
{
	__super::OnEnd();
}



CKing_BT_Attack_Attack4* CKing_BT_Attack_Attack4::Create(void* pArg)
{
	CKing_BT_Attack_Attack4* pInstance = new CKing_BT_Attack_Attack4;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CKing_BT_Attack_Attack4");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKing_BT_Attack_Attack4::Free()
{
	__super::Free();
}
