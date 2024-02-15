#include "stdafx.h"
#include "Boss_BT_Counter.h"
#include "Boss.h"
#include "Model.h"
#include "ServerSessionManager.h"
#include <Player_Gunslinger.h>
#include <Player_Slayer.h>
#include <Player_Destroyer.h>
#include <Player_Bard.h>
#include <Player_Doaga.h>
#include "Controller_MG.h"
#include "Controller_WDR.h"
#include "Controller_WR.h"
#include "Controller_SP.h"
#include "Player_Controller_GN.h"

CBoss_BT_Counter::CBoss_BT_Counter()
{
}

void CBoss_BT_Counter::OnStart()
{
	__super::OnStart(0);
	if (static_cast<CBoss*>(m_pGameObject)->Get_BossType() == CBoss::BOSS_TYPE::VAlTAN)
	{
		CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
		if (nullptr == pPlayer)
			return;
		_float fEstherGauge = 20.f;
		if (static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
			fEstherGauge *= 1.5f;
		if (pPlayer->Is_PartyLeader())
		{
			if (TEXT("Gunslinger") == pPlayer->Get_ObjectTag())
			{
				static_cast<CPlayer_Gunslinger*>(pPlayer)->Get_GN_Controller()->Get_EstherGageAddMessage(fEstherGauge);
			}
			else if (TEXT("WR") == pPlayer->Get_ObjectTag())
			{
				static_cast<CPlayer_Slayer*>(pPlayer)->Get_WR_Controller()->Get_EstherGageAddMessage(fEstherGauge);
			}
			else if (TEXT("WDR") == pPlayer->Get_ObjectTag())
			{
				static_cast<CPlayer_Destroyer*>(pPlayer)->Get_WDR_Controller()->Get_EstherGageAddMessage(fEstherGauge);
			}
			else if (TEXT("SP") == pPlayer->Get_ObjectTag())
			{
				static_cast<CPlayer_Doaga*>(pPlayer)->Get_SP_Controller()->Get_EstherGageAddMessage(fEstherGauge);
			}
		}
	}
}

CBT_Node::BT_RETURN CBoss_BT_Counter::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss*>(m_pGameObject)->Get_BossType() == CBoss::BOSS_TYPE::VAlTAN)
	{
		Add_Sound(0, 0, L"Effect", L"Valtan#421 (644163935)");
		Add_Sound(0, 1, L"Effect", L"Boss_Down");
		Add_Sound(2, 2, L"Effect", L"Valtan#17 (437813884)");
	}
	else if (static_cast<CBoss*>(m_pGameObject)->Get_BossType() == CBoss::BOSS_TYPE::KING)
	{
		Add_Sound(0, 0, L"Effect", L"KING_61");
		Add_Sound(0, 1, L"Effect", L"KING_58");
		Add_Sound(2, 2, L"Effect", L"KING_64");
	}
	return __super::OnUpdate(fTimeDelta);
}

void CBoss_BT_Counter::OnEnd()
{
	__super::OnEnd();
}



CBoss_BT_Counter* CBoss_BT_Counter::Create(void* pArg)
{
	CBoss_BT_Counter* pInstance = new CBoss_BT_Counter;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBoss_BT_Counter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_BT_Counter::Free()
{
	__super::Free();
}
