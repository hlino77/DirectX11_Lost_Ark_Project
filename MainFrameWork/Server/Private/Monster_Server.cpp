#include "stdafx.h"
#include "GameInstance.h"
#include "Monster_Server.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "GameSessionManager.h"
#include "CollisionManager.h"
#include "RigidBody.h"
#include "NavigationMgr.h"
#include "BehaviorTree.h"
#include "Skill_Server.h"

CMonster_Server::CMonster_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Monster", OBJ_TYPE::MONSTER)
{
}

CMonster_Server::CMonster_Server(const CMonster_Server& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster_Server::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMonster_Server::Initialize(void* pArg)
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
	m_pRigidBody->SetMass(2.0f);
	m_pRigidBody->Set_Gravity(false);

    return S_OK;
}

void CMonster_Server::Tick(_float fTimeDelta)
{
	if (m_IsSetuponCell)
	{
		CNavigationMgr::GetInstance()->SetUp_OnCell(m_iCurrLevel, this);
		m_pRigidBody->Tick(fTimeDelta);
	}

	if (m_pBehaviorTree != nullptr)
		m_pBehaviorTree->Tick(fTimeDelta);

	Find_NearTarget(fTimeDelta);

	m_fHitTerm -= fTimeDelta;
	Update_StatusEffect(fTimeDelta);

	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
}

void CMonster_Server::LateTick(_float fTimeDelta)
{
	if (m_PlayAnimation.valid())
	{
		m_PlayAnimation.get();
		Set_to_RootPosition(fTimeDelta, m_fRootTargetDistance);
	}

	Set_Colliders(fTimeDelta);

	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CMonster_Server::Render()
{
	return S_OK;
}

void CMonster_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	
}

void CMonster_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CMonster_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	
}

void CMonster_Server::Update_StatusEffect(_float fTimeDelta)
{
	for (size_t i = 0; i < (_uint)STATUSEFFECT::EFFECTEND; i++)
	{
		if (m_fStatusEffects[i] > 0)
			m_fStatusEffects[i] -= fTimeDelta;
		else
			m_fStatusEffects[i] = 0.f;
	}
}


_bool CMonster_Server::Is_Maz()
{
	for (size_t i = 1; i < (_uint)STATUSEFFECT::EFFECTEND; i++)
	{
		if (m_fStatusEffects[i]>0)
			return true;
	}
	return false;
}

_bool CMonster_Server::Is_Skill()
{
	if (m_iSkillStack > m_iMaxSkillStack || m_fSkillCoolDown > 15.f)
		return true;
	return false;
}



void CMonster_Server::Set_AttackRange(_int iRangeIndex)
{
	if (iRangeIndex > m_vecAttackRanges.size())
		return;
	m_fAttackRange = m_vecAttackRanges[iRangeIndex];
}

HRESULT CMonster_Server::Ready_Components()
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

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_" + m_strObjectTag;
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	


	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}


void CMonster_Server::Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta)
{
	m_pTransformCom->LookAt_Lerp_ForLand(vDir, 5.0f, fTimeDelta);
	m_pTransformCom->Go_Straight(fSpeed, fTimeDelta);
}

void CMonster_Server::Body_Collision(CGameObject* pObject)
{
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Vec3 vOtherPos = pObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

	Vec3 vDir = vPos - vOtherPos;
	_float fDistance = vDir.Length();

	if (fDistance < 0.5f)
	{
		vDir.Normalize();
		Vec3 vTargetPos = vOtherPos + vDir * 0.5f;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTargetPos);
	}

}

void CMonster_Server::Hit_Attack(CCollider* pCollider)
{
	CGameObject* pOwner = pCollider->Get_Owner();

	_uint iObjType = pOwner->Get_ObjectType();

	if (iObjType == OBJ_TYPE::SKILL)
	{
		_uint iSkillOwnerType = dynamic_cast<CSkill_Server*>(pOwner)->Get_SkillOwner()->Get_ObjectType();
		if (iSkillOwnerType != OBJ_TYPE::PLAYER)
			return;

		m_pHitObject = dynamic_cast<CSkill_Server*>(pOwner)->Get_SkillOwner();
	}
	else if (iObjType != OBJ_TYPE::PLAYER)
		return;
	else if (iObjType == OBJ_TYPE::PLAYER)
		m_pHitObject = pOwner;

	m_iHp -= pCollider->Get_Attack();
	if (m_iHp < 0)
		m_iHp = 0;
	Send_Hp();


	_uint iAttackType = pCollider->Get_AttackType();


	switch (iAttackType)
	{
	case (_uint)COLLIDER_ATTACK::MIDDLE:
		if (m_iHp <= 0)
		{
			Set_State(L"Dying_Normal");
			break;
		}
		Set_State(L"Hit_Middle");
		break;
	case (_uint)COLLIDER_ATTACK::SPINBLOWUP:
		Set_State(L"Hit_SpinBlowUp");
		break;
	case (_uint)COLLIDER_ATTACK::SPINBLOWDOWN:
		Set_State(L"Hit_SpinBlowDown");
		break;
	}

	if (pCollider->Get_SlowMotion())
		Set_SlowMotion(true);

}

void CMonster_Server::Set_Die()
{
	m_IsHit = true;
	m_bDead = true;
}

void CMonster_Server::Set_SlowMotion(_bool bSlow)
{
	if (bSlow)
	{
		if (m_iSlowMotionCount <= 0)
		{
			m_fAttackMoveSpeed = 0.1f;
			m_fAnimationSpeed = 0.01f;
			m_pRigidBody->Set_Active(false);
		}

		m_iSlowMotionCount++;
	}
	else
	{
		--m_iSlowMotionCount;

		if (m_iSlowMotionCount <= 0)
		{
			m_fAttackMoveSpeed = 8.0f;
			m_fAnimationSpeed = 1.0f;
			m_pRigidBody->Set_Active(true);
			m_iSlowMotionCount = 0;
		}
	}

	Send_SlowMotion(bSlow);
}

void CMonster_Server::Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy)
{
	WRITE_LOCK
	m_iHp -= iDamage;
	_float fStatusDuration = fDuration;

	if (!m_IsSuperArmor)
	{
		m_IsAttacked = true;
		if (m_IsHit || m_IsSecondHit)
		{
			if (fForce < 20 && 100 != fStatusDuration)
			{
				fForce *= 0.2f;
			}
		}
		if (100 <= fStatusDuration)
		{
			fStatusDuration -= 100.f;
		}
		if (m_IsHit&&m_fHitTerm <0.f)
		{
			m_IsHit = false;
			m_IsSecondHit = true;
			m_fHitTerm = 0.2f;
		}
		else if( m_fHitTerm < 0.f)
		{
			m_IsSecondHit = false;
			m_IsHit = true;
			m_fHitTerm = 0.2f;
		}
		Vec3 vLook = {};
		Vec3 vBack = {};

		if (vHitPos.y == 0.f) 
		{
			
			m_pTransformCom->LookAt_Dir(vHitPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION));
			vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			vBack = -vLook;
			vBack.Normalize();

		}
		else if (vHitPos.y == 1.f)
		{
			vBack = vHitPos;
			vBack.y = 0.f;
			vLook = -vBack;
			vLook.Normalize();
			m_pTransformCom->LookAt_Dir(vLook);
		}
		vBack = m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.f;
		vBack.Normalize();
		if (fForce < 20.f)
		{
			m_pRigidBody->ClearForce(ForceMode::FORCE);
			m_pRigidBody->ClearForce(ForceMode::VELOCITY_CHANGE);
			m_pRigidBody->AddForce(vBack * fForce, ForceMode::FORCE);
		}
		else if (fForce < 30.f)
		{
			m_pRigidBody->ClearForce(ForceMode::FORCE);
			m_pRigidBody->ClearForce(ForceMode::VELOCITY_CHANGE);

			m_pRigidBody->AddForce(vBack * (fForce - 20.f), ForceMode::FORCE);
			m_IsBound = true;
		}
		else if (fForce >= 30.f)
		{
			m_pRigidBody->ClearForce(ForceMode::FORCE);
			m_pRigidBody->ClearForce(ForceMode::VELOCITY_CHANGE);
		
			m_pRigidBody->AddForce(vBack * (fForce - 30.f), ForceMode::FORCE);
			m_IsBound = false;
			m_IsTwist = true;
		}
	}

	if (iStatusEffect > (_int)STATUSEFFECT::GROGGY && fStatusDuration !=0&&iStatusEffect < (_int)STATUSEFFECT::SILENCE)
	{
		for (size_t i = 0; i < (_uint)STATUSEFFECT::EFFECTEND; i++)
		{
				m_fStatusEffects[i] = 0;
		}
		m_fStatusEffects[iStatusEffect] += fStatusDuration;
	}
	Send_Collision(iDamage, vHitPos,iStatusEffect, fForce, fDuration, iGroggy);
}

void CMonster_Server::Send_Collision(_uint iDamage, Vec3 vHitPos, _uint iEffect, _float fForce, _float fDuration, _uint iGroggy)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	pGameInstance->AddRef();

	Protocol::S_COLLISION pkt;

	pkt.set_ilevel(m_iCurrLevel);
	pkt.set_ilayer(m_iLayer);
	pkt.set_iobjectid(m_iObjectID);

	pkt.set_idamage(iDamage);
	pkt.set_istatuseffect(iEffect);
	pkt.set_fforce(fForce);
	pkt.set_fduration(fDuration);
	pkt.set_igroggy(iGroggy);

	auto vSendHitPos = pkt.mutable_vhitpos();
	vSendHitPos->Resize(3, 0.0f);
	memcpy(vSendHitPos->mutable_data(), &vHitPos, sizeof(Vec3));

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);

	Safe_Release(pGameInstance);
}



void CMonster_Server::Find_NearTarget(_float fTimeDelta)
{
	m_fScantime += fTimeDelta;
	if (m_fScantime > 1.f)
	{
		m_fScantime = 0.f;
		_float fDistance = 99999.f;
		CGameObject* pNearTarget = nullptr;

		vector<CGameObject*> pTargets = CGameInstance::GetInstance()->Find_GameObjects(m_iCurrLevel, (_uint)LAYER_TYPE::LAYER_PLAYER);
		if (pTargets.empty())
			m_pNearTarget = nullptr;
		else
		{
			for (auto& Object : pTargets)
			{
				_int iObjectID = Object->Get_ObjectID();
				if (Object->Is_Dead() || Object->Is_Active() == false || (Object)->Get_ServerState() == L"Dead_Start" || (Object)->Get_ServerState() == L"Dead_End")
					continue;

				if (pNearTarget == nullptr)
				{
					pNearTarget = Object;
					Vec3 vCallObjectPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
					Vec3 vObjectPos = Object->Get_TransformCom()->Get_State(CTransform::STATE::STATE_POSITION);
					fDistance = (vCallObjectPos - vObjectPos).Length();
					continue;
				}

				Vec3 vCallObjectPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
				Vec3 vObjectPos = Object->Get_TransformCom()->Get_State(CTransform::STATE::STATE_POSITION);

				_float fNewDistance = (vObjectPos - vCallObjectPos).Length();

				if (fNewDistance < fDistance)
				{
					pNearTarget = Object;
					fDistance = fNewDistance;
				}
			}

		}
		if (pNearTarget != m_pNearTarget)
		{
			m_pNearTarget = pNearTarget;
			Send_NearTarget();
		}
	}

}

void CMonster_Server::Send_NearTarget()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Protocol::S_NEARTARGET pkt;
	
	pkt.set_ilevel(m_iCurrLevel);
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_ilayer(m_iLayer);

	if (m_pNearTarget == nullptr)
	{
		pkt.set_itargetobjectid(-1);
		pkt.set_itargetobjectlayer(-1);
	}
	else
	{
		pkt.set_itargetobjectid(m_pNearTarget->Get_ObjectID());
		pkt.set_itargetobjectlayer(m_pNearTarget->Get_ObjectLayer());
	}

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);

	Safe_Release(pGameInstance);
}

_float CMonster_Server::Get_NearTargetDistance()
{
	if (m_pNearTarget == nullptr)
		return 10000.0f;

	Vec3 vTargetPos = m_pNearTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float fDistance = (vTargetPos - vPos).Length();
	return fDistance;
}

Vec3 CMonster_Server::Get_Target_Direction()
{
	if (m_pNearTarget == nullptr)
		return  m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	Vec3 vTargetPosition = m_pNearTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Vec3 vDirection = vTargetPosition - vCurrentPosition;

	vDirection.Normalize();
	return vDirection;
}

Vec3 CMonster_Server::Get_Target_RandomDirection()
{

	Vec3 vTargetPosition = m_pNearTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION)+m_vRandomPosition;
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Vec3 vDirection = vTargetPosition - vCurrentPosition;

	vDirection.Normalize();
	return vDirection;
}

_bool CMonster_Server::Is_Close_To_TargetRandomPosition()
{
	if (m_pNearTarget == nullptr)
		return  false;
	Vec3 vTargetPosition = m_pNearTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION) + m_vRandomPosition;
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if ((vTargetPosition - vCurrentPosition).Length() < 1.5f)
		return true;
	else
		return false;
}

void CMonster_Server::Set_RandomPosition()
{
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vTargetPos = m_vRandomPosition = Vec3(vCurrentPosition.x + CGameInstance::GetInstance()->Get_RandomFloat(-5, 5), vCurrentPosition.y, vCurrentPosition.z + CGameInstance::GetInstance()->Get_RandomFloat(-5, 5));

}

void CMonster_Server::Set_RandomPosition(_float fRange)
{
	float	fRandomRange = max(1.5f, fRange);
	m_vTargetPos = m_vRandomPosition = Vec3(CGameInstance::GetInstance()->Get_RandomFloat(-fRandomRange, fRandomRange), 0.f, CGameInstance::GetInstance()->Get_RandomFloat(-fRandomRange, fRandomRange));

}

void CMonster_Server::Move_to_RandomPosition(_float fTimeDelta)
{
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Move_Dir(m_vRandomPosition - vCurrentPosition, 1.f, fTimeDelta);
}

_float CMonster_Server::Get_Target_Distance()
{
	if (m_pNearTarget == nullptr)
		return 99999.f;

	Vec3 vTargetPosition = m_pNearTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	return (vTargetPosition - vCurrentPosition).Length();
}

_bool CMonster_Server::Is_Close_To_RandomPosition()
{
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if ((m_vRandomPosition - vCurrentPosition).Length() < 0.2f)
		return true;
	else
		return false;
}

Vec3 CMonster_Server::Get_NearTarget_Position()
{
	if (m_pNearTarget == nullptr)
		return Vec3();

	return m_pNearTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
}

void CMonster_Server::LookAt_Target_Direction_Lerp(_float fTimeDelta)
{
	if (m_pNearTarget == nullptr)
		return;

	Vec3 vTargetPosition = m_pNearTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if ((vTargetPosition - vCurrentPosition).Length() > 0.05f)
		m_pTransformCom->LookAt_Lerp_ForLand(vTargetPosition- vCurrentPosition, 4.0f, fTimeDelta);
}

void CMonster_Server::LookAt_Target_Direction()
{
	if (m_pNearTarget == nullptr)
		return;

	Vec3 vTargetPosition = m_pNearTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->LookAt_Dir(vTargetPosition - vCurrentPosition);
}

void CMonster_Server::Send_State(const wstring& szName)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Protocol::S_STATE pkt;
	pkt.set_strstate(CAsUtils::ToString(szName));

	auto tObject = pkt.mutable_tobject();

	tObject->set_ilevel(pGameInstance->Get_CurrLevelIndex());
	tObject->set_ilayer(m_iLayer);
	tObject->set_iobjectid(m_iObjectID);

	auto vTargetPos = tObject->mutable_vtargetpos();
	vTargetPos->Resize(3, 0.0f);
	Vec3 vPlayerTargetPos = m_vTargetPos.load();
	memcpy(vTargetPos->mutable_data(), &vPlayerTargetPos, sizeof(Vec3));


	auto matWorld = tObject->mutable_matworld();
	matWorld->Resize(16, 0.0f);
	Matrix matPlayerWorld = m_pTransformCom->Get_WorldMatrix();
	memcpy(matWorld->mutable_data(), &matPlayerWorld, sizeof(Matrix));

	if (m_pNearTarget == nullptr)
		pkt.set_itargetobjectid(-1);
	else
	{
		pkt.set_itargetobjectid(m_pNearTarget->Get_ObjectID());
		pkt.set_itargetobjectlayer(m_pNearTarget->Get_ObjectLayer());
	}

	if (m_pHitObject == nullptr)
		pkt.set_ihitobjectid(-1);
	else
	{
		pkt.set_ihitobjectid(m_pHitObject->Get_ObjectID());
		pkt.set_ihitobjectlayer(m_pHitObject->Get_ObjectLayer());
	}

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);

	Safe_Release(pGameInstance);
}


void CMonster_Server::Set_State(const wstring& szName)
{
	m_pStateMachine->Change_State(szName);
	Send_State(szName);
}

void CMonster_Server::Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame)
{
	m_pModelCom->Reserve_NextAnimation(iAnimIndex, fChangeTime, iStartFrame, iChangeFrame);
}

void CMonster_Server::Send_ColliderState(const _uint& iLayer)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CSphereCollider* pCollider = m_Coliders[iLayer];

	Protocol::S_COLLIDERSTATE pkt;
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_ilevel(pGameInstance->Get_CurrLevelIndex());
	pkt.set_ilayer(m_iLayer);

	pkt.set_icollayer(pCollider->Get_ColLayer());
	pkt.set_bactive(pCollider->IsActive());
	pkt.set_fradius(pCollider->Get_Radius());
	pkt.set_iattacktype(pCollider->Get_AttackType());
	pkt.set_iattack(pCollider->Get_Attack());
	pkt.set_bslow(pCollider->Get_SlowMotion());


	auto vOffset = pkt.mutable_voffset();
	vOffset->Resize(3, 0.0f);
	Vec3 vColliderOffset = pCollider->Get_Offset();
	memcpy(vOffset->mutable_data(), &vColliderOffset, sizeof(Vec3));

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);

	Safe_Release(pGameInstance);
}

void CMonster_Server::Send_SlowMotion(_bool bSlow)
{
	Protocol::S_SLOWMOTION pkt;


	pkt.set_ilevel(CGameInstance::GetInstance()->Get_CurrLevelIndex());
	pkt.set_ilayer(m_iLayer);
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_bslow(bSlow);

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);
}

void CMonster_Server::Send_Hp()
{
	Protocol::S_HP pkt;

	pkt.set_ilevel(CGameInstance::GetInstance()->Get_CurrLevelIndex());
	pkt.set_ilayer(m_iLayer);
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_ihp((int32)m_iHp);

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);

}

void CMonster_Server::Send_Monster_Action()
{
	Protocol::S_MONSTERSTATE pkt;

	pkt.set_ilevel(m_iCurrLevel);
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_ilayer(m_iLayer);
	pkt.set_strstate(CAsUtils::ToString(m_strAction));
	if (m_pNearTarget == nullptr)
		pkt.set_itargetobjectid(-1);
	else
	{
		pkt.set_itargetobjectid(m_pNearTarget->Get_ObjectID());
		pkt.set_itargetobjectlayer(m_pNearTarget->Get_ObjectLayer());
	}
	
	auto vTargetPos = pkt.mutable_vtargetpos();
	vTargetPos->Resize(3, 0.0f);
	Vec3 vPlayerTargetPos = m_vTargetPos.load();
	memcpy(vTargetPos->mutable_data(), &vPlayerTargetPos, sizeof(Vec3));

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);
}

void CMonster_Server::Set_Colliders(_float fTimeDelta)
{
	
}

void CMonster_Server::Set_to_RootPosition(_float fTimeDelta, _float _TargetDistance)
{
	if(Get_Target_Distance() > _TargetDistance)
	m_pModelCom->Set_ToRootPos(m_pTransformCom);
}

HRESULT CMonster_Server::Ready_Coliders()
{
	

	return S_OK;
}



CGameObject* CMonster_Server::Clone(void* pArg)
{
	CMonster_Server* pInstance = new CMonster_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CMonster" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Server::Free()
{
	__super::Free();



	Safe_Release(m_pModelCom);
	Safe_Release(m_pBehaviorTree);
	Safe_Release(m_pTransformCom);
}
