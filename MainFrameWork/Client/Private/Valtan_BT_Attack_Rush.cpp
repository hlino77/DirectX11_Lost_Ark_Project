#include "stdafx.h"
#include "Valtan_BT_Attack_Rush.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include "NavigationMgr.h"
#include "ColliderSphere.h"
#include <Boss_Valtan.h>
#include "Effect_Manager.h"
#include "Effect.h"
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

CValtan_BT_Attack_Rush::CValtan_BT_Attack_Rush()
{
}

void CValtan_BT_Attack_Rush::OnStart()
{
	if (static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
	{
		__super::OnStart(1);
		m_pGameObject->Set_Render(true);
	}
	else
		__super::OnStart();
	m_iLoop = 0;

	m_pEffectWarning = nullptr;
	m_EffectRush.clear();
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Rush::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_iLoop < 2)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 48 && m_iLoop < 2)
	{
		m_iLoop++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
		if (m_iLoop == 2)
		{
			vector<CEffect*> Effect;
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START_OUTLIST(L"ValtanRushWarning", &tDesc, Effect);
			m_pEffectWarning = Effect.front();
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_iLoop == 2 && static_cast<CBoss*>(m_pGameObject)->Get_Armor() < 1)
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(true);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 48 && m_iLoop < 3)
	{
		m_iLoop++;
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);
		if (!static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
		{
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(1.8f);	
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(0.46f, 0.f, -1.65f));
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-spine")));
			static_cast<CBoss*>(m_pGameObject)->Set_Atk(20);
			static_cast<CBoss*>(m_pGameObject)->Set_Force(52.f);
		}
		m_iCurrAnimation = 1;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime, m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);

		m_pEffectWarning->EffectEnd();
		m_pEffectWarning = nullptr;
		Add_Sound(L"Voice2", CHANNEL_EFFECT); 
		Add_Sound( L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#288 (673586812)", CHANNEL_EFFECT);
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START_OUTLIST(L"ValtanRush", &tDesc, m_EffectRush);
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_iCurrAnimation == 1)
	{
		if (m_EffectRush.empty() == false)
		{
			Matrix& matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			for (auto& Effect : m_EffectRush)
			{
				Effect->Update_Pivot(matWorld);
			}
		}
		
		m_pGameObject->Get_TransformCom()->Go_Straight(static_cast<CMonster*>(m_pGameObject)->Get_MoveSpeed() * 3.f, fTimeDelta);
		_float fOffset = 1.1f;
		if (!static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
		{
			static_cast<CBoss*>(m_pGameObject)->Set_RimLight(0.1f, 0.7f);
			static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.1f, 0.7f);
		}
		if(static_cast<CBoss*>(m_pGameObject)->Get_Phase()==1)
			fOffset = 1.1f;
		else
			fOffset = 2.f;
		if (CNavigationMgr::GetInstance()->Is_Outside(m_pGameObject->Get_CurrLevel(), m_pGameObject, fOffset))
		{
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(1.5f);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(1.42f, -0.8536f, -0.3f));
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("b_wp_r_01")));
			m_iCurrAnimation = 2;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);

			for (auto& Effect : m_EffectRush)
			{
				Effect->EffectEnd();
			}
			m_EffectRush.clear();                                              
		}
	}

	{
		Add_Sound(0, 0, L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#128 (662482722)", CHANNEL_EFFECT, 12);
		if(m_bSoundOn[0]&&m_iLoop== 1)
			Add_Sound(0, 1, L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#128 (662482722)", CHANNEL_EFFECT, 12);
		if (m_bSoundOn[1] && m_iLoop == 2)
			Add_Sound(0, 2, L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#128 (662482722)", CHANNEL_EFFECT, 12);

	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Rush::OnEnd()
{
	__super::OnEnd();		
	static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);
	m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(1.5f);
	m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(1.42f, -0.8536f, -0.3f));
	m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("b_wp_r_01")));
	m_eReturn = RETURN_END;
	m_iLoop = 0;
	if (!static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
		static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);

	if (m_iCurrAnimation == 1&& !static_cast<CBoss*>(m_pGameObject)->Is_Dummy() && static_cast<CBoss*>(m_pGameObject)->Get_GroggyGauge()>0)
	{
		for (auto& Effect : m_EffectRush)
		{
			Effect->EffectEnd();
		}
		m_EffectRush.clear();

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"ValtanRushCrash", &tDesc);

		if (static_cast<CBoss*>(m_pGameObject)->Get_BossType() == CBoss::BOSS_TYPE::VAlTAN)
		{
			CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
			if (nullptr == pPlayer)
				return;
			_float fEstherGauge = 20.f;
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

	if (m_pEffectWarning != nullptr)
	{
		m_pEffectWarning->EffectEnd();
		m_pEffectWarning = nullptr;
	}
}



CValtan_BT_Attack_Rush* CValtan_BT_Attack_Rush::Create(void* pArg)
{
	CValtan_BT_Attack_Rush* pInstance = new CValtan_BT_Attack_Rush;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Rush");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Rush::Free()
{
	__super::Free();
}
