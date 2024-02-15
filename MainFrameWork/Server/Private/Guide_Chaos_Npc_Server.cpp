#include "stdafx.h"
#include "GameInstance.h"
#include "Guide_Chaos_Npc_Server.h"
#include "AsUtils.h"
#include "Player_Server.h"
#include "ChaosDungean_Server.h"
#include "PartyManager.h"
#include "Party_Server.h"

CGuide_Chaos_Npc_Server::CGuide_Chaos_Npc_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CNpc_Server(pDevice, pContext)
{
}

CGuide_Chaos_Npc_Server::CGuide_Chaos_Npc_Server(const CGuide_Chaos_Npc_Server& rhs)
	: CNpc_Server(rhs)
{
}

HRESULT CGuide_Chaos_Npc_Server::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGuide_Chaos_Npc_Server::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strObjectTag = TEXT("Guide_Chaos_Npc");

	return S_OK;
}

void CGuide_Chaos_Npc_Server::Tick(_float fTimeDelta)
{


}

void CGuide_Chaos_Npc_Server::LateTick(_float fTimeDelta)
{

}

HRESULT CGuide_Chaos_Npc_Server::Render()
{

	return S_OK;
}

HRESULT CGuide_Chaos_Npc_Server::Render_Debug()
{
	return S_OK;
}

HRESULT CGuide_Chaos_Npc_Server::Actice_Npc_Function(int32 iLevel, int32 iPlayerID)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer_Server* pPlayer = dynamic_cast<CPlayer_Server*>(pGameInstance->Find_GameObject(iLevel, (_uint)LAYER_TYPE::LAYER_PLAYER, iPlayerID));

	CParty_Server* pParty = pPlayer->Get_Party();

	vector<CPlayer_Server*> Players;
	if (pParty != nullptr)
	{
		Players = pParty->Get_Players();
	}
	else
	{
		Players.push_back(pPlayer);
	}

	if (Players.empty())
		return E_FAIL;

	CChaosDungean_Server::DUNGEANDESC tDesc;
	tDesc.eLevel = CHAOSDUNGEANLEVEL::LEVEL1;
	tDesc.Players = Players;

	pGameInstance->Add_GameObject(LEVEL_CHAOS_1, (_uint)LAYER_TYPE::LAYER_BACKGROUND, L"Prototype_GameObject_ChaosDungean", &tDesc);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CGuide_Chaos_Npc_Server::Ready_Components()
{

	return S_OK;
}

CGuide_Chaos_Npc_Server* CGuide_Chaos_Npc_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGuide_Chaos_Npc_Server* pInstance = new CGuide_Chaos_Npc_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CGuide_Chaos_Npc_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGuide_Chaos_Npc_Server::Clone(void* pArg)
{
	CGuide_Chaos_Npc_Server* pInstance = new CGuide_Chaos_Npc_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CGuide_Chaos_Npc_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGuide_Chaos_Npc_Server::Free()
{
	__super::Free();
}
