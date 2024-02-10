#include "stdafx.h"
#include "GameInstance.h"
#include "Upgrade_Npc.h"
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

#include "UI_NPC_ItemUpgrade.h"
#include "ServerSessionManager.h"
#include "UI_Manager.h"

CUpgrade_Npc::CUpgrade_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CFunction_Npc(pDevice, pContext)
{
}

CUpgrade_Npc::CUpgrade_Npc(const CUpgrade_Npc& rhs)
	: CFunction_Npc(rhs)
{
}

HRESULT CUpgrade_Npc::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUpgrade_Npc::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	__super::Initialize(pArg);

	if (FAILED(Ready_UpgradeUI()))
		return E_FAIL;

	m_strObjectTag = TEXT("Upgrade_Npc");

	return S_OK;
}

void CUpgrade_Npc::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (LEVELID::LEVEL_TOOL_NPC == m_iCurrLevel)
		return;

}

void CUpgrade_Npc::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (LEVELID::LEVEL_TOOL_NPC == m_iCurrLevel)
		return;

}

HRESULT CUpgrade_Npc::Render()
{
	__super::Render();

	return S_OK;
}

void CUpgrade_Npc::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_NPC && (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER == pOther->Get_ColLayer()
		&& true == static_cast<CPlayer*>(pOther->Get_Owner())->Is_ClickNpc())
	{
		m_pUpgradeUI->Set_Active_UpGrade(true);
		static_cast<CPlayer*>(pOther->Get_Owner())->Set_ClickNpcState(false);
		CGameInstance::GetInstance()->StopSound(CHANNEL_BGM);
		CGameInstance::GetInstance()->PlayBGM(L"RomanticWeapon.wav", CHANNEL_BGM, 0.5f);
	}
}

void CUpgrade_Npc::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{

}

void CUpgrade_Npc::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{

}

HRESULT CUpgrade_Npc::Ready_Components()
{
	__super::Ready_Components();

	return S_OK;
}

HRESULT CUpgrade_Npc::Ready_UpgradeUI()
{
	if (LEVELID::LEVEL_TOOL_NPC == m_iCurrLevel)
		return S_OK;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pUpgradeUI = static_cast<CUI_NPC_ItemUpgrade*>(pGameInstance->Add_GameObject(m_iCurrLevel,
		(_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_Npc_Item_Upgrade"), this));
	if (m_pUpgradeUI == nullptr)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CUpgrade_Npc* CUpgrade_Npc::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUpgrade_Npc* pInstance = new CUpgrade_Npc(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUpgrade_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUpgrade_Npc::Clone(void* pArg)
{
	CUpgrade_Npc* pInstance = new CUpgrade_Npc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUpgrade_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUpgrade_Npc::Free()
{
	__super::Free();

	if (nullptr != m_pUpgradeUI)
	{
		m_pUpgradeUI->Set_Dead(true);
	}
}
