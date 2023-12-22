#include "stdafx.h"
#include "Level_Bern.h"
#include "GameInstance.h"
#include "Camera.h"
#include "Player.h"
#include "AsUtils.h"
#include "StaticModel.h"
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
#include "UI_IdentityGN_BackGroundWing.h"
#include "UI_IdentityGN_BackGroundFrame.h"
#include "UI_IdentityGN_BackGroundShine.h"
#include "UI_IdentityGN_MainFrame.h"
#include "UI_IdentityGN_WF_Front.h"
#include "Projectile.h"
#include "UI_DamageFont.h"


CLevel_Bern::CLevel_Bern(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Bern::Initialize()
{
	CServerSessionManager::GetInstance()->Get_ServerSession()->Set_LevelState(LEVELSTATE::INITREADY);

	Send_UserInfo();

	CNavigationMgr::GetInstance()->Add_Navigation(L"Arena.navi");

	Ready_Renderer();

	Ready_Events();

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_pImGuiManager = CUI_Tool::GetInstance();
	Safe_AddRef(m_pImGuiManager);

	if (FAILED(Ready_Lights()))
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


	while (true)
	{
		CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
		if (pPlayer)
			break;
	}
	
	/*if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;*/

	if (FAILED(CUI_Tool::GetInstance()->Reserve_Manager(g_hWnd, m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Player_Camera(LAYER_TYPE::LAYER_CAMERA)))
		return E_FAIL;

	Start_Collision();

	CChat_Manager::GetInstance()->Set_Active(true);


	while (true)
	{
		if (CServerSessionManager::GetInstance()->Get_ServerSession()->Get_LevelState() == LEVELSTATE::INITEND)
			break;
	}


	return S_OK;
}

HRESULT CLevel_Bern::Tick(_float fTimeDelta)
{
	/*if(KEY_TAP(KEY::F9))
		m_pRendererCom->Set_StaticShadow();*/

	if ((KEY_HOLD(KEY::SHIFT)) && (KEY_TAP(KEY::V)))
	{
		CUI_DamageFont* pDamageFont =  CPool<CUI_DamageFont>::Get_Obj();
		pDamageFont->Print_DamageFont(Vec3(0.f, 0.f, 0.f), 12.f, true, 1001235123);
	}
	if ((KEY_HOLD(KEY::SHIFT)) && (KEY_TAP(KEY::B)))
	{
		CUI_DamageFont* pDamageFont = CPool<CUI_DamageFont>::Get_Obj();
		pDamageFont->Print_DamageFont(Vec3(0.f, 0.f, 0.f), 12.f, false, 100123123);
	}

	return S_OK;
}

HRESULT CLevel_Bern::LateTick(_float fTimeDelta)
{
	CUI_Tool::GetInstance()->LateTick();
	return S_OK;
}

HRESULT CLevel_Bern::Render_Debug()
{
	m_pImGuiManager->Tick();
	return S_OK;
}

HRESULT CLevel_Bern::Exit()
{
	End_Collision();
	End_Picking();
	CServerSessionManager::GetInstance()->Set_Player(nullptr);
	CPhysXMgr::GetInstance()->Reset();
	CNavigationMgr::GetInstance()->Reset_Navigation();
	CGameInstance::GetInstance()->Reset_Lights();
	CGameInstance::GetInstance()->StopSoundAll();
	CChat_Manager::GetInstance()->Set_Active(false);
	CUI_Tool::GetInstance()->Set_ToolMode(false);
	return S_OK;
}

HRESULT CLevel_Bern::Ready_Lights()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHTDESC			LightDesc;

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = LIGHTDESC::TYPE_POINT;
	//LightDesc.vPosition = Vec4(15.0f, 5.0f, 15.0f, 1.f);
	//LightDesc.fRange = 10.f;
	//LightDesc.vDiffuse = Vec4(1.f, 0.0f, 0.f, 1.f);
	//LightDesc.vAmbient = Vec4(0.5f, 0.5f, 0.5f, 1.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;

	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	//LightDesc.eType = LIGHTDESC::TYPE_POINT;
	//LightDesc.vPosition = Vec4(25.0f, 5.0f, 15.0f, 1.f);
	//LightDesc.fRange = 10.f;
	//LightDesc.vDiffuse = Vec4(0.0f, 1.f, 0.f, 1.f);
	//LightDesc.vAmbient = Vec4(0.5f, 0.5f, 0.5f, 1.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = Vec4(0.705f, -0.667f, -0.239f, 0.f);
	LightDesc.vDiffuse = Vec4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = Vec4(1.0f, 1.0f, 1.0f, 1.f);
	LightDesc.vSpecular = Vec4(1.f, 1.f, 1.f, 1.f);


	//CTexture* pStaticShadowMap = CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/LightMap/ArenaLight.dds");

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;


	Vec3 vLook = LightDesc.vDirection;
	vLook.Normalize();
	//Vec3 vPos = Vec3(-80.78f, 83.75f, 33.80f);
	//Matrix matLightView = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

	Vec3 vOffset = Vec3(-117.04f, 110.27f, 36.20f);

	pGameInstance->Ready_StaticLightMatrix(vOffset, vLook);
	vOffset.Normalize();
	vOffset *= 30.0f;
	pGameInstance->Ready_LightMatrix(vOffset, vLook);


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Bern::Ready_Layer_Camera(const LAYER_TYPE eLayerType)
{


	return S_OK;
}

HRESULT CLevel_Bern::Ready_Layer_Player(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Bern::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	//Load_ColMesh(LEVELID::LEVEL_ARENA, L"../Bin/Resources/ColMeshData/Arena.data");
	//Load_MapData(LEVELID::LEVEL_BERN, L"../Bin/Resources/MapData/Arena.data");
	//m_pRendererCom->Set_StaticShadowMap();

	//Prototype_GameObject_SkyBoxDay

	//pGameInstance->GetInstance()->Add_GameObject(LEVELID::LEVEL_ARENA, (_uint)LAYER_TYPE::LAYER_SKYBOX, L"Prototype_GameObject_SkyBoxDay");


	CStaticModel::MODELDESC Desc;
	Desc.strFileName = L"SM_ENV_TCHEXA_ArenaGround_Aa";
	Desc.iLayer = (_uint)LAYER_TYPE::LAYER_BACKGROUND;


	CGameObject* pObject = pGameInstance->Add_GameObject(LEVEL_BERN, Desc.iLayer, TEXT("Prototype_GameObject_StaticModel"), &Desc);
	if (nullptr == pObject)
	{
		Safe_Release(pGameInstance);
		return E_FAIL;
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Bern::Ready_Layer_Monster(const LAYER_TYPE eLayerType)
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

HRESULT CLevel_Bern::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pUI = pGameInstance->Add_GameObject(LEVEL_BERN, _uint(eLayerType), TEXT("Prototype_GameObject_ChatUI"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI(LEVEL_BERN, static_cast<CUI*>(pUI));

	pUI = pGameInstance->Add_GameObject(LEVEL_BERN, _uint(eLayerType), TEXT("Prototype_GameObject_IdentityGNUI"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI(LEVEL_BERN, static_cast<CUI*>(pUI));


	pUI = pGameInstance->Add_GameObject(LEVEL_BERN, _uint(eLayerType), TEXT("Prototype_GameObject_PlayerHPUI"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI(LEVEL_BERN, static_cast<CUI*>(pUI));

	pUI = pGameInstance->Add_GameObject(LEVEL_BERN, _uint(eLayerType), TEXT("Prototype_GameObject_PlayerMPUI"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI(LEVEL_BERN, static_cast<CUI*>(pUI));

	pUI = pGameInstance->Add_GameObject(LEVEL_BERN, _uint(eLayerType), TEXT("Prototype_GameObject_SpeechBubble"), 
		CServerSessionManager::GetInstance()->Get_Player());
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI(LEVEL_BERN, static_cast<CUI*>(pUI));

	pUI = pGameInstance->Add_GameObject(LEVEL_BERN, _uint(eLayerType), TEXT("Prototype_GameObject_QuickSkillUI"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI(LEVEL_BERN, static_cast<CUI*>(pUI));



	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Bern::Ready_Layer_Effect(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_Bern::Ready_Player_Camera(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();

	if (pPlayer == nullptr)
		return E_FAIL;

	CCamera_Player::PlayerCameraDesc CameraDesc;

	CameraDesc.tCameraDesc.iLayer = (_uint)eLayerType;
	CameraDesc.tCameraDesc.vEye = Vec4(0.f, 10.f, -10.f, 1.f);
	CameraDesc.tCameraDesc.vAt = Vec4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.tCameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.tCameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.tCameraDesc.fNear = 0.2f;
	CameraDesc.tCameraDesc.fFar = 1200.0f;

	CameraDesc.tCameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.tCameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.pPlayer = pPlayer;

	CGameObject* pCamera = pGameInstance->Add_GameObject(LEVEL_BERN, _uint(eLayerType), TEXT("Prototype_GameObject_Camera_Player"), &CameraDesc);
	if (pCamera == nullptr)
		return E_FAIL;

	pPlayer->Set_Camera(dynamic_cast<CCamera_Player*>(pCamera));

	Safe_Release(pGameInstance);

	return S_OK;
}



HRESULT CLevel_Bern::Send_UserInfo()
{
	if (CServerSessionManager::GetInstance()->Get_Player() == nullptr)
	{
		Protocol::S_LOGIN pkt;

		pkt.set_iclass(CServerSessionManager::GetInstance()->Get_Class());
		pkt.set_strnickname(CAsUtils::ToString(CServerSessionManager::GetInstance()->Get_NickName()));

		SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
		CServerSessionManager::GetInstance()->Send(pSendBuffer);

	}

	return S_OK;
}

void CLevel_Bern::Send_LevelState(LEVELSTATE eState)
{
	Protocol::S_LEVEL_STATE pkt;
	pkt.set_ilevelstate((uint32)eState);

	SendBufferRef sendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Get_ServerSession()->Send(sendBuffer);
}

void CLevel_Bern::Wait_ServerLevelState(LEVELSTATE eState)
{
	ServerSessionRef& ServerSession = CServerSessionManager::GetInstance()->Get_ServerSession();

	while (true)
	{
		if (ServerSession->Get_LevelState() == eState)
			break;
	}
}


HRESULT CLevel_Bern::Load_MapData(LEVELID eLevel, const wstring& szFullPath)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFullPath, FileMode::Read);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));


	_uint iSize = file->Read<_uint>();

	for (_uint i = 0; i < iSize; ++i)
	{
		wstring szModelName = CAsUtils::ToWString(file->Read<string>());
		Matrix	matWorld = file->Read<Matrix>();


		{
			CStaticModel::MODELDESC Desc;
			Desc.strFileName = szModelName;
			Desc.iLayer = (_uint)LAYER_TYPE::LAYER_BACKGROUND;


			CGameObject* pObject = pGameInstance->Add_GameObject(eLevel, Desc.iLayer, TEXT("Prototype_GameObject_StaticModel"), &Desc);
			if (nullptr == pObject)
			{
				Safe_Release(pGameInstance);
				return E_FAIL;
			}

			pObject->Get_TransformCom()->Set_WorldMatrix(matWorld);


			_uint iColliderCount = file->Read<_uint>();
			CStaticModel* pStaticModel = dynamic_cast<CStaticModel*>(pObject);


			for (_uint i = 0; i < iColliderCount; ++i)
			{
				pStaticModel->Add_Collider();

				CSphereCollider* pCollider = pStaticModel->Get_StaticCollider(i);

				{
					Vec3 vOffset = file->Read<Vec3>();
					pCollider->Set_Offset(vOffset);


					_float fRadius = file->Read<_float>();
					pCollider->Set_Radius(fRadius);


					pCollider->Update_Collider();
				}

				_bool bChild = file->Read<_bool>();

				if (bChild)
				{
					pStaticModel->Add_ChildCollider(i);

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

		
			//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_STATICSHADOW, pObject);
		}
	}




	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Bern::Load_ColMesh(LEVELID eLevel, const wstring& szFullPath)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFullPath, FileMode::Read);


	_uint iSize = file->Read<_uint>();

	for (_uint i = 0; i < iSize; ++i)
	{
		wstring szModelName = CAsUtils::ToWString(file->Read<string>());
		Matrix	matWorld = file->Read<Matrix>();

		CColMesh::MODELDESC Desc;
		Desc.strFileName = szModelName;
		Desc.iLayer = (_uint)LAYER_TYPE::LAYER_COLMESH;


		CGameObject* pObject = pGameInstance->Add_GameObject(eLevel, Desc.iLayer, TEXT("Prototype_GameObject_ColMesh"), &Desc);
		if (nullptr == pObject)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		pObject->Get_TransformCom()->Set_WorldMatrix(matWorld);


		_uint iColliderCount = file->Read<_uint>();
		CColMesh* pColMesh = dynamic_cast<CColMesh*>(pObject);


		for (_uint i = 0; i < iColliderCount; ++i)
		{
			pColMesh->Add_Collider();

			CSphereCollider* pCollider = pColMesh->Get_StaticCollider(i);

			{
				Vec3 vOffset = file->Read<Vec3>();
				pCollider->Set_Offset(vOffset);


				_float fRadius = file->Read<_float>();
				pCollider->Set_Radius(fRadius);


				pCollider->Update_Collider();
			}

			_bool bChild = file->Read<_bool>();

			if (bChild)
			{
				pColMesh->Add_ChildCollider(i);

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




	}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Bern::Ready_Events()
{

	return S_OK;
}

HRESULT CLevel_Bern::Ready_Renderer()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	m_pRendererCom = dynamic_cast<CRenderer*>(pGameInstance->Clone_Component(nullptr, LEVEL_STATIC, L"Prototype_Component_Renderer"));
	if (m_pRendererCom == nullptr)
		return E_FAIL;


	Safe_Release(pGameInstance);
	return S_OK;
}


void CLevel_Bern::Set_CheckGruop()
{
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER, (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER, (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER, (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER);
}

void CLevel_Bern::Start_Collision()
{
	Set_CheckGruop();

	m_pCollisionThread = new thread([=]()
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		CCollisionManager* pCollisionManager = CCollisionManager::GetInstance();
		pCollisionManager->AddRef();

		if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Collision_Bern"))))
			return FALSE;

		if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Collision_60_Bern"))))
			return FALSE;

		_float		fTimeAcc = 0.f;


		while (!pCollisionManager->Is_Stop())
		{
			fTimeAcc += pGameInstance->Compute_TimeDelta(TEXT("Timer_Collision_Bern"));

			if (fTimeAcc >= 1.f / 60.0f)
			{
				pCollisionManager->LateTick_Collision(pGameInstance->Compute_TimeDelta(TEXT("Timer_Collision_60_Bern")));
				fTimeAcc = 0.f;
			}
		}

		Safe_Release(pCollisionManager);

		Safe_Release(pGameInstance);

		ThreadManager::GetInstance()->DestroyTLS();
	});
}


void CLevel_Bern::End_Picking()
{
	CPickingMgr::GetInstance()->Set_Stop(true);
	m_pPickingThread->join();
	CPickingMgr::GetInstance()->Reset();
	Safe_Delete(m_pPickingThread);
}

void CLevel_Bern::End_Collision()
{
	CCollisionManager::GetInstance()->Set_Stop(true);
	m_pCollisionThread->join();
	CCollisionManager::GetInstance()->Reset();
	Safe_Delete(m_pCollisionThread);
}

CLevel_Bern * CLevel_Bern::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Bern*	pInstance = new CLevel_Bern(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Bern::Free()
{
	__super::Free();

	Safe_Release(m_pImGuiManager);
}
