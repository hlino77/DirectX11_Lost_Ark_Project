#include "stdafx.h"
#include "GameInstance.h"
#include "Skill_TeleportDoor_Server.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "GameSessionManager.h"
#include "CollisionManager.h"


CSkill_TeleportDoor_Server::CSkill_TeleportDoor_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkill_Server(pDevice, pContext)
{
}

CSkill_TeleportDoor_Server::CSkill_TeleportDoor_Server(const CSkill_TeleportDoor_Server& rhs)
	: CSkill_Server(rhs)
{
}

HRESULT CSkill_TeleportDoor_Server::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSkill_TeleportDoor_Server::Initialize(void* pArg)
{
	RisingSunDesc* pDesc = static_cast<RisingSunDesc*>(pArg);
	m_pSkillOwner = pDesc->pSkillOwner;
	m_iObjectID = pDesc->iObjectID;
	m_iCurrLevel = m_pSkillOwner->Get_CurrLevel();

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_bEnd = false;

	m_fDelay = 10.0f;

    return S_OK;
}

void CSkill_TeleportDoor_Server::Tick(_float fTimeDelta)
{
	if (m_fDelay <= 0.0f)
	{
		Set_Dead(true);
	}
	else
	{
		m_fDelay -= fTimeDelta;
	}
}

void CSkill_TeleportDoor_Server::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_TeleportDoor_Server::Render()
{

	return S_OK;
}

void CSkill_TeleportDoor_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	
}

void CSkill_TeleportDoor_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_TeleportDoor_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	
}

void CSkill_TeleportDoor_Server::Explosion()
{

}

void CSkill_TeleportDoor_Server::Buff_Player(_uint iPlayerID)
{
	WRITE_LOCK;
	if (m_bEnd == false)
	{
		m_bEnd = true;
		//ÆÐÅ¶

		Protocol::S_BUFF_SKILL pkt;
		pkt.set_ilevel(m_iCurrLevel);
		pkt.set_iplayerid(iPlayerID);
		pkt.set_iskillid(m_iObjectID);

		CGameSessionManager::GetInstance()->Broadcast(CServerPacketHandler::MakeSendBuffer(pkt));
	}
}


HRESULT CSkill_TeleportDoor_Server::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);

    return S_OK;
}



void CSkill_TeleportDoor_Server::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);

	Safe_Release(m_pTransformCom);
}

CSkill_TeleportDoor_Server* CSkill_TeleportDoor_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkill_TeleportDoor_Server* pInstance = new CSkill_TeleportDoor_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_TeleportDoor_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_TeleportDoor_Server::Clone(void* pArg)
{
	CSkill_TeleportDoor_Server* pInstance = new CSkill_TeleportDoor_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CSkill_TeleportDoor_Server" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}
