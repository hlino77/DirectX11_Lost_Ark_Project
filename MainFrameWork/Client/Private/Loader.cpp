#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "AsFileUtils.h"
#include "AsUtils.h"
#include <filesystem>

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

#include "Camera_Free.h"
#include "StaticModel.h"
#include "ServerSession.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSessionManager.h"
#include "Camera_Player.h"
#include "BackGround_Server.h"

#include "Monster_Zombie.h"
#include "Monster_Plant.h"
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
#include "UI_QuickSkill.h"
#include "UI_Skill_MoveIcon.h"
#include "UI_SpaceBar_Icon.h"


//Monsters
#include "Monster_Zombie.h"
#include "Monster_Plant.h"
#include "Boss_Golem.h"
#include "Monster_Ghoul.h"
#include <Monster_Reaper.h>
#include "Weapon_Boss_King.h"
#include "Weapon_Mn_Reaper.h"
#include "Boss_King.h"
#include <Monster_Pawn.h>
#include <Weapon_Mn_PawnShield.h>
#include <Weapon_Mn_PawnSword.h>

#include "VoidEffect.h"

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

	if(FAILED(Loading_IdentityTexture()))
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

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpeechBubble"),
		CUI_SpeechBubble::Create(m_pDevice, m_pContext))))
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

	/* For.Model */
	m_strLoading = TEXT("모델을 로딩 중 입니다.");
	{
		wstring strMeshEffectFilePath = TEXT("../Bin/Resources/Effects/FX_Meshes/");
		for (const auto& category : filesystem::directory_iterator(strMeshEffectFilePath))
		{
			if (!category.is_directory())
				continue;
			for (const auto& entry : filesystem::directory_iterator(category.path()))
			{
				Matrix matPivot = Matrix::Identity;
				const wstring& strFileName = entry.path().stem();
				wstring strComponentName = L"Prototype_Component_Model_" + strFileName;
				wstring strFinalPath = category.path().generic_wstring() + TEXT("/");

				if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFinalPath, strFileName, true, false))))
					return E_FAIL;
			}
		}
	}

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

	Safe_Release(pGameInstance);

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_ServerSelect()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Component*/
	m_strLoading = TEXT("컴포넌트를 로딩 중 입니다.");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Controller_GN"),
		CPlayer_Controller_GN::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	m_strLoading = TEXT("컴포넌트를 로딩 중 입니다.");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Controller_WR"),
		CController_WR::Create(m_pDevice, m_pContext))))
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

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_EntranceButton"),
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

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");



	Safe_Release(pGameInstance);

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Bern()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");

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

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Gunslinger"),
		CPlayer_Gunslinger::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_WR"),
		CPlayer_Slayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_WP_WR_Base"),
		CWeapon_WR::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Zombie"),
		CMonster_Zombie::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Plant"),
		CMonster_Plant::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Hand"),
		CWeapon_Hand::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Hand_2"),
		CWeapon_Hand_2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Long"),
		CWeapon_Long::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Shot"),
		CWeapon_Shot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Shot_2"),
		CWeapon_Shot_2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StaticModel"),
		CStaticModel::Create(m_pDevice, m_pContext, PROP))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_Golem"),
		CBoss_Golem::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Ghoul"),
		CMonster_Ghoul::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Reaper"),
		CMonster_Reaper::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Mn_Reaper"),
		CWeapon_Mn_Reaper::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Boss_King"),
		CBoss_King::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Boss_King"),
		CWeapon_Boss_King::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Pawn"),
		CMonster_Pawn::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Mn_PawnShield"),
		CWeapon_Mn_PawnShield::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon_Mn_PawnSword"),
		CWeapon_Mn_PawnSword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(Loading_Skill_For_Level_Bern()))
		return E_FAIL;

	//Load_MapData(LEVEL_ARENA, L"../Bin/Resources/MapData/Arena.data");
	//Load_ColMesh(LEVEL_ARENA, L"../Bin/Resources/ColMeshData/Arena.data");


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Chaos1()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	//pUIManager->ObjectManager_to_UIManager(LEVEL_LOADING);
	pUIManager->Loading_UI(0.1f);


	{
		wstring strFileName = L"Monster_Zombie";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_1, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Loading_UI(800.f);
	}

	{
		wstring strFileName = L"Monster_Ghoul";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_1, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Loading_UI(15600.f);
	}


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Chaos2()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	//pUIManager->ObjectManager_to_UIManager(LEVEL_LOADING);
	pUIManager->Loading_UI(0.1f);

	{
		wstring strFileName = L"Monster_Plant";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_2, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Loading_UI(300.f);
	}




	{
		wstring strFileName = L"Boss_Golem";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_2, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Loading_UI(600.f);
	}

	
	{

		wstring strFileName = L"Monster_Reaper";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_2, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Loading_UI(1200.f);
	}

	{
		wstring strFileName = L"Reaper_Wp";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_2, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Loading_UI(1600.f);
	}

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Chaos3()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	//pUIManager->ObjectManager_to_UIManager(LEVEL_LOADING);
	pUIManager->Loading_UI(0.1f);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	{
		wstring strFileName = L"Boss_King";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_3, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Loading_UI(100.f);
	}

	{
		wstring strFileName = L"Boss_Wp_KingSword";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_3, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		pUIManager->Loading_UI(300.f);
	}
	{

		wstring strFileName = L"Monster_Pawn";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_3, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		pUIManager->Loading_UI(900.f);
	}

	{
		wstring strFileName = L"Weapon_Mn_PawnShield";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_3, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		pUIManager->Loading_UI(1200.f);
	}
	{
		wstring strFileName = L"Weapon_Mn_PawnSword";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_CHAOS_3, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		pUIManager->Loading_UI(1600.f);
	}

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	Safe_Release(pGameInstance);
	return S_OK;
}



HRESULT CLoader::Load_MapData(LEVELID eLevel, const wstring& szFilePath)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFilePath, FileMode::Read);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));


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
			CModel::Create(m_pDevice, m_pContext, szModelPath, szModelName, true, false, PivotMatrix))))
			return E_FAIL;


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

HRESULT CLoader::Load_ColMesh(LEVELID eLevel, const wstring& szFilePath)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFilePath, FileMode::Read);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));


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

HRESULT CLoader::Loading_IdentityTexture()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_BackGround.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_BackGround_Wing"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_BackGroundWing.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_BackGround_Shine"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_BackGround_Shine.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_MainFrame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_WF_MainFrame%d.png",3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_WeaponFrame_ShineEffect"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_WeaponFrame_ShineEffect%d.png",3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_WeaponFrame_Front"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_WeaponFrame_Front.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_Arrow"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_Arrow.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_ETC"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_ETC.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_SkillCoolTimer"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_SkillCoolTimer%d.png",4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_WeaponChange_Spark"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Identity/Gunslinger/IdentityGN_WeaponChange_Spark.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hp_Empty"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Hp_Empty.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hp_Fill"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Hp_Fill.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hp_Frame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Hp_Frame.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mp_Empty"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Mp_Empty.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mp_Fill"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Mp_Fill.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mp_Frame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Mp_Frame.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Shield_Fill"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/PlayerHUD/Shield_Fill.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_Bubble"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chat/Chat_Bubble.png"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}



HRESULT CLoader::Loading_Model_For_Level_Bern()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(-90.0f));

	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	//pUIManager->ObjectManager_to_UIManager(LEVEL_LOADING);
	pUIManager->Loading_UI(0.1f);

	/* 플레이어 */
	{
		wstring strFileName = L"Gunslinger";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		pUIManager->Loading_UI(154.f);
	}

	{
		wstring strFileName = L"WR";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		pUIManager->Loading_UI(154.f);
	}

	/* 플레이어 장비 */
	{
		wstring strFileName = L"GN_Face";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Loading_UI(231.f);
	}

	{
		wstring strFileName = L"GN_Legend_Helmet";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Loading_UI(308.f);
	}

	{
		wstring strFileName = L"GN_Legend_Body";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Loading_UI(385.f);
	}

	{
		wstring strFileName = L"GN_Legend_Leg";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Loading_UI(462.f);
	}

	{
		wstring strFileName = L"GN_Mococo_Head";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Loading_UI(539.f);
	}

	{
		wstring strFileName = L"GN_Mococo_Body";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Loading_UI(616.f);
	}

	{
		wstring strFileName = L"WR_Body_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Loading_UI(616.f);
	}

	{
		wstring strFileName = L"WR_Head_Mococo";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false))))
			return E_FAIL;
		pUIManager->Loading_UI(616.f);
	}

	/* 플레이어 무기 */
	{
		wstring strFileName = L"GN_WP_Hand_Legend";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		pUIManager->Loading_UI(693.f);
	}

	{
		wstring strFileName = L"GN_WP_Long_Legend";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		pUIManager->Loading_UI(800.f);
	}

	{
		wstring strFileName = L"GN_WP_Shot_Legend";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		pUIManager->Loading_UI(1000.f);
	}

	{
		wstring strFileName = L"WP_WR_Base";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		pUIManager->Loading_UI(1000.f);
	}


	/* 지형 */

	{
		wstring strFileName = L"SM_ENV_TCHEXA_ArenaGround_Aa";
		wstring strFilePath = L"../Bin/Resources/Meshes/Static/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		pUIManager->Loading_UI(1200.f);
	}

	
	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_Model_For_Level_Lobby()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_Skill_For_Level_Bern()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Empty"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/Skill_Empty%d.png",2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Shine"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/Skill_Shine.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Frame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/Skill_Frame.png"))))
		return E_FAIL;
	
	//HG SKILL
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_AT02_Grenade"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/AT02_Grenade.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bullet_Rain"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Bullet_Rain.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Death_Fire"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Death_Fire.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Dexterous_Shot"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Dexterous_Shot.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Equilibrium"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Equilibrium.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Meteor_Stream"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Equilibrium.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Peacekeeper"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Peacekeeper.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Plasma_Bullet"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Plasma_Bullet.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Quick_Step"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Quick_Step.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Somersault_Shot"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Somersault_Shot.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Spiral_Tracker"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/HG/Spiral_Tracker.png"))))
		return E_FAIL;
	}

	//SG SKILL
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Dual_Buckshot"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/SG/Dual_Buckshot.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Hour_of_Judgment"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/SG/Hour_of_Judgment.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Last_Request"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/SG/Last_Request.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Sharpshooter"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/SG/Sharpshooter.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Shotgun_Rapid_Fire"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/SG/Shotgun_Rapid_Fire.png"))))
			return E_FAIL;
	}

	//RF SKILL
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Catastrophe"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/RF/Catastrophe.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Focused_Shot"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/RF/Focused_Shot.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Perfect_Shot"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/RF/Perfect_Shot.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Spiral_Flame"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/RF/Spiral_Flame.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Target_Down"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/RF/Target_Down.png"))))
			return E_FAIL;
	}

	//Space
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Space"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Skill_Slot/GN_SkillIcon/Space%d.png",2))))
			return E_FAIL;
	}

	//Holding_Gauge
	{
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_Frame"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Holding_Bar/Holding_Frame.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_Empty"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Holding_Bar/Holding_Empty.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_Fill"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Holding_Bar/Holding_Fill.png"))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Holding_Shine"),
			CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Holding_Bar/Holding_Shine.png"))))
			return E_FAIL;
	}

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SkillFrame"),
		CUI_SkillIcon_Frame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_QuickSkillUI"),
		CUI_QuickSkill::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MoveFrame"),
		CUI_SkillI_MoveFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpaceBarIcon"),
		CUI_SpaceBar_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

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