#include "stdafx.h"
#include "GameInstance.h"
#include "Guide_Valtan_Npc_Server.h"
#include "AsUtils.h"
#include "Player_Server.h"
#include "ChaosDungean_Server.h"
#include "PartyManager.h"
#include "Party_Server.h"
#include "ValtanMain_Server.h"

CGuide_Valtan_Npc_Server::CGuide_Valtan_Npc_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CNpc_Server(pDevice, pContext)
{
}

CGuide_Valtan_Npc_Server::CGuide_Valtan_Npc_Server(const CGuide_Valtan_Npc_Server& rhs)
	: CNpc_Server(rhs)
{
}

HRESULT CGuide_Valtan_Npc_Server::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGuide_Valtan_Npc_Server::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strObjectTag = TEXT("Guide_Valtan_Npc");

	return S_OK;
}

void CGuide_Valtan_Npc_Server::Tick(_float fTimeDelta)
{


}

void CGuide_Valtan_Npc_Server::LateTick(_float fTimeDelta)
{

}

HRESULT CGuide_Valtan_Npc_Server::Render()
{

	return S_OK;
}

HRESULT CGuide_Valtan_Npc_Server::Render_Debug()
{
	return S_OK;
}

HRESULT CGuide_Valtan_Npc_Server::Actice_Npc_Function(int32 iLevel, int32 iPlayerID)
{
	if (true == m_bFirst)
		return S_OK;

	m_bFirst = true;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	vector<CGameObject*> LevelObjects = pGameInstance->Find_GameObjects(LEVEL_BERN, (_uint)LAYER_TYPE::LAYER_PLAYER);

	if (LevelObjects.empty())
		return E_FAIL;

	vector<CPlayer_Server*> Players;

	for (auto& Object : LevelObjects)
	{
		CPlayer_Server* pPlayer = dynamic_cast<CPlayer_Server*>(Object);
		if (pPlayer)
			Players.push_back(pPlayer);
	}

	CValtanMain_Server::DUNGEANDESC tDesc;
	tDesc.Players = Players;
	pGameInstance->Add_GameObject(LEVEL_VALTANMAIN, (_uint)LAYER_TYPE::LAYER_BACKGROUND, L"Prototype_GameObject_ValtanMain", &tDesc);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CGuide_Valtan_Npc_Server::Ready_Components()
{

	return S_OK;
}

CGuide_Valtan_Npc_Server* CGuide_Valtan_Npc_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGuide_Valtan_Npc_Server* pInstance = new CGuide_Valtan_Npc_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CGuide_Valtan_Npc_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGuide_Valtan_Npc_Server::Clone(void* pArg)
{
	CGuide_Valtan_Npc_Server* pInstance = new CGuide_Valtan_Npc_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CGuide_Valtan_Npc_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGuide_Valtan_Npc_Server::Free()
{
	__super::Free();
}
