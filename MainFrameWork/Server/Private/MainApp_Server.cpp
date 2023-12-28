#include "stdafx.h"
#include "MainApp_Server.h"
//
#include "GameInstance.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include <tchar.h>
#include "Protocol.pb.h"
#include <iostream>
#include "Level_Loading_Server.h"
#include "StateMachine.h"
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include "DeadLockProfiler.h"
#include "SocketUtils.h"
#include "RigidBody.h"
#include "ColliderOBB.h"
#include "NavigationMgr.h"
#include "Renderer.h"
#include "EventMgr.h"
#include "LockFree_Transform.h"
#include "UseLock_Transform.h"
#include "BehaviorTree.h"

CMainApp_Server::CMainApp_Server()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp_Server::Initialize()
{
	SocketUtils::Init();

	CServerPacketHandler::Init();

	wstring strIP = L"127.0.0.1";

	ServerServiceRef service = std::make_shared<ServerService>(
		NetAddress(strIP, 7777),
		std::make_shared<IocpCore>(),
		std::make_shared<CGameSession>, // TODO : SessionManager 등
		10);

 	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 7; i++)
	{
		ThreadManager::GetInstance()->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	cout << "Server Open" << endl;

	if (FAILED(m_pGameInstance->Initialize_Engine_Server(LEVEL_END, _uint(LAYER_TYPE::LAYER_END))))
		return E_FAIL;

	if (FAILED(Initiailize_Server()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	/* 1-4. 게임내에서 사용할 레벨(씬)을 생성한다.   */
	if (FAILED(Open_Level(LEVEL_BERN)))
		return E_FAIL;


	return S_OK;
}

void CMainApp_Server::Tick(_float fTimeDelta)
{
	m_pGameInstance->FinalTick(fTimeDelta);

	CGameSessionManager::GetInstance()->Tick(fTimeDelta);

	m_pGameInstance->Tick_Server(fTimeDelta);

}

HRESULT CMainApp_Server::Render()
{
	m_pRenderer_Com->Draw_Server();

	m_pGameInstance->Render_Debug();
	return S_OK;
}

HRESULT CMainApp_Server::Initiailize_Server()
{
	CNavigationMgr::GetInstance()->Reserve_Manager(nullptr, nullptr);
	CCollisionManager::GetInstance()->Reserve_Manager((_uint)LAYER_COLLIDER::LAYER_END);
	CEventMgr::GetInstance()->Reserve_EventMgr(nullptr, nullptr);

	return S_OK;
}

HRESULT CMainApp_Server::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer_Com = CRenderer::Create(nullptr, nullptr))))
		return E_FAIL;
	Safe_AddRef(m_pRenderer_Com);

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		CLockFree_Transform::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"),
		CUseLock_Transform::Create(nullptr, nullptr))))
		return E_FAIL;

	/* For.Prototype_Component_State */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"),
		CStateMachine::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"),
		COBBCollider::Create(nullptr, nullptr))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"),
		CSphereCollider::Create(nullptr, nullptr))))
		return E_FAIL;

	/* For.Prototype_Component_RigidBody */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"),
		CRigidBody::Create(nullptr, nullptr))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_BehaviorTree"),
		CBehaviorTree::Create(nullptr, nullptr))))
		return E_FAIL;
	return S_OK;
}

HRESULT CMainApp_Server::Open_Level(LEVELID eLevelID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;


	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading_Server::Create(eLevelID))))
		return E_FAIL;

	return S_OK;
}

CMainApp_Server* CMainApp_Server::Create()
{
	CMainApp_Server* pInstance = new CMainApp_Server();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp_Server::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();

	ThreadManager::GetInstance()->Join();
	SocketUtils::Clear();
}
