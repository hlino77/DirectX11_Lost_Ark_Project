#include "stdafx.h"
#include "Valtan_BT_Phase3.h"
#include "Monster.h"
#include "Model.h"
#include <Boss.h>
CValtan_BT_Phase3::CValtan_BT_Phase3()
{
}

void CValtan_BT_Phase3::OnStart()
{
	__super::OnStart(0);
	static_cast<CBoss*>(m_pGameObject)->Set_Phase(3);
	static_cast<CBoss*>(m_pGameObject)->Set_Hp(529402339);
	static_cast<CBoss*>(m_pGameObject)->Set_Armor(4);
}

CBT_Node::BT_RETURN CValtan_BT_Phase3::OnUpdate(const _float& fTimeDelta)
{

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Phase3::OnEnd()
{
	__super::OnEnd();
}


CValtan_BT_Phase3* CValtan_BT_Phase3::Create(void* pArg)
{
	CValtan_BT_Phase3* pInstance = new CValtan_BT_Phase3;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Phase3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Phase3::Free()
{
	__super::Free();
}
