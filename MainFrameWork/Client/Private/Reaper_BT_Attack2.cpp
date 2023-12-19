#include "stdafx.h"
#include "Reaper_BT_Attack2.h"
#include "Monster.h"
#include "Model.h"
CReaper_BT_Attack2::CReaper_BT_Attack2()
{
}

void CReaper_BT_Attack2::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CReaper_BT_Attack2::OnUpdate(const _float& fTimeDelta)
{
	return __super::OnUpdate(fTimeDelta);
}

void CReaper_BT_Attack2::OnEnd()
{
	__super::OnEnd();


}



CReaper_BT_Attack2* CReaper_BT_Attack2::Create(void* pArg)
{
	CReaper_BT_Attack2* pInstance = new CReaper_BT_Attack2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CReaper_BT_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CReaper_BT_Attack2::Free()
{
	__super::Free();
}
