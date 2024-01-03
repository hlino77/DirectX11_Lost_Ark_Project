#include "stdafx.h"
#include "Valtan_BT_Attack_Attack8.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack8::CValtan_BT_Attack_Attack8()
{
}

void CValtan_BT_Attack_Attack8::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack8::OnUpdate(const _float& fTimeDelta)
{
	
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack8::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack8* CValtan_BT_Attack_Attack8::Create(void* pArg)
{
	CValtan_BT_Attack_Attack8* pInstance = new CValtan_BT_Attack_Attack8;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack8::Free()
{
	__super::Free();
}
