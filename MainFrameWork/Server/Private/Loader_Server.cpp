#include "stdafx.h"
#include "Loader_Server.h"
#include "GameInstance.h"
#include "GameSessionManager.h"
#include "Player_Server.h"
#include "Monster_Server.h"



CLoader_Server::CLoader_Server()
{
}

_uint APIENTRY ThreadEntry(void* pArg)
{

	CoInitializeEx(nullptr, 0);

	/* ���Ӱ� ������ �����尡 ���Ѵ�. */
	CLoader_Server* pLoader = (CLoader_Server*)pArg;

	pLoader->Loading();

	return 0;
}

HRESULT CLoader_Server::Initialize(LEVELID eNextLevel)
{
	InitializeCriticalSection(&m_Critical_Section);

	m_eNextLevel = eNextLevel;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntry, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

_int CLoader_Server::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr = 0;

	switch (m_eNextLevel)
	{
	case LEVEL_BERN:
		hr = Loading_For_Level_Bern();
		break;
	}

	if (FAILED(hr))
		return -1;

	LeaveCriticalSection(&m_Critical_Section);

	return 0;
}

HRESULT CLoader_Server::Loading_For_Level_Bern()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer_Server::Create(nullptr, nullptr))))
		return E_FAIL;



	Loading_Model_For_Level_Bern();

	Safe_Release(pGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader_Server::Loading_Model_For_Level_Bern()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	

	{
		wstring strFileName = L"Gunslinger";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(nullptr, nullptr, strFilePath, strFileName, false, false, PivotMatrix))))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);



	return S_OK;
}

CLoader_Server* CLoader_Server::Create(LEVELID eNextLevel)
{
	CLoader_Server* pInstance = new CLoader_Server();

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader_Server::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_Critical_Section);

	CloseHandle(m_hThread);
}