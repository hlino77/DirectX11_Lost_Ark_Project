#include "stdafx.h"
#include "Common_BT_Move.h"
#include "Monster.h"
#include "Model.h"

CCommon_BT_Move::CCommon_BT_Move()
{
}

void CCommon_BT_Move::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CCommon_BT_Move::OnUpdate(_float fTimeDelta)
{


	if(static_cast<CMonster*>(m_pGameObject)->Is_Close_To_RandomPosition())
		return BT_FAIL;

	static_cast<CMonster*>(m_pGameObject)->Move_to_RandomPosition(fTimeDelta);
	if (static_cast<CMonster*>(m_pGameObject)->Get_MonsterType() == CMonster::PAWN)
	{
		if (CSound_Manager::GetInstance()->Is_Channel_Playing(L"Pawn_3"))
			Add_Sound_Channel(L"Effect", L"Pawn_3", 0.2f);
	}
	return BT_RUNNING;
}

void CCommon_BT_Move::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Move* CCommon_BT_Move::Create(void* pArg)
{
	CCommon_BT_Move* pInstance = new CCommon_BT_Move;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Move");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Move::Free()
{
	__super::Free();
}
