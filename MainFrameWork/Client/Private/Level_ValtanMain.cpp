#include "stdafx.h"
#include "Level_ValtanMain.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Player.h"
#include "AsUtils.h"

#include "StaticModel.h"
#include "AnimModel.h"

#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "Camera_Player.h"
#include "AsFileUtils.h"
#include "AsUtils.h"
#include "ColMesh.h"
#include "ColliderOBB.h"
#include "ThreadManager.h"
#include "CollisionManager.h"
#include "PickingMgr.h"
#include "NavigationMgr.h"
#include "EventMgr.h"
#include "PhysXMgr.h"
#include "RigidBody.h"
#include "Pool.h"
#include "Renderer.h"
#include "Monster_Zombie.h"
#include "Chat_Manager.h"
#include "UI_Tool.h"
#include "UI_Manager.h"
#include "Projectile.h"
#include "UI_DamageFont.h"
#include "Damage_Manager.h"
#include "UI_SpaceBar_Icon.h"
#include "QuadTreeMgr.h"
#include <filesystem>
#include "Party.h"
#include "Sound_Manager.h"
#include "Effect_Manager.h"
#include "SkyFloor.h"


CLevel_ValtanMain::CLevel_ValtanMain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_ValtanMain::Initialize()
{
	CServerSessionManager::GetInstance()->Get_ServerSession()->Set_LevelState(LEVELSTATE::INITREADY);

	Send_UserInfo();

	Ready_Renderer();

	Ready_Events();

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	//m_pImGuiManager = CUI_Tool::GetInstance();

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_SkyBox(LAYER_TYPE::LAYER_SKYBOX)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(LAYER_TYPE::LAYER_CAMERA)))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(LAYER_TYPE::LAYER_BACKGROUND)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(LAYER_TYPE::LAYER_PLAYER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(LAYER_TYPE::LAYER_MONSTER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;


	if (FAILED(Load_BossMapData(LEVEL_VALTANMAIN, TEXT("../Bin/Resources/MapData/Boss_End.data"))))
	{
		return E_FAIL;
	}

	if (FAILED(Ready_NonActive_NaviCell()))
	{
		return E_FAIL;
	}

	while (true)
	{
		CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
		if (pPlayer)
			break;
	}

	
	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	CServerSessionManager::GetInstance()->Send_LevelState(LEVELSTATE::PLAYERREADY);

	while (true)
	{
		if (CServerSessionManager::GetInstance()->Get_ServerSession()->Get_LevelState() == LEVELSTATE::INITEND)
			break;
	}

	if (FAILED(Ready_Player_Camera(LAYER_TYPE::LAYER_CAMERA)))
		return E_FAIL;


	Start_Collision();
	Start_Damage();
	Start_QuadTree();

	CChat_Manager::GetInstance()->Set_Active(false);

	CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_FadeInOut(1.0f, true);
	CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_FadeIntensity(1.0f);

	CUI_Manager::GetInstance()->Set_UIs_Active(false, LEVELID::LEVEL_VALTANMAIN);
	CUI_Manager::GetInstance()->Set_RenderNickName(false, LEVELID::LEVEL_VALTANMAIN);
	CUI_Manager::GetInstance()->Set_RenderNickName(false, LEVELID::LEVEL_STATIC);
	CUI_Manager::GetInstance()->Set_LevelCustomOption();

	// DOF
	CRenderer::Set_DOF_Range(50.f);

	return S_OK;
}

HRESULT CLevel_ValtanMain::Tick(const _float& fTimeDelta)
{
	/*if(KEY_TAP(KEY::F9))
		m_pRendererCom->Set_StaticShadow();*/

	CRenderer::Set_Add_Fog_Time(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_ValtanMain::LateTick(const _float& fTimeDelta)
{
	if (KEY_TAP(KEY::C))
		CUI_Manager::GetInstance()->Set_UIs_Active(false, LEVEL_CHAOS_3);
	if (KEY_TAP(KEY::V))
		CUI_Manager::GetInstance()->Set_UIs_Active(true, LEVEL_CHAOS_3);

	//CUI_Tool::GetInstance()->LateTick();
	return S_OK;
}

HRESULT CLevel_ValtanMain::Render_Debug()
{
	//m_pImGuiManager->Tick();
	return S_OK;
}

HRESULT CLevel_ValtanMain::Exit()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	End_Collision();
	End_Picking();
	End_Damage();
	End_QuadTree();

	if (FAILED(pGameInstance->Clear_LightShadowTexture()))
		return E_FAIL;

	CPhysXMgr::GetInstance()->Reset();
	CUI_Manager::GetInstance()->Clear(LEVELID::LEVEL_VALTANMAIN);
	CGameInstance::GetInstance()->Reset_Lights();
	CSound_Manager::GetInstance()->Stop_SoundAll();
	//CUI_Tool::GetInstance()->Set_ToolMode(false);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_ValtanMain::Ready_Lights()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHTDESC			LightDesc;


	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = Vec4(-0.454f, -0.89f, 0.007f, 0.f);
	LightDesc.vDirection.Normalize();
	LightDesc.vDiffuse = Vec4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vAmbient = Vec4(1.0f, 1.0f, 1.0f, 1.f);
	LightDesc.vSpecular = Vec4(1.f, 1.f, 1.f, 1.f);


	/*CTexture* pStaticShadowMap = CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/LightMap/Light_Valtan.dds");

	if (FAILED(pGameInstance->Set_LightShadowTexture(pStaticShadowMap)))
		return E_FAIL;*/
	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	Vec3 vLook = LightDesc.vDirection;
	vLook.Normalize();

	Vec3 vOffset = Vec3(137.76f, 64.00f, 94.34f);

	pGameInstance->Ready_StaticLightMatrix(vOffset, vLook);
	vOffset = vLook * -30.0f;
	pGameInstance->Ready_LightMatrix(vOffset, vLook);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_ValtanMain::Ready_Layer_SkyBox(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pSkyDome = pGameInstance->Add_GameObject(LEVEL_VALTANMAIN, _uint(eLayerType), TEXT("Prototype_GameObject_SkyDome"));
	if (nullptr == pSkyDome)
		return E_FAIL;

	pSkyDome->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(100.f, 0.f, 100.f));


	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	Matrix matPivot = Matrix::Identity;
	matPivot.Translation(Vec3(100.f, 0.f, 100.f));
	tDesc.pPivotMatrix = &matPivot;
	EFFECT_START(L"VT_PhotoZone", &tDesc);

	CSkyFloor::SkyFloorDescription desc;

	// 첫 번째
	desc.vTileCount = Vec2(2.f, 2.f);
	desc.vFloorUVoffset = Vec2(0.005f, 0.005f);
	desc.fAlpha = 0.4f;
	CGameObject* pSkyFloor = pGameInstance->Add_GameObject(LEVEL_VALTANMAIN, _uint(eLayerType), TEXT("Prototype_GameObject_SkyFloor"), &desc);
	if (nullptr == pSkyFloor)
		return E_FAIL;

	pSkyFloor->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(100.f, -138.f, 100.f));

	// 두 번째
	desc.vTileCount = Vec2(4.f, 4.f);
	desc.vFloorUVoffset = Vec2(0.0047f, 0.0047f);
	desc.fAlpha = 0.4f;
	pSkyFloor = pGameInstance->Add_GameObject(LEVEL_VALTANMAIN, _uint(eLayerType), TEXT("Prototype_GameObject_SkyFloor"), &desc);
	if (nullptr == pSkyFloor)
		return E_FAIL;

	pSkyFloor->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(100.f, -136.f, 100.f));

	// 세 번째
	desc.vTileCount = Vec2(5.f, 5.f);
	desc.vFloorUVoffset = Vec2(0.005f, 0.005f);
	desc.fAlpha = 0.4f;
	pSkyFloor = pGameInstance->Add_GameObject(LEVEL_VALTANMAIN, _uint(eLayerType), TEXT("Prototype_GameObject_SkyFloor"), &desc);
	if (nullptr == pSkyFloor)
		return E_FAIL;

	pSkyFloor->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(100.f, -134.f, 100.f));

	CRenderer::Set_IBLTexture(22);

	// Fog
	CRenderer::Set_Fog_Color(Vec3(0.80f, 0.80f, 0.8f));
	CRenderer::Set_Fog_StartHeight(1.f);
	CRenderer::Set_Fog_EndHeight(-57.f);
	CRenderer::Set_Fog_Density(0.008f);
	CRenderer::Set_Fog_ChangeSpeed(1.f);
	CRenderer::Set_Fog_MinValue(0.5f);


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_ValtanMain::Ready_Layer_Camera(const LAYER_TYPE eLayerType)
{


	return S_OK;
}

HRESULT CLevel_ValtanMain::Ready_Layer_Player(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_ValtanMain::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_ValtanMain::Ready_Layer_Monster(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	//CMonster_Zombie::MODELDESC MonsterDesc = {};
	//MonsterDesc.iLayer = (_uint)LAYER_TYPE::LAYER_MONSTER;
	//MonsterDesc.vPos = Vec3(6, 0, 1);
	//
	//CGameObject* pObject = pGameInstance->Add_GameObject(LEVEL_BERN, MonsterDesc.iLayer, TEXT("Prototype_GameObject_Monster_Zombie"),&MonsterDesc);
	//if (nullptr == pObject)
	//{
	//	Safe_Release(pGameInstance);
	//	return E_FAIL;
	//}

	//MonsterDesc.vPos = Vec3(3, 0, -1);

	// pObject = pGameInstance->Add_GameObject(LEVEL_BERN, MonsterDesc.iLayer, TEXT("Prototype_GameObject_Monster_Plant"), &MonsterDesc);
	//if (nullptr == pObject)
	//{
	//	Safe_Release(pGameInstance);
	//	return E_FAIL;
	//}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_ValtanMain::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LEVELID eLevel = LEVEL_VALTANMAIN;

	CGameObject* pUI = pGameInstance->Add_GameObject(eLevel, _uint(eLayerType), TEXT("Prototype_GameObject_ChatUI"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI(eLevel, static_cast<CUI*>(pUI));

	if (L"Gunslinger" == CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectTag())
	{
		pUI = pGameInstance->Add_GameObject(eLevel, _uint(eLayerType), TEXT("Prototype_GameObject_IdentityGNUI"));
		if (nullptr == pUI)
			return E_FAIL;
		else
			CUI_Manager::GetInstance()->Add_UI(eLevel, static_cast<CUI*>(pUI));
	}
	else if (L"WR" == CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectTag())
	{
		pUI = pGameInstance->Add_GameObject(eLevel, _uint(eLayerType), TEXT("Prototype_GameObject_WRIdentity"));
		if (nullptr == pUI)
			return E_FAIL;
		else
			CUI_Manager::GetInstance()->Add_UI(eLevel, static_cast<CUI*>(pUI));
	}
	else if (L"WDR" == CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectTag())
	{
		pUI = pGameInstance->Add_GameObject(eLevel, _uint(eLayerType), TEXT("Prototype_GameObject_WDRIdentity_UI"));
		if (nullptr == pUI)
			return E_FAIL;
		else
			CUI_Manager::GetInstance()->Add_UI(eLevel, static_cast<CUI*>(pUI));
	}
	else if (L"SP" == CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectTag())
	{
		pUI = pGameInstance->Add_GameObject(eLevel, _uint(eLayerType), TEXT("Prototype_GameObject_UI_IdentitySP"));
		if (nullptr == pUI)
			return E_FAIL;
		else
			CUI_Manager::GetInstance()->Add_UI(eLevel, static_cast<CUI*>(pUI));
	}

	pUI = pGameInstance->Add_GameObject(eLevel, _uint(eLayerType), TEXT("Prototype_GameObject_PlayerHPUI"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI(eLevel, static_cast<CUI*>(pUI));

	pUI = pGameInstance->Add_GameObject(eLevel, _uint(eLayerType), TEXT("Prototype_GameObject_PlayerMPUI"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI(eLevel, static_cast<CUI*>(pUI));

	pUI = pGameInstance->Add_GameObject(eLevel, _uint(eLayerType), TEXT("Prototype_GameObject_QuickSkillUI"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI(eLevel, static_cast<CUI*>(pUI));

	pUI = pGameInstance->Add_GameObject(eLevel, _uint(eLayerType), TEXT("Prototype_GameObject_PartyUI"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI(eLevel, static_cast<CUI*>(pUI));

	pUI = pGameInstance->Add_GameObject(eLevel, _uint(eLayerType), TEXT("Prototype_GameObject_UI_DeadScene"));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		CUI_Manager::GetInstance()->Add_UI(eLevel, static_cast<CUI*>(pUI));
		CUI_Manager::GetInstance()->Player_DeadScene(false, eLevel);
	}
	pUI = pGameInstance->Add_GameObject(eLevel, _uint(eLayerType), TEXT("Prototype_GameObject_UI_EstherSkill"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI(eLevel, static_cast<CUI*>(pUI));

	pUI = pGameInstance->Add_GameObject(eLevel, _uint(eLayerType), TEXT("Prototype_GameObject_UI_ValtanClearWnd"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI(eLevel, static_cast<CUI*>(pUI));

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_ValtanMain::Ready_Layer_Effect(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_ValtanMain::Ready_Player_Camera(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();

	if (pPlayer == nullptr)
		return E_FAIL;

	CCamera_Player::PlayerCameraDesc CameraDesc;

	CameraDesc.tCameraDesc.iLayer = (_uint)eLayerType;
	CameraDesc.tCameraDesc.vEye = Vec4(0.f, 10.0f, -10.f, 1.f);
	CameraDesc.tCameraDesc.vAt = Vec4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.tCameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.tCameraDesc.fAspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
	CameraDesc.tCameraDesc.fNear = 0.2f;
	CameraDesc.tCameraDesc.fFar = 1200.0f;

	CameraDesc.tCameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.tCameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.pPlayer = pPlayer;
	CameraDesc.vOffset = Vec3(0.0f, 0.0f, -1.0f);
	CameraDesc.fDefaultLength = 9.5f;

	CGameObject* pCamera = pGameInstance->Add_GameObject(LEVEL_VALTANMAIN, _uint(eLayerType), TEXT("Prototype_GameObject_Camera_Player"), &CameraDesc);
	if (pCamera == nullptr)
		return E_FAIL;

	pPlayer->Set_Camera(dynamic_cast<CCamera_Player*>(pCamera));

	Safe_Release(pGameInstance);

	return S_OK;
}



HRESULT CLevel_ValtanMain::Send_UserInfo()
{
	if (CServerSessionManager::GetInstance()->Get_Player() == nullptr)
	{
		return E_FAIL;
	}
	else
	{
		CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();

		Protocol::S_PLAYERLEVELMOVE pkt;

		pkt.set_icurrlevel(pPlayer->Get_CurrLevel());
		pkt.set_inextlevel(LEVELID::LEVEL_VALTANMAIN);

		SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
		CServerSessionManager::GetInstance()->Send(pSendBuffer);

		pPlayer->Set_CurrLevel(LEVELID::LEVEL_VALTANMAIN);
		Vec3 vScale = pPlayer->Get_TransformCom()->Get_Scale();
		pPlayer->Get_TransformCom()->Set_WorldMatrix(XMMatrixIdentity());
		pPlayer->Get_TransformCom()->Set_Scale(vScale);
		
		Vec3 vPos;
		_uint iPartyIndex = 0;
		CParty* pParty = pPlayer->Get_Party();
		if (pParty != nullptr)
		{
			iPartyIndex = pParty->Get_PartyIndex(pPlayer);
		}

		if (iPartyIndex == 1)
		{
			vPos = Vec3(97.96f, 0.32f, 85.25f);
		}
		else if (iPartyIndex == 2)
		{
			vPos = Vec3(99.18f, 0.32f, 85.13f);
		}
		else if (iPartyIndex == 3)
		{
			vPos = Vec3(100.90f, 0.32f, 85.08f);
		}
		else if (iPartyIndex == 4)
		{
			vPos = Vec3(102.26f, 0.32f, 85.24f);
		}
		else
		{
			vPos = Vec3(97.96f, 0.32f, 85.25f);
		}

		pPlayer->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
		pPlayer->Set_TargetPos(Vec3());
		pPlayer->Ready_PhysxBoneBranch();
		pPlayer->Ready_Coliders();
		CNavigationMgr::GetInstance()->Find_FirstCell(LEVEL_VALTANMAIN, pPlayer);
		pPlayer->Reset_Level();
	}

	return S_OK;
}

void CLevel_ValtanMain::Send_LevelState(LEVELSTATE eState)
{
	Protocol::S_LEVEL_STATE pkt;
	pkt.set_ilevelstate((uint32)eState);

	SendBufferRef sendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Get_ServerSession()->Send(sendBuffer);
}

void CLevel_ValtanMain::Wait_ServerLevelState(LEVELSTATE eState)
{
	ServerSessionRef& ServerSession = CServerSessionManager::GetInstance()->Get_ServerSession();

	while (true)
	{
		if (ServerSession->Get_LevelState() == eState)
			break;
	}
}


HRESULT CLevel_ValtanMain::Load_MapData(LEVELID eLevel, const wstring& szFullPath)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFullPath, FileMode::Read);

	//Matrix		PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixRotationX(XMConvertToRadians(90.0f));


	Vec3	QuadTreePosition = {};
	Vec3	QuadTreeScale = {};
	_uint	QuadTreeMaxDepth = {};

	file->Read<Vec3>(QuadTreePosition);
	file->Read<Vec3>(QuadTreeScale);
	file->Read<_uint>(QuadTreeMaxDepth);

	CQuadTreeMgr::GetInstance()->Make_QaudTree(QuadTreePosition, QuadTreeScale, QuadTreeMaxDepth);


	vector<wstring> paths =
	{
	L"../Bin/Resources/Export/Bern/",
	L"../Bin/Resources/Export/Chaos1/",
	L"../Bin/Resources/Export/Chaos2/",
	L"../Bin/Resources/Export/Chaos3/",
	L"../Bin/Resources/Export/Boss/"
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

		CStaticModel::MODELDESC Desc;
		Desc.strFileName = CAsUtils::ToWString(strFileName);
		Desc.strFilePath = selectedPath;
		Desc.iLayer = (_uint)LAYER_TYPE::LAYER_BACKGROUND;
		Desc.IsMapObject = true;
		Desc.bInstance = bInstance;

		CGameObject* pObject = pGameInstance->Add_GameObject(eLevel, Desc.iLayer, TEXT("Prototype_GameObject_StaticModel"), &Desc);

		if (nullptr == pObject)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		pObject->Get_TransformCom()->Set_WorldMatrix(matWorld);
		//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_STATICSHADOW, pObject);

		_uint			QuadTreeSize = {};

		file->Read<_uint>(QuadTreeSize);

		for (size_t i = 0; i < QuadTreeSize; i++)
		{
			_uint Index = {};
			file->Read<_uint>(Index);

			pObject->Add_QuadTreeIndex(Index);
			CQuadTreeMgr::GetInstance()->Add_Object(pObject, Index);
		}

	}


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_ValtanMain::Load_BossMapData(LEVELID eLevel, const wstring& szFullPath)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFullPath, FileMode::Read);

	//Matrix		PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixRotationX(XMConvertToRadians(90.0f));


	Vec3	QuadTreePosition = {};
	Vec3	QuadTreeScale = {};
	_uint	QuadTreeMaxDepth = {};

	file->Read<Vec3>(QuadTreePosition);
	file->Read<Vec3>(QuadTreeScale);
	file->Read<_uint>(QuadTreeMaxDepth);

	CQuadTreeMgr::GetInstance()->Make_QaudTree(QuadTreePosition, QuadTreeScale, QuadTreeMaxDepth);


	vector<wstring> paths =
	{
	L"../Bin/Resources/Export/Bern/",
	L"../Bin/Resources/Export/Chaos1/",
	L"../Bin/Resources/Export/Chaos2/",
	L"../Bin/Resources/Export/Chaos3/",
	L"../Bin/Resources/Export/Boss/",
	L"../Bin/Resources/Export/Lobby/",
	L"../Bin/Resources/Export/Nature/"
	};


	_uint iSize = file->Read<_uint>(); // Object List Size 


	bool fileFound = false;

	for (_uint i = 0; i < iSize; ++i)
	{
		string strFileName = file->Read<string>(); // Object Model FileName
		wstring selectedPath = {};

		// Path Find
		for (const auto& path : paths)
		{
			wstring fullPath = path + CAsUtils::ToWString(strFileName);

			if (std::filesystem::exists(fullPath))
			{
				selectedPath = path;
			}
		}
		// Not Found
		if (selectedPath.empty())
		{
			MessageBox(g_hWnd, L"File not found in any specified paths.", L"Error", MB_OK);
			return E_FAIL;
		}

		// Object World Matrix
		Matrix	matWorld = file->Read<Matrix>();

		// Instancing Check 
		_bool bInstance = file->Read<_bool>();


		// Object ModelType
		_uint ModelType = file->Read<_uint>();

	
		// Clone GameObject
		CGameObject* pObject = nullptr;

		if (0 == ModelType) // NonAnim
		{
			CStaticModel::MODELDESC Desc;
			Desc.strFileName = CAsUtils::ToWString(strFileName);
			Desc.strFilePath = selectedPath;
			Desc.iLayer = (_uint)LAYER_TYPE::LAYER_BACKGROUND;
			Desc.IsMapObject = true;
			Desc.bInstance = bInstance;

			pObject = pGameInstance->Add_GameObject(eLevel, Desc.iLayer, TEXT("Prototype_GameObject_StaticModel"), &Desc);
			pObject->Get_TransformCom()->Set_WorldMatrix(matWorld);


			// Under Navi Cell Info

		    _uint vecNavicellsize = file->Read<_uint>(); // Read Vector Size

			for (size_t i = 0; i < vecNavicellsize; i++)
			{
				_uint CellIndex = file->Read<_uint>();

				dynamic_cast<CStaticModel*>(pObject)->Add_NaviCellIndex(CellIndex);
				CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, CellIndex, false);
			}

			_uint iColliderCount = file->Read<_uint>();

		/*	if (iColliderCount == 0)
			{
				m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_STATICSHADOW, pObject);
			}*/

			for (_uint i = 0; i < iColliderCount; ++i)
			{

				dynamic_cast<CStaticModel*>(pObject)->Add_Collider();

				CSphereCollider* pCollider = dynamic_cast<CStaticModel*>(pObject)->Get_StaticCollider(i);

				{
					Vec3 vOffset = file->Read<Vec3>();
					pCollider->Set_Offset(vOffset);


					_float fRadius = file->Read<_float>();
					pCollider->Set_Radius(fRadius);


					pCollider->Set_Center();
				}

				_bool bChild = file->Read<_bool>();

				if (bChild)
				{
					dynamic_cast<CStaticModel*>(pObject)->Add_ChildCollider(i);

					COBBCollider* pChild = dynamic_cast<COBBCollider*>(pCollider->Get_Child());


					Vec3 vOffset = file->Read<Vec3>();
					pChild->Set_Offset(vOffset);

					Vec3 vScale = file->Read<Vec3>();
					pChild->Set_Scale(vScale);

					Quaternion vQuat = file->Read<Quaternion>();
					pChild->Set_Orientation(vQuat);

					pChild->Set_StaticBoundingBox();
				}
			}
			dynamic_cast<CStaticModel*>(pObject)->Add_CollidersToManager();

		}
		else if (1 == ModelType) // Anim
		{
			CAnimModel::MODELDESC Desc;
			Desc.strFileName = CAsUtils::ToWString(strFileName);
			Desc.strFilePath = selectedPath;
			Desc.iLayer = (_uint)LAYER_TYPE::LAYER_BACKGROUND;
			Desc.IsMapObject = true;
			Desc.bInstance = bInstance;

		    pObject = pGameInstance->Add_GameObject(eLevel, Desc.iLayer, TEXT("Prototype_GameObject_AnimModel"), &Desc);
		
			pObject->Get_TransformCom()->Set_WorldMatrix(matWorld);
		}

		if (nullptr == pObject)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}


		

		//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_STATICSHADOW, pObject);

		_uint			QuadTreeSize = {};

		file->Read<_uint>(QuadTreeSize);

		for (size_t i = 0; i < QuadTreeSize; i++)
		{
			_uint Index = {};
			file->Read<_uint>(Index);

			pObject->Add_QuadTreeIndex(Index);
			CQuadTreeMgr::GetInstance()->Add_Object(pObject, Index);
		}

	}


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_ValtanMain::Ready_NonActive_NaviCell()
{
	// Except Navi Cell Index -> bottom of Anim Model Wall02_01 ~ Wall02_04 
	
	// 01
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 931, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 932, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 933, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 934, false);

	// 02
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 571, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 572, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 894, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 895, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 897, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 898, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 899, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 900, false);

	// 03
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 799, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 827, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 828, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 829, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 834, false);

	// 04
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 666, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 667, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 668, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 699, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 700, false);
	CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, 701, false);


	return S_OK;
}


HRESULT CLevel_ValtanMain::Ready_Events()
{

	return S_OK;
}

HRESULT CLevel_ValtanMain::Ready_Renderer()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	m_pRendererCom = dynamic_cast<CRenderer*>(pGameInstance->Clone_Component(nullptr, LEVEL_STATIC, L"Prototype_Component_Renderer"));
	if (m_pRendererCom == nullptr)
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}


void CLevel_ValtanMain::Set_CheckGruop()
{
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_STATICMODEL, (_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_STATICMODEL, (_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER, (_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER, (_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER, (_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER, (_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER, (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER, (_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER, (_uint)LAYER_COLLIDER::LAYER_SAFEZONE);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER, (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER_BUFF);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER, (_uint)LAYER_COLLIDER::LAYER_BUFF_PLAYER);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER, (_uint)LAYER_COLLIDER::LAYER_BUFF_ESTHER);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER, (_uint)LAYER_COLLIDER::LAYER_BODY_BOSS);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER, (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER, (_uint)LAYER_COLLIDER::LAYER_BODY_BOSS);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER, (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER, (_uint)LAYER_COLLIDER::LAYER_BODY_BOSS);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_SPAWNER, (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_SPAWNER, (_uint)LAYER_COLLIDER::LAYER_BODY_BOSS);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_SKILL_ESTHER, (_uint)LAYER_COLLIDER::LAYER_BODY_BOSS);
}

void CLevel_ValtanMain::Start_Collision()
{
	Set_CheckGruop();

	m_pCollisionThread = new thread([=]()
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		CCollisionManager* pCollisionManager = CCollisionManager::GetInstance();
		pCollisionManager->AddRef();

		if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Collision"))))
			return FALSE;

		if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Collision_60"))))
			return FALSE;

		_float		fTimeAcc = 0.f;


		while (!pCollisionManager->Is_Stop())
		{
			fTimeAcc += pGameInstance->Compute_TimeDelta(TEXT("Timer_Collision"));

			if (fTimeAcc >= 1.f / 60.0f)
			{
				pCollisionManager->LateTick_Collision(pGameInstance->Compute_TimeDelta(TEXT("Timer_Collision_60")));
				fTimeAcc = 0.f;
			}
		}

		Safe_Release(pCollisionManager);
		Safe_Release(pGameInstance);

		CGameInstance::GetInstance()->Delete_Timer(L"Timer_Collision");
		CGameInstance::GetInstance()->Delete_Timer(L"Timer_Collision_60");

		return TRUE;
	});
}

void CLevel_ValtanMain::Start_Damage()
{
	m_pDamageThread = new thread([=]()
		{
			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			Safe_AddRef(pGameInstance);

			CDamage_Manager* pDamageManager = CDamage_Manager::GetInstance();
			pDamageManager->AddRef();

			if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Damage"))))
				return FALSE;

			if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Damage_60"))))
				return FALSE;

			_float		fTimeAcc = 0.f;


			while (!pDamageManager->Is_Stop())
			{
				fTimeAcc += pGameInstance->Compute_TimeDelta(TEXT("Timer_Damage"));

				if (fTimeAcc >= 1.f / 60.0f)
				{
					pDamageManager->Tick(pGameInstance->Compute_TimeDelta(TEXT("Timer_Damage_60")));
					fTimeAcc = 0.f;
				}
			}

			Safe_Release(pDamageManager);
			Safe_Release(pGameInstance);

			CGameInstance::GetInstance()->Delete_Timer(L"Timer_Damage");
			CGameInstance::GetInstance()->Delete_Timer(L"Timer_Damage_60");

			return TRUE;
		});
}

void CLevel_ValtanMain::Start_QuadTree()
{
	m_pQuadTreeThread = new thread([=]()
		{
			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			Safe_AddRef(pGameInstance);

			CQuadTreeMgr* pQuadTreeManager = CQuadTreeMgr::GetInstance();
			pQuadTreeManager->AddRef();

			if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_QuadTree"))))
				return FALSE;

			if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_QuadTree_60"))))
				return FALSE;

			_float		fTimeAcc = 0.f;


			while (!pQuadTreeManager->Is_End())
			{
				fTimeAcc += pGameInstance->Compute_TimeDelta(TEXT("Timer_QuadTree"));

				if (fTimeAcc >= 1.f / 60.0f)
				{
					pQuadTreeManager->Tick(pGameInstance->Compute_TimeDelta(TEXT("Timer_QuadTree_60")));
					fTimeAcc = 0.f;
				}
			}

			Safe_Release(pQuadTreeManager);
			Safe_Release(pGameInstance);

			CGameInstance::GetInstance()->Delete_Timer(L"Timer_QuadTree");
			CGameInstance::GetInstance()->Delete_Timer(L"Timer_QuadTree_60");

			return TRUE;
		});
}

void CLevel_ValtanMain::End_Damage()
{
	if (m_pDamageThread == nullptr)
		return;

	CDamage_Manager::GetInstance()->Set_Stop(true);
	m_pDamageThread->join();
	CDamage_Manager::GetInstance()->Reset();
	Safe_Delete(m_pDamageThread);
}


void CLevel_ValtanMain::End_Picking()
{
	if (m_pPickingThread == nullptr)
		return;

	CPickingMgr::GetInstance()->Set_Stop(true);
	m_pPickingThread->join();
	CPickingMgr::GetInstance()->Reset();
	Safe_Delete(m_pPickingThread);
}

void CLevel_ValtanMain::End_Collision()
{
	if (m_pCollisionThread == nullptr)
		return;

	CCollisionManager::GetInstance()->Set_Stop(true);
	m_pCollisionThread->join();
	CCollisionManager::GetInstance()->Reset();
	Safe_Delete(m_pCollisionThread);
}

void CLevel_ValtanMain::End_QuadTree()
{
	if (m_pQuadTreeThread == nullptr)
		return;

	CQuadTreeMgr::GetInstance()->Set_End(true);
	m_pQuadTreeThread->join();
	CQuadTreeMgr::GetInstance()->Reset_QaudTree();
	Safe_Delete(m_pQuadTreeThread);
}

CLevel_ValtanMain * CLevel_ValtanMain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_ValtanMain*	pInstance = new CLevel_ValtanMain(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_VatanMain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_ValtanMain::Free()
{
	__super::Free();
}
