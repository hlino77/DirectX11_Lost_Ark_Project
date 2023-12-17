#include "stdafx.h"
#include "Ghoul_BT_Attack_1.h"
#include "Monster.h"
#include "Transform.h"
#include "Model.h"

CGhoul_BT_Attack_1::CGhoul_BT_Attack_1()
{
}

void CGhoul_BT_Attack_1::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CGhoul_BT_Attack_1::OnUpdate(const _float& fTimeDelta)
{

	return  __super::OnUpdate(fTimeDelta);
}

void CGhoul_BT_Attack_1::OnEnd()
{
	__super::OnEnd();

}



CGhoul_BT_Attack_1* CGhoul_BT_Attack_1::Create(void* pArg)
{
	CGhoul_BT_Attack_1* pInstance = new CGhoul_BT_Attack_1;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGhoul_BT_Attack_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhoul_BT_Attack_1::Free()
{
	__super::Free();
}
