#include "stdafx.h"
#include "King_BT_Attack_Attack3.h"
#include "Monster.h"
#include "Model.h"
CKing_BT_Attack_Attack3::CKing_BT_Attack_Attack3()
{
}

void CKing_BT_Attack_Attack3::OnStart()
{
	__super::OnStart(0);


}

CBT_Node::BT_RETURN CKing_BT_Attack_Attack3::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	return __super::OnUpdate(fTimeDelta);;
}

void CKing_BT_Attack_Attack3::OnEnd()
{
	__super::OnEnd();

}



CKing_BT_Attack_Attack3* CKing_BT_Attack_Attack3::Create(void* pArg)
{
	CKing_BT_Attack_Attack3* pInstance = new CKing_BT_Attack_Attack3;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CKing_BT_Attack_Attack3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKing_BT_Attack_Attack3::Free()
{
	__super::Free();
}
