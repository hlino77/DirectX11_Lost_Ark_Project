#include "stdafx.h"
#include "Boss_BT_ArmorBreak.h"
#include "Boss.h"
#include "Model.h"
CBoss_BT_ArmorBreak::CBoss_BT_ArmorBreak()
{
}

void CBoss_BT_ArmorBreak::OnStart()
{
	__super::OnStart(0);
	static_cast<CBoss*>(m_pGameObject)->Set_Armor(static_cast<CBoss*>(m_pGameObject)->Get_Armor() - 1);
}

CBT_Node::BT_RETURN CBoss_BT_ArmorBreak::OnUpdate(const _float& fTimeDelta)
{
	return __super::OnUpdate(fTimeDelta);
}

void CBoss_BT_ArmorBreak::OnEnd()
{
	__super::OnEnd();

}



CBoss_BT_ArmorBreak* CBoss_BT_ArmorBreak::Create(void* pArg)
{
	CBoss_BT_ArmorBreak* pInstance = new CBoss_BT_ArmorBreak;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBoss_BT_ArmorBreak");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_BT_ArmorBreak::Free()
{
	__super::Free();
}
