#include "stdafx.h"
#include "GameInstance.h"
#include "Guide_Chaos_Npc.h"
#include "UI_SpeechBubble.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "CollisionManager.h"
#include "Player.h"
#include "Player_Gunslinger.h"
#include "Player_Slayer.h"
#include "Player_Destroyer.h"
#include "Player_Bard.h"
#include "Player_Controller_GN.h"
#include "Controller_WR.h"
#include "Controller_WDR.h"
#include "Controller_MG.h"
#include "AsUtils.h"


#include "ServerSessionManager.h"
#include "UI_NPC_ChaosDungeon_NewWnd.h"

CGuide_Chaos_Npc::CGuide_Chaos_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CFunction_Npc(pDevice, pContext)
{
}

CGuide_Chaos_Npc::CGuide_Chaos_Npc(const CGuide_Chaos_Npc& rhs)
	: CFunction_Npc(rhs)
{
}

HRESULT CGuide_Chaos_Npc::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CGuide_Chaos_Npc::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	__super::Initialize(pArg);

	if (FAILED(Ready_ChaosEntranceUI()))
		return E_FAIL;


	m_strObjectTag = TEXT("Guide_Chaos_Npc");

	return S_OK;
}

void CGuide_Chaos_Npc::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (LEVELID::LEVEL_TOOL_NPC == m_iCurrLevel)
		return;

	Activate_GuideUI();
}

void CGuide_Chaos_Npc::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (LEVELID::LEVEL_TOOL_NPC == m_iCurrLevel)
		return;

}

HRESULT CGuide_Chaos_Npc::Render()
{
	__super::Render();

	return S_OK;
}

void CGuide_Chaos_Npc::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_NPC && (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER == pOther->Get_ColLayer()
		&& true == static_cast<CPlayer*>(pOther->Get_Owner())->Is_ClickNpc())
	{
		m_pChaosUI->Set_Active(true);
		static_cast<CPlayer*>(pOther->Get_Owner())->Set_ClickNpcState(false);
		m_bCallFunctionFirst = false;
	}
}

void CGuide_Chaos_Npc::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
	
}

void CGuide_Chaos_Npc::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{

}

HRESULT CGuide_Chaos_Npc::Ready_Components()
{
	__super::Ready_Components();

	return S_OK;
}

HRESULT CGuide_Chaos_Npc::Ready_ChaosEntranceUI()
{
	if (LEVELID::LEVEL_TOOL_NPC == m_iCurrLevel)
		return S_OK;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pChaosUI = static_cast<CUI_NPC_ChaosDungeon_NewWnd*>(pGameInstance->Add_GameObject(m_iCurrLevel,
		(_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_Npc_ChaosDungeonEntrance_NewWnd"), this));

	if (m_pChaosUI == nullptr)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CGuide_Chaos_Npc::Activate_GuideUI()
{
	if (true == m_pChaosUI->Is_Entered() && false == m_pChaosUI->Is_Active()
		&& false == m_bCallFunctionFirst)
	{
		m_bCallFunctionFirst = true;
		Send_UI_State();
	}		
}

void CGuide_Chaos_Npc::Send_UI_State()
{
	Protocol::S_NPC tDesc;

	tDesc.set_ilevel(m_iCurrLevel);
	tDesc.set_iplayerid(m_pCtrlPlayer->Get_ObjectID());
	tDesc.set_strnpcname(CAsUtils::W2S(m_strObjectTag));

	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(tDesc);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);
}

CGuide_Chaos_Npc* CGuide_Chaos_Npc::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGuide_Chaos_Npc* pInstance = new CGuide_Chaos_Npc(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CGuide_Chaos_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGuide_Chaos_Npc::Clone(void* pArg)
{
	CGuide_Chaos_Npc* pInstance = new CGuide_Chaos_Npc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CGuide_Chaos_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGuide_Chaos_Npc::Free()
{
	__super::Free();

	if (nullptr != m_pChaosUI)
	{
		m_pChaosUI->Set_Dead(true);
	}
}
