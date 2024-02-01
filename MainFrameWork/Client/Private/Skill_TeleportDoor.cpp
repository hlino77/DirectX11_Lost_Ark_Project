#include "stdafx.h"
#include "Player.h"
#include "Skill_TeleportDoor.h"
#include "GameInstance.h"
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include "ServerSessionManager.h"
#include "Effect_Manager.h"
#include "Effect.h"


CSkill_TeleportDoor::CSkill_TeleportDoor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSkill_TeleportDoor::CSkill_TeleportDoor(const CSkill_TeleportDoor& rhs)
          : CSkill(rhs)
{
}

HRESULT CSkill_TeleportDoor::Initialize_Prototype()
{
	m_bEnter = false;
	m_bEnd = false;
	m_bSend = false;

    return S_OK;
}

HRESULT CSkill_TeleportDoor::Initialize(void* pArg)
{
	RisingSunDesc* tDesc = static_cast<RisingSunDesc*>(pArg);
	m_pSkillOwner = tDesc->pSkillOwner;
	m_iObjectID = tDesc->iObjectID;
	m_iCurrLevel = m_pSkillOwner->Get_CurrLevel();
	
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(*tDesc->matWorld);

	m_fDelay = 1.0f;

	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER_BUFF]->Update_Collider();
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER_BUFF]->SetActive(true);

	{
		CEffect_Manager::EFFECTPIVOTDESC tEffectDesc;
		tEffectDesc.pPivotMatrix = &m_pTransformCom->Get_WorldMatrix();
		EFFECT_START_OUTLIST(L"TeleportDoor", &tEffectDesc, m_Effects);
	}

    return S_OK;
}

void CSkill_TeleportDoor::Tick(_float fTimeDelta)
{
	if (0.f >= m_fDeadTime)
	{
		Effect_End();
		m_bEnd = true;
	}
	else
	{
		m_fDeadTime -= fTimeDelta;
	}

	if (m_bEnd == true)
	{
		if (m_fDelay <= 0.0f)
		{
			CCollisionManager::GetInstance()->Out_Colider(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER_BUFF]);
			Set_Dead(true);
		}
		else
		{
			m_fDelay -= fTimeDelta;
		}

		return;
	}

	if (m_bSend == false && m_bEnter == true)
	{
		if (KEY_TAP(KEY::G))
		{
			m_bSend = true;
			//패킷
			Send_Buff();
		}
	}
}

void CSkill_TeleportDoor::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_DebugObject(this);
}

HRESULT CSkill_TeleportDoor::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;
    return S_OK;
}


void CSkill_TeleportDoor::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (pOther->Get_Owner()->Get_ObjectType() == OBJ_TYPE::PLAYER && pOther->Get_Owner()->Is_Control())
	{
		m_bEnter = true;
	}
}

void CSkill_TeleportDoor::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{

}

void CSkill_TeleportDoor::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	if (pOther->Get_Owner()->Get_ObjectType() == OBJ_TYPE::PLAYER && pOther->Get_Owner()->Is_Control())
	{
		m_bEnter = false;
	}
}


void CSkill_TeleportDoor::Buff_Player(_uint iPlayerID)
{
	m_bEnd = true;
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER_BUFF]->SetActive(false);
	Effect_End();

	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	if (pPlayer->Get_ObjectID() == iPlayerID)
	{
		//테스트
		Matrix matPlayerWorld = pPlayer->Get_TransformCom()->Get_WorldMatrix();
		matPlayerWorld.Translation(m_pSkillOwner->Get_TransformCom()->Get_State(CTransform::STATE_POSITION));

		Protocol::S_PLAYERTELEPORT pkt;
		pkt.set_ilevel(pPlayer->Get_CurrLevel());
		pkt.set_iplayerid(iPlayerID);
		
		auto matWorld = pkt.mutable_matrix();
		matWorld->Resize(16, 0.0f);
		memcpy(matWorld->mutable_data(), &matPlayerWorld, sizeof(Matrix));

		CServerSessionManager::GetInstance()->Send(CClientPacketHandler::MakeSendBuffer(pkt));

		pPlayer->Set_Render(false);
		pPlayer->Set_NoneControlState(L"Idle");
		pPlayer->Get_TransformCom()->Set_WorldMatrix(matPlayerWorld);
		pPlayer->Set_Render(true);
	}
}

HRESULT CSkill_TeleportDoor::Ready_Coliders()
{
	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER_BUFF;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderSkill"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;

		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER_BUFF, pCollider);
	}
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER_BUFF]->Set_Radius(1.0f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER_BUFF]->SetActive(true);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER_BUFF]->Set_Offset(Vec3(0.0f, 0.0f, 0.0f));

	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}
	return S_OK;
}

HRESULT CSkill_TeleportDoor::Ready_Components()
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

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);


    return S_OK;
}

void CSkill_TeleportDoor::Send_Buff()
{
	Protocol::S_BUFF_SKILL pkt;
	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();

	pkt.set_iplayerid(pPlayer->Get_ObjectID());
	pkt.set_iskillid(m_iObjectID);
	pkt.set_ilevel(m_iCurrLevel);

	CServerSessionManager::GetInstance()->Send(CClientPacketHandler::MakeSendBuffer(pkt));
}

void CSkill_TeleportDoor::Effect_End()
{
	for (auto& Effect : m_Effects)
	{
		Effect->EffectEnd();
	}
}

CSkill_TeleportDoor* CSkill_TeleportDoor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_TeleportDoor* pInstance = new CSkill_TeleportDoor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSkill_TeleportDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_TeleportDoor::Clone(void* pArg)
{
    CSkill_TeleportDoor* pInstance = new CSkill_TeleportDoor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSkill_TeleportDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_TeleportDoor::Free()
{
	if (false == m_bEnd)
	{
		Effect_End();
	}

	__super::Free();
}

