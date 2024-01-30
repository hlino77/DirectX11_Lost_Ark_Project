#include "stdafx.h"
#include "GameInstance.h"
#include "Skill_RisingSun_Server.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "GameSessionManager.h"
#include "CollisionManager.h"


CSkill_RisingSun_Server::CSkill_RisingSun_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkill_Server(pDevice, pContext)
{
}

CSkill_RisingSun_Server::CSkill_RisingSun_Server(const CSkill_RisingSun_Server& rhs)
	: CSkill_Server(rhs)
{
}

HRESULT CSkill_RisingSun_Server::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSkill_RisingSun_Server::Initialize(void* pArg)
{
	RisingSunDesc* pDesc = static_cast<RisingSunDesc*>(pArg);
	m_pSkillOwner = pDesc->pSkillOwner;
	m_iObjectID = pDesc->iObjectID;
	m_iCurrLevel = m_pSkillOwner->Get_CurrLevel();

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_bEnd = false;

    return S_OK;
}

void CSkill_RisingSun_Server::Tick(_float fTimeDelta)
{

}

void CSkill_RisingSun_Server::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_RisingSun_Server::Render()
{

	return S_OK;
}

void CSkill_RisingSun_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	
}

void CSkill_RisingSun_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSkill_RisingSun_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	
}

void CSkill_RisingSun_Server::Explosion()
{

}

void CSkill_RisingSun_Server::Buff_Player(_uint iPlayerID)
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


HRESULT CSkill_RisingSun_Server::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);

    return S_OK;
}



void CSkill_RisingSun_Server::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);

	Safe_Release(m_pTransformCom);
}

CSkill_RisingSun_Server* CSkill_RisingSun_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkill_RisingSun_Server* pInstance = new CSkill_RisingSun_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_RisingSun_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_RisingSun_Server::Clone(void* pArg)
{
	CSkill_RisingSun_Server* pInstance = new CSkill_RisingSun_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CSkill_RisingSun_Server" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}
