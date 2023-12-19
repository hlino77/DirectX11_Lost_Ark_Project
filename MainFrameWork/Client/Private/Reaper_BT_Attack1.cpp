#include "stdafx.h"
#include "Reaper_BT_Attack1.h"
#include "Monster.h"
#include "Model.h"

CReaper_BT_Attack1::CReaper_BT_Attack1()
{
}

void CReaper_BT_Attack1::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CReaper_BT_Attack1::OnUpdate(const _float& fTimeDelta)
{
	return __super::OnUpdate(fTimeDelta);
}

void CReaper_BT_Attack1::OnEnd()
{
	__super::OnEnd();

}



CReaper_BT_Attack1* CReaper_BT_Attack1::Create(void* pArg)
{
	CReaper_BT_Attack1* pInstance = new CReaper_BT_Attack1;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CReaper_BT_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CReaper_BT_Attack1::Free()
{
	__super::Free();
}
