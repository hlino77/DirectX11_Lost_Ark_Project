#include "stdafx.h"
#include "Monster_Crystal_Server.h"
#include "GameInstance.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "GameSessionManager.h"
#include "CollisionManager.h"
#include "RigidBody.h"
#include "NavigationMgr.h"



CMonster_Crystal_Server::CMonster_Crystal_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster_Server(pDevice, pContext)
{
}

CMonster_Crystal_Server::CMonster_Crystal_Server(const CMonster_Server& rhs)
	: CMonster_Server(rhs)
{
}

HRESULT CMonster_Crystal_Server::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Crystal_Server::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_szModelName = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;
	m_iCurrLevel = Desc->iLevel;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	if (FAILED(Ready_BehaviourTree()))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Desc->vPosition);



	m_vecAttackRanges.clear();
	m_vecAttackRanges.push_back(0.f);
	m_vecAttackRanges.push_back(0.f);
	m_fAttackRange = m_vecAttackRanges[0];
	m_fNoticeRange = 0.f;
	m_iMaxHp = 10;
	m_iHp = m_iMaxHp;

	return S_OK;
}

void CMonster_Crystal_Server::Tick(_float fTimeDelta)
{
}

void CMonster_Crystal_Server::LateTick(_float fTimeDelta)
{
}

HRESULT CMonster_Crystal_Server::Render()
{

	return S_OK;
}

void CMonster_Crystal_Server::Set_SlowMotion(_bool bSlow)
{
}
HRESULT CMonster_Crystal_Server::Send_Dead()
{
	Protocol::S_DELETEGAMEOBJECT pkt;

	pkt.set_ilevel(CGameInstance::GetInstance()->Get_CurrLevelIndex());
	pkt.set_ilayer(m_iLayer);
	pkt.set_iobjectid(m_iObjectID);

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);

	return S_OK;
}
void CMonster_Crystal_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CMonster_Crystal_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CMonster_Crystal_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}

void CMonster_Crystal_Server::Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy)
{
	WRITE_LOCK
	m_iHp -= iDamage;
	if (m_iHp < 0)
	{
		Set_Die();
	}
	Send_Collision(iDamage, Vec3(), STATUSEFFECT::EFFECTEND, fForce, 0.f, 0);
}

HRESULT CMonster_Crystal_Server::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;



	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

	return S_OK;

}

HRESULT CMonster_Crystal_Server::Ready_BehaviourTree()
{
	return S_OK;
}

CMonster_Crystal_Server* CMonster_Crystal_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_Crystal_Server* pInstance = new CMonster_Crystal_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMonster_Crystal_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_Crystal_Server::Clone(void* pArg)
{
	CMonster_Crystal_Server* pInstance = new CMonster_Crystal_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CMonster_Crystal_Server" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CMonster_Crystal_Server::Free()
{
	__super::Free();
}
