#include "stdafx.h"
#include "Common_BT_Stand.h"
#include "Monster.h"
#include "Model.h"
Common_BT_Stand::Common_BT_Stand()
{
}

void Common_BT_Stand::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN Common_BT_Stand::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster*>(m_pGameObject)->Get_MonsterType() == CMonster::PAWN)
	{
		Add_Sound(0, 0, L"Effect", L"Pawn_50");
	}
	return __super::OnUpdate(fTimeDelta);
}

void Common_BT_Stand::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster*>(m_pGameObject)->Set_Left(false);

}



Common_BT_Stand* Common_BT_Stand::Create(void* pArg)
{
	Common_BT_Stand* pInstance = new Common_BT_Stand;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Common_BT_Stand::Free()
{
	__super::Free();
}
