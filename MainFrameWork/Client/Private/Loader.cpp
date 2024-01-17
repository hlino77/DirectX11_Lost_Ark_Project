#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "AsFileUtils.h"
#include "AsUtils.h"
#include <filesystem>
#include "NavigationMgr.h"
#include "QuadTreeMgr.h"

/* 플레이어 */
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Weapon_WR.h"

#include "Player_Gunslinger.h"
#include "Weapon_Hand.h"
#include "Weapon_Hand_2.h"
#include "Weapon_Long.h"
#include "Weapon_Shot.h"
#include "Weapon_Shot_2.h"
#include "Player_Controller_GN.h"

#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Weapon_WDR.h"

#include "Player_Bard.h"
#include "Controller_MG.h"
#include "Weapon_MG.h"

#include "SkyDome.h"

/* 유틸*/
#include "Camera_Free.h"
#include "StaticModel.h"
#include "ServerSession.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSessionManager.h"
#include "Camera_Player.h"
#include "BackGround_Server.h"
#include "Camera_Lobby.h"


#include "Projectile.h"

//UI
#include "UI_ServerWnd.h"
#include "UI_ServerGrid.h"
#include "UI_ServerLogo.h"
#include "UI_ServerEntranceButton.h"
#include "UI_Server.h"
#include "UI_Loading.h"
#include "UI_LoadingFill.h"
#include "UI_TextBox.h"
#include "BackGround_Lobby.h"
#include "UI_Lobby_NickName.h"
#include "UI_Lobby_EntranceServer_Button.h"
#include "UI_Lobby_StageName.h"
#include "UI_Lobby_NickNameChange.h"
#include "UI_Lobby.h"
#include "UI_ChatFrame.h"
#include "UI_ChatWriteFrame.h"
#include "UI_ChatLanguageIcon.h"
#include "UI_ChatSmallerButton.h"
#include "UI_Manager.h"
#include "UI_IdentityGN_BackGroundWing.h"
#include "UI_IdentityGN_BackGroundFrame.h"
#include "UI_IdentityGN_BackGroundShine.h"
#include "UI_IdentityGN_MainFrame.h"
#include "UI_IdentityGN_ShineFrame.h"
#include "UI_IdentityGN_WF_Front.h"
#include "UI_Identity_GN.h"
#include "UI_IdentityGN_Arrow.h"
#include "UI_IdentityGN_Spark.h"
#include "UI_Chat.h"
#include "UI_Player_HPEmpty.h"
#include "UI_Player_HPFill.h"
#include "UI_Player_HPFrame.h"
#include "UI_Player_HP.h"
#include "UI_Player_MPEmpty.h"
#include "UI_Player_MPFill.h"
#include "UI_Player_MPFrame.h"
#include "UI_Player_MP.h"
#include "UI_SpeechBubble.h"
#include "UI_SkillIcon_Frame.h"
#include "UI_ItemIcon_Frame.h"
#include "UI_ETCIcon_Frame.h"
#include "UI_QuickSkill.h"
#include "UI_Skill_MoveIcon.h"
#include "UI_SpaceBar_Icon.h"
#include "UI_WRIdentity_Body.h"
#include "UI_WRIdentity.h"
#include "UI_WRIdentity_GaugeFrame.h"
#include "UI_WRIdentity_Gauge.h"
#include "UI_WRIdentity_GaugeShine.h"
#include "UI_WRIdentity_Spark.h"
#include "UI_WRIdentity_EyeShine.h"
#include "UI_WRIdentity_SkillIcon.h"
#include "UI_WRIdentity_Boomb.h"
#include "UI_WRIdentity_BindingKey.h"
#include "UI_WDRIdentity_BackGround.h"
#include "UI_WDRIdentity_GaugeFrame.h"
#include "UI_WDRIdentity_Gauge.h"
#include "UI_WDRIdentity_Hammer.h"
#include "UI_WDRIdentity_Chain.h"
#include "UI_WDRIdentity_Bubbles_Frame.h"
#include "UI_WDRIdentity_Bubble.h"
#include "UI_WDRIdentity_Identity_Skill.h"
#include "UI_WDRIdentity.h"
#include "UI_IdentityMG_BackGround.h"
#include "UI_IdentityMG_Roap.h"
#include "UI_IdentityMG_Harp.h"
#include "UI_IdentityMG_Gauge.h"
#include "UI_IdentityMG_Skill.h"
#include "UI_IdentityMG_SkillFrame.h"
#include "UI_IdentityMG.h"
#include "UI_ChaosDungeon_GaugeFrame.h"
#include "UI_ChaosDungeon_GaugeShine.h"
#include "UI_ChaosDungeon_GaugeSpinShine.h"
#include "UI_ChaosDungeon_Gauge.h"
#include "UI_ChaosDungeon_Gauge_Cut.h"
#include "UI_ChaosDungeon_NameFrame.h"
#include "UI_ChaosDungeon_TimerFrame.h"
#include "UI_ChaosDungeon.h"
#include "UI_ChaosDungeon_Clear.h"
#include "UI_InGame_NamePlate.h"
#include "UI_Boss_Hp.h"
#include "UI_Monster_Hp.h"
#include "UI_NPC_ChaosDungeon.h"
#include "UI_NPC_ChaosDungeon_NewWnd.h"

//Monsters
#include "Monster_Zombie.h"
#include "Monster_Plant.h"
#include "Boss_Golem.h"
#include "Monster_Ghoul.h"
#include <Monster_Reaper.h>
#include "Boss_King.h"
#include "Boss_Valtan.h"
#include <Monster_Pawn.h>

//Weapons
#include "Weapon_Boss_King.h"
#include "Weapon_Mn_Reaper.h"
#include <Weapon_Mn_PawnShield.h>
#include <Weapon_Mn_PawnSword.h>
#include "Weapon_Boss_Valtan.h"

//BossSkills
#include "SKill_King_ChargeSwing.h"
#include "SKill_King_Eruption.h"
#include <SKill_Golem_Charge_Punch.h>
#include <SKill_Golem_Jump.h>
#include <SKill_Golem_Swipe.h>
#include "SKill_Valtan_Doughnut.h"
#include "SKill_Valtan_Breath.h"


//Lobby MQ
#include "Player_Select_GN.h"
#include "Player_Select_MG.h"
#include "Player_Select_WDR.h"
#include "Player_Select_WR.h"
#include "Tea.h"


//Effect
#include "VoidEffect.h"
#include "Effect_Custom_SpiralChaser.h"
#include "Effect_Custom_Grenade.h"
#include "Effect_Custom_PerpectShotBullet.h"
#include "Effect_Custom_CrossHair.h"
#include "Effect_Custom_DeathFireBomb.h"

//NPC
#include "tinyxml2.h"
#include "Deco_Npc.h"
#include "Guide_Chaos_Npc.h"
#include "Npc_Part.h"

namespace fs = std::filesystem;

CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

// unsigned(__stdcall* _beginthreadex_proc_type)(void*);

_uint APIENTRY ThreadEntry(void* pArg)
{
	
	CoInitializeEx(nullptr, 0);

	/* 새롭게 생성된 스레드가 일한다. */
	CLoader*		pLoader = (CLoader*)pArg;

	pLoader->Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevel)
{
	InitializeCriticalSection(&m_Critical_Section);

	m_eNextLevel = eNextLevel;

	/* 새로운 스레드를 만들자. */
	/* 스레드 : 내 코드를 읽어주는 하나의 흐름? */
	/* 3 : 생성한 스레드가 호출해야하는 함수의 주소 */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntry, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

_int CLoader::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr = 0;

	switch (m_eNextLevel)
	{
	case LEVEL_TOOL:
		hr = Loading_For_Level_Tool();
		break;
	case LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;
	case LEVEL_SERVERSELECT:
		hr = Loading_For_Level_ServerSelect();
		break;
	case LEVEL_LOBBY:
		hr = Loading_For_Level_Lobby();
		break;
	case LEVEL_BERN:
		hr = Loading_For_Level_Bern();
		break;
	case LEVEL_CHAOS_1:
		hr = Loading_For_Level_Chaos1();
		break;
	case LEVEL_CHAOS_2:
		hr = Loading_For_Level_Chaos2();
		break;
	case LEVEL_CHAOS_3:
		hr = Loading_For_Level_Chaos3();
		break;
	case LEVEL_TOOL_NPC:
		hr = Loading_For_Level_Tool_Npc();
		break;
	case LEVEL_VALTANMAIN:
		hr = Loading_For_Level_ValtanMain();
		break;
	}

	if (FAILED(hr))
		return -1;	

	LeaveCriticalSection(&m_Critical_Section);

	return 0;	
}

HRESULT CLoader::Loading_For_Level_Logo()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_Frame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chat/Chatting_Frame.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_WritingFrame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chat/Chatting_WritingFrame.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_ReceiverSelect"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chat/Chatting_ReceiverSelect%d.png", 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_Smaller"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chat/Chatting_Smaller%d.png",4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_State"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chat/Chatting_State%d.png", 2))))
		return E_FAIL;

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChatFrame"),
		CUI_ChatFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChatWriteFrame"),
		CUI_ChatWriteFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChatLanguageIcon"),
		CUI_ChatLanguageIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChatSmallerButton"),
		CUI_ChatSmallerButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChatUI"),
		CUI_Chat::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityGN_BackGroundWing"),
		CUI_IdentityGN_BackGroundFWing::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityGN_BackGround"),
		CUI_IdentityGN_BackGroundFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityGN_BackGroundShine"),
		CUI_IdentityGN_BackGroundShine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityGN_Arrow"),
		CUI_IdentityGN_Arrow::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityGN_MainFrame"),
		CUI_IdentityGN_MainFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityGN_ShineFrame"),
		CUI_IdentityGN_ShineFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityGN_WF_Front"),
		CUI_IdentityGN_WF_Front::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityGN_WF_Spark"),
		CUI_IdentityGN_Spark::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityGNUI"),
		CUI_IdentityGN::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HPEmpty"),
		CUI_Player_HPEmpty::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HPFill"),
		CUI_Player_HPFill::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HPFrame"),
		CUI_Player_HPFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerHPUI"),
		CUI_Player_HP::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MPEmpty"),
		CUI_Player_MPEmpty::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MPFill"),
		CUI_Player_MPFill::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MPFrame"),
		CUI_Player_MPFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerMPUI"),
		CUI_Player_MP::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WRIdentity_Body"),
		CUI_WRIdentity_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WRIdentity_GaugeFrame"),
		CUI_WRIdentity_GaugeFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WRIdentity_Gauge"),
		CUI_WRIdentity_Gauge::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WRIdentity_GaugeShine"),
		CUI_WRIdentity_GaugeShine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WRIdentity_EyeShine"),
		CUI_WRIdentity_EyeShine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WRIdentity_Spark"),
		CUI_WRIdentity_Spark::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WRIdentity_SkillIcon"),
		CUI_WRIdentity_SkillIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WRIdentity_Boomb"),
		CUI_WRIdentity_Boomb::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WRIdentity_BindingKey"),
		CUI_WRIdentity_BindingKey::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WRIdentity"),
		CUI_WRIdentity::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WDRIdentity_BackGround"),
		CUI_WDRIdentity_BackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WDRIdentity_GaugeFrame"),
		CUI_WDRIdentity_GaugeFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WDRIdentity_Gauge"),
		CUI_WDRIdentity_Gauge::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WDRIdentity_Hammer"),
		CUI_WDRIdentity_Hammer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WDRIdentity_Chain"),
		CUI_WDRIdentity_Chain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WDRIdentityy_BubblesFrame"),
		CUI_WDRIdentity_Bubbles_Frame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WDRIdentity_Bubble"),
		CUI_WDRIdentity_Bubble::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WDRIdentity_Skill"),
		CUI_WDRIdentity_Identity_Skill::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WDRIdentity_UI"),
		CUI_WDRIdentity::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityMG_BackGround"),
		CUI_IdentityMG_BackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityMG_Roap"),
		CUI_IdentityMG_Roap::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityMG_Harp"),
		CUI_IdentityMG_Harp::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityMG_Gauge"),
		CUI_IdentityMG_Gauge::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityMG_Skill"),
		CUI_IdentityMG_Skill::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityMG_SkillFrame"),
		CUI_IdentityMG_SkillFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_IdentityMG_UI"),
		CUI_IdentityMG::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Tool()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StaticModel"),
		CStaticModel::Create(m_pDevice, m_pContext, PROP))))
		return E_FAIL;

	if (FAILED(Load_MapData(LEVEL_TOOL, TEXT("../Bin/Resources/MapData/Chaos1.data"))))
		return E_FAIL;

	/* For.Model */
	m_strLoading = TEXT("모델을 로딩 중 입니다.");
	

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	{
		/*wstring strTextureEffectFilePath = TEXT("../Bin/Resources/Effects/FX_Textures/");
		for (const auto& category : filesystem::directory_iterator(strTextureEffectFilePath))
		{
			if (!category.is_directory())
				continue;
			for (const auto& entry : filesystem::directory_iterator(category.path()))
			{
				if (!entry.is_regular_file() || "desktop" == entry.path().stem().generic_string())
					continue;

				const wstring& strFileName = entry.path().stem();
				wstring strComponentName = L"Prototype_Component_Texture_" + strFileName;
				wstring strFinalPath = entry.path().generic_wstring();

				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL, strComponentName,
					CTexture::Create(m_pDevice, m_pContext, strFinalPath))))
					return E_FAIL;
			}
		}*/
	}

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");


	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_VoidEffect"),
		CVoidEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkyDome"),
		CSkyDome::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Tool_Npc()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StaticModel"),
	CStaticModel::Create(m_pDevice, m_pContext, PROP))))
	return E_FAIL;

	m_strLoading = TEXT("맵을 로딩 중 입니다.");
	CNavigationMgr::GetInstance()->Add_Navigation(LEVELID::LEVEL_TOOL_NPC, L"BernCastle.Navi");

	if (FAILED(Load_MapData(LEVEL_TOOL_NPC, TEXT("../Bin/Resources/MapData/BernCastle.data"))))
		return E_FAIL;

	/* For.Model */
	m_strLoading = TEXT("모델을 로딩 중 입니다.");
	Loading_Model_For_Level_Tool_Npc();

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");


	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NamePlate"),
		CUI_InGame_NamePlate::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DecoNpc"),
		CDeco_Npc::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Guide_Chaos_Npc"),
		CGuide_Chaos_Npc::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NpcPart"),
		CNpc_Part::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	while (true)
	{
		if (m_Futures.empty())
			break;

		for (auto iter = m_Futures.begin(); iter != m_Futures.end();)
		{
			if (iter->valid())
			{
				if (iter->wait_for(std::chrono::seconds(1)) == future_status::ready)
				{
					if (FAILED(iter->get()))
						return E_FAIL;

					iter = m_Futures.erase(iter);
				}
				else
					++iter;
			}
			else
				++iter;
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_ServerSelect()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	// 로비를 서버셀렉트에서 로딩
	Loading_Model_For_Level_Lobby();

	/* For.Component*/
	m_strLoading = TEXT("컴포넌트를 로딩 중 입니다.");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Controller_GN"),
		CPlayer_Controller_GN::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Controller_WR"),
		CController_WR::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Controller_WDR"),
		CController_WDR::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Controller_MG"),
		CController_MG::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Select_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/BackGround/Server_BackGround/%d.jpg", 149))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Select_Wnd"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Server_Select/Server_SelectWnd.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Select_WaitingWnd"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Server_Select/Server_Select_Waiting_Wnd.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Select_Grid"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Server_Select/Server_Select_Grid.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Logo_Grid"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Server_Select/Logo_Grid.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Select_GridWnd"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Server_Select/Server_GridWnd.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Button_Exit"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Button_UI/Exit%d.png",4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Button_Option"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Button_UI/Option%d.png", 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Button_Return"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Button_UI/Return%d.png", 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_JusinLogo"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Logo/Jusin.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_LostArkLogo"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Logo/LostArk_Logo.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Server_EntranceButton"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Button_UI/Button%d.png",2))))
		return E_FAIL;

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");
	//BackGround
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_Server"),
		CBackGround_Server::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//UI
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ServerWnd"),
		CUI_ServerWnd::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ServerGrid"),
		CUI_ServerGrid::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ServerLogo"),
		CUI_ServerLogo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ServerEntranceButton"),
		CUI_ServerEntranceButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_TextBox"),
	//	CUI_TextBox::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ServerUI"),
		CUI_Server::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingUI"),
		CUI_Loading::Create(m_pDevice, m_pContext))))
		return E_FAIL;*/




	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tea"),
		CTea::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//Mannequin
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Select_GN"),
		CPlayer_Select_GN::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Select_WR"),
		CPlayer_Select_WR::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Select_WDR"),
		CPlayer_Select_WDR::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Select_MG"),
		CPlayer_Select_MG::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	while (true)
	{
		if (m_Futures.empty())
			break;

		for (auto iter = m_Futures.begin(); iter != m_Futures.end();)
		{
			if (iter->valid())
			{
				if (iter->wait_for(std::chrono::seconds(1)) == future_status::ready)
				{
					if (FAILED(iter->get()))
						return E_FAIL;

					iter = m_Futures.erase(iter);
				}
				else
					++iter;
			}
			else
				++iter;
		}
	}


	Safe_Release(pGameInstance);

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Lobby()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Character_Lobby"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/BackGround/Lobby/Character_Lobby.png"))))
		return E_FAIL;

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StaticModel_Lobby"),
		CStaticModel::Create(m_pDevice, m_pContext, OBJ_TYPE::PROP))))
		return E_FAIL; 
	
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Lobby"),
		CCamera_Lobby::Create(m_pDevice, m_pContext,TEXT("Camera_Lobby")))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_Lobby"),
		CBackGround_Lobby::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Load_MapData(LEVEL_LOBBY, TEXT("../Bin/Resources/MapData/Character_Select_Lobby_ver2.data"));

	Loading_LobbyUI();

	Safe_Release(pGameInstance);


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Bern()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);
	pUIManager->Set_MaxFiles(423);

	if (FAILED(Loading_QuickSlot()))
		return E_FAIL;

	if (FAILED(Loading_Npc_UI()))
		return E_FAIL;

	//Load_MapData(TEXT(LEVEL_BERN, "../Bin/Resources/MapData/Bern.data"));
	CNavigationMgr::GetInstance()->Add_Navigation(LEVELID::LEVEL_BERN, L"BernCastle.Navi");
	pUIManager->Add_CurrFile();

	Load_MapData(LEVEL_BERN, TEXT("../Bin/Resources/MapData/BernCastle.data"));
	//CNavigationMgr::GetInstance()->Add_Navigation(TEXT("Level_Chaos_Navi"), L"Chaos1.Navi");
	pUIManager->Add_CurrFile();

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_CrossHair"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Effects/FX_Textures/Mask/fx_y_gbs_03_r.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_CrossHairOutCircle"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Effects/FX_Textures/Mask/fx_e_ring_012_cl.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(Loading_ChaosDungeon_UI()))
		return E_FAIL;

	/* For.Mesh */
	m_strLoading = TEXT("메시를 로딩 중 입니다.");

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	m_strLoading = TEXT("모델을 로딩 중 입니다.");
	Loading_Model_For_Level_Bern();

	m_strLoading = TEXT("객체 원형을 로딩 중 입니다.");

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pDevice, m_pContext, L"Player_Camera"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Gunslinger"),
		CPlayer_Gunslinger::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_WR"),
		CPlayer_Slayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_WDR"),
		CPlayer_Destroyer::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_MG"),
		CPlayer_Bard::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WP_WR_Base"),
		CWeapon_WR::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WDR_WP_Base"),
		CWeapon_WDR::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MG_WP_Base"),
		CWeapon_MG::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Zombie"),
		CMonster_Zombie::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Plant"),
		CMonster_Plant::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Hand"),
		CWeapon_Hand::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Hand_2"),
		CWeapon_Hand_2::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Long"),
		CWeapon_Long::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Shot"),
		CWeapon_Shot::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Shot_2"),
		CWeapon_Shot_2::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StaticModel"),
		CStaticModel::Create(m_pDevice, m_pContext, PROP))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Golem"),
		CBoss_Golem::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SKill_Golem_Charge_Punch"),
		CSKill_Golem_Charge_Punch::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SKill_Golem_Jump"),
		CSKill_Golem_Jump::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SKill_Golem_Swipe"),
		CSKill_Golem_Swipe::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Ghoul"),
		CMonster_Ghoul::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Reaper"),
		CMonster_Reaper::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Mn_Reaper"),
		CWeapon_Mn_Reaper::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_King"),
		CBoss_King::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SKill_King_ChargeSwing"),
		CSKill_King_ChargeSwing::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SKill_King_Eruption"),
		CSKill_King_Eruption::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Boss_King"),
		CWeapon_Boss_King::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Pawn"),
		CMonster_Pawn::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Mn_PawnShield"),
		CWeapon_Mn_PawnShield::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Mn_PawnSword"),
		CWeapon_Mn_PawnSword::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Valtan"),
		CBoss_Valtan::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Boss_Valtan"),
		CWeapon_Boss_Valtan::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SKill_Valtan_Doughnut"),
		cSKill_Valtan_Doughnut::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SKill_Valtan_Breath"),
		CSKill_Valtan_Breath::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkyDome"),
		CSkyDome::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NamePlate"),
		CUI_InGame_NamePlate::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Custom_SpiralChaser"),
		CEffect_Custom_SpiralChaser::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Custom_Grenade"),
		CEffect_Custom_Grenade::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Custom_PerpectShotBullet"),
		CEffect_Custom_PerpectShotBullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Custom_CrossHair"),
		CEffect_Custom_CrossHair::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect_Custom_DeathFireBomb"),
		CEffect_Custom_DeathFireBomb::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	//Load_MapData(LEVEL_ARENA, L"../Bin/Resources/MapData/Arena.data");
	//Load_ColMesh(LEVEL_ARENA, L"../Bin/Resources/ColMeshData/Arena.data");

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DecoNpc"),
		CDeco_Npc::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Guide_Chaos_Npc"),
		CGuide_Chaos_Npc::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NpcPart"),
		CNpc_Part::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	while (true)
	{
		if (m_Futures.empty())
			break;

		for (auto iter = m_Futures.begin(); iter != m_Futures.end();)
		{
			if (iter->valid())
			{
				if (iter->wait_for(std::chrono::seconds(1)) == future_status::ready)
				{
					if (FAILED(iter->get()))
						return E_FAIL;

					iter = m_Futures.erase(iter);
					pUIManager->Add_CurrFile();

				}
				else
					++iter;
			}
			else
				++iter;
		}
	}

	if (FAILED(Load_NpcData()))
		return E_FAIL;


	Safe_Release(pUIManager);
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Chaos1()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);
	pUIManager->Set_MaxFiles(10);

	CNavigationMgr::GetInstance()->Add_Navigation(LEVELID::LEVEL_CHAOS_1, L"Chaos1.Navi");
	pUIManager->Add_CurrFile();
	
	Load_MapData(LEVEL_CHAOS_1, TEXT("../Bin/Resources/MapData/Chaos1.data"));
	pUIManager->Add_CurrFile();

	{
		wstring strFileName = L"Monster_Ghoul";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_1, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}


	{

		wstring strFileName = L"Monster_Reaper";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_1, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}

	{
		wstring strFileName = L"Reaper_Wp";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_1, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	Safe_Release(pUIManager);
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Chaos2()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);
	pUIManager->Set_MaxFiles(5);

	//CNavigationMgr::GetInstance()->Add_Navigation(LEVELID::LEVEL_CHAOS_2, L"Arena.Navi");

	CNavigationMgr::GetInstance()->Add_Navigation(LEVELID::LEVEL_CHAOS_2, L"Chaos2.Navi");
	pUIManager->Add_CurrFile();

	Load_MapData(LEVEL_CHAOS_2, TEXT("../Bin/Resources/MapData/Chaos2.data"));
	pUIManager->Add_CurrFile();


	{
		wstring strFileName = L"Monster_Plant";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_2, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}


	{
		wstring strFileName = L"Monster_Zombie";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_2, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}


	{
		wstring strFileName = L"Boss_Golem";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_2, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}



	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	Safe_Release(pUIManager);
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Chaos3()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);

	pUIManager->Set_MaxFiles(7);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	CNavigationMgr::GetInstance()->Add_Navigation(LEVELID::LEVEL_CHAOS_3, L"Chaos3.Navi");
	pUIManager->Add_CurrFile();

	Load_MapData(LEVEL_CHAOS_3, TEXT("../Bin/Resources/MapData/Chaos3.data"));
	pUIManager->Add_CurrFile();

	{
		wstring strFileName = L"Boss_King";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_3, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}

	{
		wstring strFileName = L"Boss_Wp_KingSword";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_3, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(180.0f))* XMMatrixRotationX(XMConvertToRadians(80.0f))))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}
	{

		wstring strFileName = L"Monster_Pawn";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_3, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}

	{
		wstring strFileName = L"Weapon_Mn_PawnShield";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_3, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}
	{
		wstring strFileName = L"Weapon_Mn_PawnSword";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_3, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	Safe_Release(pUIManager);
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_For_Level_ValtanMain()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);
	pUIManager->Set_MaxFiles(10);

	CNavigationMgr::GetInstance()->Add_Navigation(LEVELID::LEVEL_VALTANMAIN, L"Chaos1.Navi");
	pUIManager->Add_CurrFile();

	Load_MapData(LEVEL_VALTANMAIN, TEXT("../Bin/Resources/MapData/Chaos1.data"));
	pUIManager->Add_CurrFile();

	{
		wstring strFileName = L"Boss_Valtan";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_VALTANMAIN, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}

	{
		wstring strFileName = L"Boss_Valtan_Parts1";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_VALTANMAIN, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}

	{
		wstring strFileName = L"Boss_Valtan_Parts2";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_VALTANMAIN, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}

	{
		wstring strFileName = L"Boss_Valtan_Ghost";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_VALTANMAIN, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}

	{
		wstring strFileName = L"Wp_Boss_Valtan";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_VALTANMAIN, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Add_CurrFile();
	}

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	Safe_Release(pUIManager);
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Load_NpcData()
{
	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);

	wstring strLoadpath = (L"../Bin/Resources/ObjectData/Npc/");

	for (const auto& entry : fs::directory_iterator(strLoadpath))
	{
		if (fs::is_regular_file(entry))
		{
			if (L".dat" == entry.path().extension())
			{
				if (FAILED(Start_Load_Npc(entry.path())))
				{
					return E_FAIL;
				}
				pUIManager->Add_CurrFile();

			}
		}
	}
	Safe_Release(pUIManager);
	return S_OK;
}

HRESULT CLoader::Start_Load_Npc(const wstring& strPath)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CNpc::NPCDESC	NpcCreateDesc;

	shared_ptr<CAsFileUtils> LoadObject = make_shared<CAsFileUtils>();
	LoadObject->Open(strPath, FileMode::Read);

	string strData;
	LoadObject->Read<_uint>(NpcCreateDesc.iNpcType);
	LoadObject->Read(strData);
	NpcCreateDesc.strNpcTag = CAsUtils::S2W(strData);
	LoadObject->Read<Matrix>(NpcCreateDesc.matStart);

	LoadObject->Read<_uint>(NpcCreateDesc.iNpcShape);
	LoadObject->Read(strData);
	NpcCreateDesc.strNpcMq = CAsUtils::S2W(strData);
	LoadObject->Read(strData);
	NpcCreateDesc.strNpcHead = CAsUtils::S2W(strData);
	LoadObject->Read(strData);
	NpcCreateDesc.strNpcBody = CAsUtils::S2W(strData);

	_uint	iSize;
	LoadObject->Read<_bool>(NpcCreateDesc.IsMove);
	LoadObject->Read<_uint>(iSize);
	for (size_t i = 0; i < iSize; i++)
	{
		Vec3 vPos;
		LoadObject->Read<Vec3>(vPos);
		NpcCreateDesc.vecMovePos.push_back(vPos);
	}

	LoadObject->Read<_bool>(NpcCreateDesc.bUseWeaponPart);
	LoadObject->Read(strData);
	NpcCreateDesc.strLeftPart = CAsUtils::S2W(strData);
	LoadObject->Read<Matrix>(NpcCreateDesc.Left_OffsetMatrix);
	LoadObject->Read(strData);
	NpcCreateDesc.strRightPart = CAsUtils::S2W(strData);
	LoadObject->Read<Matrix>(NpcCreateDesc.Right_OffsetMatrix);

	LoadObject->Read<_bool>(NpcCreateDesc.IsTalk);
	LoadObject->Read<_float>(NpcCreateDesc.fTalkStartTime);

	{
		fs::path strPath = fs::path("../Bin/Resources/ObjectData/Npc/" + CAsUtils::ToString(NpcCreateDesc.strNpcTag) + ".xml");

		shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();
		tinyxml2::XMLError error = document->LoadFile(strPath.generic_string().c_str());
		if (error != tinyxml2::XML_SUCCESS)
		{
			return E_FAIL;
		}

		tinyxml2::XMLElement* root = nullptr;
		root = document->FirstChildElement();
		tinyxml2::XMLElement* node = nullptr;
		node = root->FirstChildElement();
		{
			tinyxml2::XMLElement* element = nullptr;

			element = node->FirstChildElement();
			if (element->GetText())
			{
				wstring strInfo = CAsUtils::S2W(element->GetText());
				if (strInfo.length() > 0)
				{
					NpcCreateDesc.strNpcName = strInfo;
				}
			}

			element = element->NextSiblingElement();
			if (element->GetText())
			{
				wstring strInfo = CAsUtils::S2W(element->GetText());
				if (strInfo.length() > 0)
				{
					NpcCreateDesc.strIdleAnim = strInfo;
				}
			}

			element = element->NextSiblingElement();
			if (element->GetText())
			{
				wstring strInfo = CAsUtils::S2W(element->GetText());
				if (strInfo.length() > 0)
				{
					NpcCreateDesc.strActAnim = strInfo;
				}
			}

			element = element->NextSiblingElement();
			NpcCreateDesc.fChangeAnimTime = element->FloatAttribute("Time");
		}

		node = node->NextSiblingElement();
		{
			tinyxml2::XMLElement* element = nullptr;

			element = node->FirstChildElement();
			NpcCreateDesc.fTalkStartTime = element->FloatAttribute("Time");

			_uint iSize;
			element = element->NextSiblingElement();
			iSize = element->IntAttribute("Size");

			for (size_t i = 0; i < iSize; i++)
			{
				element = element->NextSiblingElement();
				if (element->GetText())
				{
					wstring strTalk = CAsUtils::S2W(element->GetText());
					if (strTalk.length() > 0)
					{
						NpcCreateDesc.vecTalks.push_back(strTalk);
					}
				}
			}
		}

		node = node->NextSiblingElement();
		{
			tinyxml2::XMLElement* element = nullptr;

			_uint iSize;
			element = node->FirstChildElement();
			iSize = element->IntAttribute("Size");

			for (size_t i = 0; i < iSize; i++)
			{
				element = element->NextSiblingElement();
				if (element->GetText())
				{
					wstring strTalk = CAsUtils::S2W(element->GetText());
					if (strTalk.length() > 0)
					{
						NpcCreateDesc.vecTalkSound.push_back(strTalk);
					}
				}
			}
		}
	}

	NpcCreateDesc.iCurLevel = (_uint)LEVELID::LEVEL_BERN;

	if ((_uint)CNpc::NPCTYPE::DECO == NpcCreateDesc.iNpcType)
	{
		CGameObject* pInstance = pGameInstance->Add_GameObject(NpcCreateDesc.iCurLevel, (_uint)LAYER_TYPE::LAYER_NPC,
			TEXT("Prototype_GameObject_DecoNpc"), &NpcCreateDesc);
		if (nullptr == pInstance)
			return E_FAIL;
	}
	else if ((_uint)CNpc::NPCTYPE::FUNCTION == NpcCreateDesc.iNpcType)
	{
		if (TEXT("Guide_Chaos_Npc") == NpcCreateDesc.strNpcTag)
		{
			CGameObject* pInstance = pGameInstance->Add_GameObject(NpcCreateDesc.iCurLevel, (_uint)LAYER_TYPE::LAYER_NPC,
				TEXT("Prototype_GameObject_Guide_Chaos_Npc"), &NpcCreateDesc);
			if (nullptr == pInstance)
				return E_FAIL;
		}
	}

	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::AutoLoad(const fs::path& strPath, LEVELID eLevel, Matrix Pivot)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/*_uint iFolderCnt = 0;
	for (const auto& entry : fs::directory_iterator(strPath))
	{
		if (fs::is_directory(entry))
		{
			iFolderCnt++;
		}
	}*/
	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);

	for (const auto& entry : fs::directory_iterator(strPath))
	{
		if (fs::is_directory(entry))
		{
			{
				wstring strFileName = entry.path().filename();
				wstring strFilePath = entry.path().parent_path();
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(eLevel, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(eLevel, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath + L"/", strFileName, true, false, Pivot))))
						return E_FAIL;
					pUIManager->Add_CurrFile();

				}
			}
		}
	}

	Safe_Release(pUIManager);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Load_MapData(LEVELID eLevel, const wstring& szFilePath)
{
	// file Open
	// Read Data File
	// Create Prototype Object
	// Create Prototype Model

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFilePath, FileMode::Read);

	Vec3	QuadTreePosition = {};
	Vec3	QuadTreeScale = {};
	_uint	QuadTreeMaxDepth = {};


	file->Read<Vec3>(QuadTreePosition);
	file->Read<Vec3>(QuadTreeScale);
	file->Read<_uint>(QuadTreeMaxDepth);

	//CQuadTreeMgr::GetInstance()->Make_QaudTree(QuadTreePosition, QuadTreeScale, QuadTreeMaxDepth);



	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));

	
	vector<wstring> paths =
	{
	L"../Bin/Resources/Export/Bern/",
	L"../Bin/Resources/Export/Chaos1/",
	L"../Bin/Resources/Export/Chaos2/",
	L"../Bin/Resources/Export/Chaos3/",
	L"../Bin/Resources/Export/Boss/",
	L"../Bin/Resources/Export/Lobby/"
	};


	_uint iSize = file->Read<_uint>();
    bool fileFound = false;

	for (_uint i = 0; i < iSize; ++i)
	{

		string strFileName = file->Read<string>();
		wstring selectedPath = {};

		for (const auto& path : paths)
		{
			wstring fullPath = path + CAsUtils::ToWString(strFileName);
		
			if (std::filesystem::exists(fullPath))
			{
				selectedPath = path;
			}
		}

		if (selectedPath.empty())
		{
			MessageBox(g_hWnd, L"File not found in any specified paths.", L"Error", MB_OK);
			return E_FAIL;
		}

		Matrix	matWorld = file->Read<Matrix>();
		_bool bInstance = false;
		file->Read<_bool>(bInstance);

		vector<_uint>	QuadTreeIndex;
		_uint			QuadTreeSize = {};
		file->Read<_uint>(QuadTreeSize);
	

		for (size_t i = 0; i < QuadTreeSize; i++)
		{
			_uint Index = {};
			file->Read<_uint>(Index);
		}



		CStaticModel::MODELDESC Desc;
		Desc.strFileName = CAsUtils::ToWString(strFileName);
		Desc.strFilePath = selectedPath;
		

		wstring strComponentName = L"Prototype_Component_Model_" + Desc.strFileName;

		if (FAILED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{

		}
		else
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, Desc.strFilePath, Desc.strFileName, true, true, PivotMatrix))))
				return E_FAIL;
		}

	}
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Load_ColMesh(LEVELID eLevel, const wstring& szFilePath)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFilePath, FileMode::Read);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(-90.0f));


	_uint iSize = file->Read<_uint>();

	for (_uint i = 0; i < iSize; ++i)
	{
		wstring szModelName = CAsUtils::ToWString(file->Read<string>());
		Matrix	matWorld = file->Read<Matrix>();

		wstring szModelPath = L"../Bin/Resources/Meshes/Static/";

		wstring strComponentName = L"Prototype_Component_Model_" + szModelName;

		if (FAILED(pGameInstance->Check_Prototype(eLevel, strComponentName)))
		{
			_uint iColCount = file->Read<_uint>();
			for (_uint i = 0; i < iColCount; ++i)
			{
				Vec3 vOffset = file->Read<Vec3>();
				_float fRadius = file->Read<_float>();


				if (file->Read<_bool>())
				{
					Vec3 vOffset = file->Read<Vec3>();
					Vec3 vScale = file->Read<Vec3>();
					Quaternion vQuat = file->Read<Quaternion>();
				}
			}
			continue;
		}


		if (FAILED(pGameInstance->Add_Prototype(eLevel, strComponentName,
			CModel::Create(m_pDevice, m_pContext, szModelPath, szModelName, true, true, PivotMatrix))))
			continue;


		_uint iColCount = file->Read<_uint>();

		for (_uint i = 0; i < iColCount; ++i)
		{
			Vec3 vOffset = file->Read<Vec3>();
			_float fRadius = file->Read<_float>();


			if (file->Read<_bool>())
			{
				Vec3 vOffset = file->Read<Vec3>();
				Vec3 vScale = file->Read<Vec3>();
				Quaternion vQuat = file->Read<Quaternion>();
			}
		}
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_LobbyUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NickName_Frame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Character_Select/Character_Name.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NickName_ShineEffect"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Character_Select/NickName_Frame%d.png",2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Current_TownFrame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Character_Select/Current_TownFrame.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Emblem"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Character_Select/Emblem%d.png",4 ))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Equipment_Level"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Character_Select/Equipment_Level.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NickName_Changer"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Character_Select/NickName_Changer.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NickName_Change"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Character_Select/NickName_Change%d.png",2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_NickName"),
		CUI_Lobby_NickName::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_StageName"),
		CUI_Lobby_StageName::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_EntranceButton"),
		CUILobby_Entrance_to_ServrerButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby_NameChanger"),
		CUI_Lobby_NickNameChange::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Lobby"),
		CUI_Lobby::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_QuickSlot()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);

	//SkillFrame
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Empty"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/Skill_Empty%d.png", 2))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Shine"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/Skill_Shine.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Frame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/Skill_Frame.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	//ItemFrame
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_Empty"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Slot/Item_Empty.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_SlotFrame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Slot/Item_SlotFrame.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_SlotFrame_Shine"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Slot/Item_SlotFrame_Shine.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	//ETC
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ETC_Empty"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Slot/ETC_Empty.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ETC_SlotFrame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Slot/ETC_SlotFrame.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	//Identity
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_BackGround.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_BackGround_Wing"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_BackGroundWing.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_BackGround_Shine"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_BackGround_Shine.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_MainFrame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_WF_MainFrame%d.png",3))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_WeaponFrame_ShineEffect"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_WeaponFrame_ShineEffect%d.png",3))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_WeaponFrame_Front"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_WeaponFrame_Front.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_Arrow"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_Arrow.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_ETC"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_ETC.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_SkillCoolTimer"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_SkillCoolTimer%d.png",4))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_WeaponChange_Spark"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_WeaponChange_Spark.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hp_Empty"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Hp_Empty.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hp_Fill"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Hp_Fill.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hp_Frame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Hp_Frame.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mp_Empty"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Mp_Empty.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mp_Fill"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Mp_Fill.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mp_Frame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Mp_Frame.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Shield_Fill"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Shield_Fill.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Slayer/WRIdentity/Identity_Body%d.png",44))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_Gauge"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Slayer/Identity_Gauge.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_GaugeFrame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Slayer/Identity_GaugeFrame.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_GaugeShine"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Slayer/Identity_GaugeShine.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_Identity_Spark"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Slayer/Spark/Identity_Spark%d.png",45))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_Identity_Eye"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Slayer/Identity_Eye.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_Identity_Eye"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Slayer/Identity_Eye.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_Identity_Identity_SkillFrame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Slayer/Identity_SkillFrame.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_Spark_Effect"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Slayer/Identity_Spark_Effect.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_Identity_BindingKey"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Slayer/BindingKey.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	//Destroyer
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Identity_MainFrame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Destroyer/Identity_MainFrame%d.png",2))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Identity_GaugeFrame_LR"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Destroyer/Identity_GaugeFrame_LR.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Identity_Gauge_LR"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Destroyer/Identity_Gauge_LR.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Identity_Hammer"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Destroyer/Identity_Hammer%d.png",2))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Identity_Bubbles_Frame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Destroyer/Identity_Bubbles_Frame.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Identity_Bubble"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Destroyer/Identity_Bubble.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Identity_On_BlueEffect"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Destroyer/Identity_On_BlueEffect.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Identity_WhiteEffect"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Destroyer/Identity_WhiteEffect.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Vortex_Gravity"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Destroyer/Vortex_Gravity.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Chain"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Destroyer/Chain/Chain%d.png",15))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Chain_Cut"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Destroyer/Chain_Cut/Chain_Cut%d.png", 50))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	//Bard
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_Background_Shadow"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Bard_Identity_Background_Shadow.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_Background"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Bard_Identity_Background.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_Harp_Roap"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Bard_Identity_Harp_Roap.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_Harp_Roapknot"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Bard_Identity_Harp_Roapknot.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_Harp"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Bard_Identity_Harp.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_HarpHolder"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Bard_Identity_HarpHolder.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_SkillIconHolder"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Bard_Identity_SkillIconHolder.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_Serenade_of_Courage"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Serenade_of_Courage.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_Serenade_of_Salvation"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Serenade_of_Salvation.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_SkillFrame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Bard_Identity_SkillFrame.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_SkillICap"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Bard_Identity_SkillICap.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_GaugeEmpty"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Bard_Identity_GaugeEmpty.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_Gauge"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Bard_Identity_Gauge%d.png",2))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_GaugeFrame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Bard_Identity_GaugeFrame.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_GaugeCut"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Bard/Bard_Identity_GaugeCut%d.png",2))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	Safe_Release(pUIManager);
	Safe_Release(pGameInstance);

	if (FAILED(Loading_SkillIcon()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ChaosDungeon_UI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);

	//Texture
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chaos_Dungeon_Gauge_Frame"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Gauge_Frame/Gauge_Frame.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chaos_Dungeon_Gague_BackGround_Shine"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Gague_BackGround_Shine/Gague_BackGround_Shine%d.png", 50))))
			return E_FAIL; 
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chaos_Dungeon_Gague_BackGround_SpinShine"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Gauge_Circle/Gauge_Circle%d.png", 78))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chaos_Dungeon_Gauge"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Gauge_Frame/Gauge.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chaos_Dungeon_Gauge_Cut"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Gauge_Frame/Gauge_Cut.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chaos_Dungeon_Dungeon_NameFrame"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Gauge_Frame/Dungeon_NameFrame.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chaos_Dungeon_Dungeon_TimerFrame"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Timer.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Emblem"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Monster_Hp/Boss_Emblem%d.png",3))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_Hp"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Monster_Hp/Boss_Hp%d.png",8))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Boss_HpFrame"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Monster_Hp/Boss_HpFrame%d.png", 3))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Groggy_GaugeFrame"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Monster_Hp/Groggy_GaugeFrame.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Groggy_Gauge"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Monster_Hp/Groggy_Gauge.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_HpFrame"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Monster_Hp/Monster_HpFrame.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Hp"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Monster_Hp/Monster_Hp.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Clear_Circle"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Clear_Circle/Clear_Circle%d.png",23))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Clear_Line"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Clear_Line.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Line_L"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Line_L.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Line_R"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Line_R.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Clear_Shine"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Clear_Shine.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Clear_Announce"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Clear_Announce.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ClearFont"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/ClearFont.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BackGroundWnd"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/BackGroundWnd.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Clear_CheckButton"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chaos_Dungeon/Clear_CheckButton%d.png",3))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

	}

	//Class
	{
		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChaosDungeon_GaugeFrame"),
			CUI_ChaosDungeon_GaugeFrame::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChaosDungeon_GaugeShine"),
			CUI_ChaosDungeon_GaugeShine::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChaosDungeon_GaugeSpinShine"),
			CUI_ChaosDungeon_GaugeSpinShine::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChaosDungeon_Gauge"),
			CUI_ChaosDungeon_Gauge::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChaosDungeon_GaugeCut"),
			CUI_ChaosDungeon_GaugeCut::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChaosDungeon_NameFrame"),
			CUI_ChaosDungeon_NameFrame::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChaosDungeon_TimerFrame"),
			CUI_ChaosDungeon_TimerFrame::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChaosDungeonUI"),
			CUI_ChaosDungeon::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BossHpUI"),
			CUI_Boss_Hp::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MonsterHpUI"),
			CUI_Monster_Hp::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ChaosDungeon_Clear"),
			CUI_ChaosDungeon_Clear::Create(m_pDevice, m_pContext))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

	}

	Safe_Release(pUIManager);
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_Npc_UI()
{
	if (FAILED(Loading_Npc_UI_Texture()))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Npc_ChaosDungeonEntrance"),
		CUI_NPC_ChaosDungeon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Npc_ChaosDungeonEntrance_NewWnd"),
		CUI_NPC_ChaosDungeon_NewWnd::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	Safe_Release(pUIManager);
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_Npc_UI_Texture()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	
	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_ChaosDungeon_Wnd"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Npc/ChaosDungeon_Entrance/ChaosDungeon_Wnd.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Button"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Npc/ChaosDungeon_Entrance/Entrance_Button%d.png",2))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Anounce"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Npc/ChaosDungeon_Entrance/Anounce.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Stage_Button"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Npc/ChaosDungeon_Entrance/Stage_Button.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Seleceted_Effect"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Npc/ChaosDungeon_Entrance/Seleceted_Effect.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_DropItem_Frame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Npc/ChaosDungeon_Entrance/DropItem_Frame.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Stage_Screen"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Npc/ChaosDungeon_Entrance/reverseruin_30000_bern.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Stage_Screen"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Npc/ChaosDungeon_Entrance/reverseruin_30000_bern.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_NewWnd_Text"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Npc/ChaosDungeon_Entrance/NewWnd/NewWnd_Text.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Accept_Button"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Npc/ChaosDungeon_Entrance/NewWnd/Accept_Button%d.png",3))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Refuse_Button"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Npc/ChaosDungeon_Entrance/NewWnd/Refuse_Button%d.png", 3))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Timer_Empty"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Npc/ChaosDungeon_Entrance/NewWnd/Timer_Empty.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_Timer"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Npc/ChaosDungeon_Entrance/NewWnd/Timer.png"))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	Safe_Release(pUIManager);
	Safe_Release(pGameInstance);
	return S_OK;
}



HRESULT CLoader::Loading_Model_For_Level_Bern()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(-90.0f));

	/* Npc 마네킹 및 모델 */
	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_FE01_MQ";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;

				
				}

				pUIManager->Add_CurrFile();
				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_FE02_MQ";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_FE03_MQ";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_FE05_MQ";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_MA01_MQ";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_MA02_MQ";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_MA03_MQ";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_MA04_MQ";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_MA05_MQ";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_MA02_BernSoldier";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_MA02_BernKnight";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_FE02_BernSoldier";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_FE02_BernKnight";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_SP_Adel";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_SP_Eadalin";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"NP_SP_Lineheart";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_BERN, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
					pUIManager->Add_CurrFile();
				}

				return S_OK;
			}));
	}

	/* Npc 모델파츠 */
	wstring strPath = L"../Bin/Resources/Meshes/NpcPart/";
	AutoLoad(strPath, LEVEL_BERN);

	/* Npc 무기파츠 */
	strPath = L"../Bin/Resources/Meshes/NpcWeapon/";
	AutoLoad(strPath, LEVEL_BERN, PivotMatrix);

	/* SkyDome */
	{
		wstring strFileName = L"SkyDome0";
		wstring strFilePath = L"../Bin/Resources/SkyDome/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		Matrix matPivot = Matrix::Identity;
		XMStoreFloat4x4(&matPivot, XMMatrixScaling(2.f, 2.f, 2.f) * XMMatrixRotationX(XMConvertToRadians(90.0f)));

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, matPivot))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

		strFileName = L"SkyDome1";
		strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, matPivot))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

		strFileName = L"SkyDome2";
		strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, matPivot))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}
		
		strFileName = L"SkyDome3";
		strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, matPivot))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}
		
		strFileName = L"SkyDome4";
		strComponentName = L"Prototype_Component_Model_" + strFileName;
		XMStoreFloat4x4(&matPivot, XMMatrixScaling(2.f, 2.f, 2.f) * XMMatrixRotationX(XMConvertToRadians(90.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f)));

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, matPivot))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}

	/* 플레이어 장비 */
	{
		wstring strFileName = L"GN_Face";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}

	{
		wstring strFileName = L"GN_Legend_Helmet";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}

	{
		wstring strFileName = L"GN_Legend_Body";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}

	{
		wstring strFileName = L"GN_Legend_Leg";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}

	/* 플레이어 무기 */
	{
		wstring strFileName = L"GN_WP_Hand_Legend";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}

	{
		wstring strFileName = L"GN_WP_Long_Legend";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}

	{
		wstring strFileName = L"GN_WP_Shot_Legend";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}

	{
		wstring strFileName = L"WP_WR_Base";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}

	{
		wstring strFileName = L"WDR_WP_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}

	{
		wstring strFileName = L"WDR_WP_Base";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}

	{
		wstring strFileName = L"MG_WP_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}
	
	//Custom Effect
	{
		wstring strFileName = L"Effect_Custom_SpiralChaser";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}

	{
		wstring strFileName = L"Effect_Custom_Grenade";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}
	
	{
		wstring strFileName = L"Effect_Custom_GN_Bomb";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
			pUIManager->Add_CurrFile();
		}

	}

	Safe_Release(pUIManager);
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_Model_For_Level_Lobby()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(-90.0f));

	/* 플레이어 */
	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"Gunslinger";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"WR";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
				}

				return S_OK;
			}));
	}

	
	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"WDR";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
				}

				return S_OK;
			}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
			{
				wstring strFileName = L"MG";
				wstring strFilePath = L"../Bin/Resources/Meshes/";
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

				if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
				{
					if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
						CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
						return E_FAIL;
				}

				return S_OK;
			}));
	}

	/* 장비 */
	{
		wstring strFileName = L"GN_Head_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
		}
	}

	{
		wstring strFileName = L"GN_Body_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
		}
	}

	{
		wstring strFileName = L"WR_Body_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
		}
	}

	{
		wstring strFileName = L"WR_Head_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
		}
	}

	{
		wstring strFileName = L"WDR_Head_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
		}
	}

	{
		wstring strFileName = L"WDR_Body_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
		}
	}

	{
		wstring strFileName = L"WDR_Body_BaseMococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
		}
	}

	{
		wstring strFileName = L"WDR_Head_BaseMococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
		}
	}

	{
		wstring strFileName = L"MG_Head_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
		}

	}

	{
		wstring strFileName = L"MG_Body_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
				return E_FAIL;
		}
	}

	{
		wstring strFileName = L"Tea";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_LOBBY, strComponentName)))
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOBBY, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
				return E_FAIL;
		}
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_Model_For_Level_Tool_Npc()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(-90.0f));

	/* Npc 마네킹 및 모델 */
	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_FE01_MQ";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;

		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_FE02_MQ";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_FE03_MQ";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_FE05_MQ";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_MA01_MQ";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_MA02_MQ";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_MA03_MQ";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_MA04_MQ";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_MA05_MQ";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_MA02_BernSoldier";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_MA02_BernKnight";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_FE02_BernSoldier";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_FE02_BernKnight";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_SP_Adel";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_SP_Eadalin";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));
	}

	{
		m_Futures.push_back(std::async([=]()->HRESULT
		{
			wstring strFileName = L"NP_SP_Lineheart";
			wstring strFilePath = L"../Bin/Resources/Meshes/";
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(pGameInstance->Check_Prototype(LEVEL_TOOL_NPC, strComponentName)))
			{
				if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOOL_NPC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
					return E_FAIL;
			}

			return S_OK;
		}));	
	}

	/* Npc 모델파츠 */
	wstring strPath = L"../Bin/Resources/Meshes/NpcPart/";
	AutoLoad(strPath, LEVEL_TOOL_NPC);

	/* Npc 무기파츠 */
	strPath = L"../Bin/Resources/Meshes/NpcWeapon/";
	AutoLoad(strPath, LEVEL_TOOL_NPC, PivotMatrix);


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


HRESULT CLoader::Loading_SkillIcon()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);	

	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);
	//GN_SKILL
	{
		//HG SKILL
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_AT02_Grenade"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/AT02_Grenade.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();


			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bullet_Rain"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Bullet_Rain.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Death_Fire"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Death_Fire.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Dexterous_Shot"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Dexterous_Shot.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Equilibrium"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Equilibrium.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Meteor_Stream"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Equilibrium.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Peacekeeper"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Peacekeeper.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Plasma_Bullet"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Plasma_Bullet.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Quick_Step"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Quick_Step.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Somersault_Shot"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Somersault_Shot.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Spiral_Tracker"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Spiral_Tracker.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

		}

		//SG SKILL
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Dual_Buckshot"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/SG/Dual_Buckshot.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Hour_of_Judgment"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/SG/Hour_of_Judgment.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Last_Request"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/SG/Last_Request.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Sharpshooter"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/SG/Sharpshooter.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Shotgun_Rapid_Fire"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/SG/Shotgun_Rapid_Fire.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

		}

		//RF SKILL
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Catastrophe"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/RF/Catastrophe.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();


			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Focused_Shot"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/RF/Focused_Shot.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Perfect_Shot"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/RF/Perfect_Shot.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Spiral_Flame"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/RF/Spiral_Flame.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Target_Down"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/RF/Target_Down.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

		}
	}

	//SLAYER_SKILL
	{
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Brutal_Impact"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Brutal_Impact.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Cross_Blade"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Cross_Blade.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Cruel_Pierce"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Cruel_Pierce.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Fatal_Sword"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Fatal_Sword.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Final_Blow"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Final_Blow.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Flash_Blade"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Flash_Blade.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Flying_Strike"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Flying_Strike.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Furious_Claw"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Furious_Claw.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Fury_Blade"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Fury_Blade.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Ground_Smash"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Ground_Smash.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Guillotine"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Guillotine.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Hurricane_Sword"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Hurricane_Sword.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Mountain_Cleave"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Mountain_Cleave.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Punishing_Draw"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Punishing_Draw.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Spinning_Sword"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Spinning_Sword.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Volcanic_Eruption"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Volcanic_Eruption.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Wild_Rush"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Wild_Rush.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_Wild_Stomp"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Normal/Slayer_Wild_Stomp.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

		}

		//BERSERKR_MODE
		{
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Slayer_IdentitySkill"),
				CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/IdentitySkill.png"))))
				return E_FAIL;
			pUIManager->Add_CurrFile();

		}
	}

	//Destroyer
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Dreadnaught"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Dreadnaught.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_dt_skill_1"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/dt_skill_1.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Earth_Eater"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Earth_Eater.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Earth_Smasher"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Earth_Smasher.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Earth_Wave"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Earth_Wave.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Endure_Pain"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Endure_Pain.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Full_Swing"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Full_Swing.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Gravitational_Energy"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Gravitational_Energy.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Gravity_Compression"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Gravity_Compression.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Gravity_Impact"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Gravity_Impact.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Heavy_Crush"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Heavy_Crush.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Jumping_Smash"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Jumping_Smash.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Neutralizer"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Neutralizer.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Perfect_Swing"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Perfect_Swing.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Power_Shoulder"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Power_Shoulder.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Power_Strike"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Power_Strike.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Running_Crash"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Running_Crash.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Seismic_Hammer"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Normal/Seismic_Hammer.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

	}

	//Bard
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bard_Sonatina"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/MG_SkillIcon/Normal/Sonatina.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bard_Sound_Shock"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/MG_SkillIcon/Normal/Sound_Shock.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bard_Prelude_of_Storm"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/MG_SkillIcon/Normal/Prelude_of_Storm.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bard_Rhapsody_of_Light"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/MG_SkillIcon/Normal/Rhapsody_of_Light.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bard_Wind_of_Music"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/MG_SkillIcon/Normal/Wind_of_Music.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bard_Sonic_Vibration"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/MG_SkillIcon/Normal/Sonic_Vibration.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bard_Heavenly_Tune"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/MG_SkillIcon/Normal/Heavenly_Tune.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bard_Guardian_Tune"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/MG_SkillIcon/Normal/Guardian_Tune.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

	}

	//Space
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Space"),//GN
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/Space%d.png",2))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_WRSpace"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WR_SkillIcon/Space%d.png", 2))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_WDRSpace"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/WDR_SkillIcon/Space%d.png", 2))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_MGSpace"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/MG_SkillIcon/Space%d.png", 2))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

	}

	//Holding_Gauge
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_Frame"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Holding_Bar/Holding_Frame.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_Empty"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Holding_Bar/Holding_Empty.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_Fill"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Holding_Bar/Holding_Fill.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_Shine"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Holding_Bar/Holding_Shine.png"))))
			return E_FAIL;
		pUIManager->Add_CurrFile();

	}

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkillFrame"),
		CUI_SkillIcon_Frame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ItemFrame"),
		CUI_ItemIcon_Frame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ETCFrame"),
		CUI_ETCIcon_Frame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QuickSkillUI"),
		CUI_QuickSkill::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MoveFrame"),
		CUI_SkillI_MoveFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpaceBarIcon"),
		CUI_SpaceBar_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	pUIManager->Add_CurrFile();

	Safe_Release(pUIManager);
	Safe_Release(pGameInstance);
	
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevel)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);	

	DeleteCriticalSection(&m_Critical_Section);

	CloseHandle(m_hThread);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}