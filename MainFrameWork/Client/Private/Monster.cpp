#include "stdafx.h"
#include "GameInstance.h"
#include "Monster.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "Camera_Player.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include  "ColliderSphereGroup.h"
#include "RigidBody.h"
#include "NavigationMgr.h"
#include "Pool.h"
#include "BindShaderDesc.h"
#include "CollisionManager.h"
#include "BehaviorTree.h"
#include "Damage_Manager.h"


CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Monster", OBJ_TYPE::MONSTER)
{
}

CMonster::CMonster(const CMonster& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;


	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Desc->vPos);

	m_pRigidBody->SetMass(2.0f);

	Find_NearTarget();



    return S_OK;
}

void CMonster::Tick(_float fTimeDelta)
{
	CNavigationMgr::GetInstance()->SetUp_OnCell(this);
	if (!m_bDie)
		m_pBehaviorTree->Tick_Action(m_strAction, fTimeDelta);
	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
	Update_StatusEffect(fTimeDelta);
	m_pRigidBody->Tick(fTimeDelta);
}

void CMonster::LateTick(_float fTimeDelta)
{
	if (m_PlayAnimation.valid())
	{
		m_PlayAnimation.get();
		Set_to_RootPosition(fTimeDelta, m_fRootTargetDistance);
	}
	Set_Colliders(fTimeDelta);

	if (nullptr == m_pRendererCom)
		return;

	CullingObject();

	Set_Colliders(fTimeDelta);


	if (m_bRimLight)
	{
		m_fRimLightTime -= fTimeDelta;
		if (m_fRimLightTime <= 0.0f)
		{
			m_fRimLightTime = 0.0f;
			m_bRimLight = false;
		}
	}
}

HRESULT CMonster::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	m_PlayAnimation.get();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_pTransformCom->Get_WorldMatrix(), sizeof(Matrix))))
		return S_OK;

	GlobalDesc gDesc = {
		pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW),
		pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ),
		pGameInstance->Get_ViewProjMatrix(),
		pGameInstance->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW)
	};

	if (FAILED(m_pShaderCom->Bind_CBuffer("GlobalBuffer", &gDesc, sizeof(GlobalDesc))))
		return S_OK;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return S_OK;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
				return S_OK;
		}
		else
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 2)))
				return S_OK;
		}
	}

	Safe_Release(pGameInstance);



    return S_OK;
}

HRESULT CMonster::Render_ShadowDepth()
{

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ))))
		return S_OK;

	Matrix matLightVeiw = pGameInstance->Get_DirectionLightMatrix();

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &matLightVeiw)))
		return S_OK;



	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return S_OK;*/

			/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
				return E_FAIL;*/


		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 3)))
			return S_OK;
	}

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CMonster::Render_Debug()
{
	for (auto& Colider : m_Coliders)
	{
		if (Colider.second->IsActive())
		{
			Colider.second->DebugRender();
			if (Colider.second->Get_Child())
				Colider.second->Get_Child()->DebugRender();
		}

	}

	return S_OK;
}

void CMonster::Set_SlowMotion(_bool bSlow)
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
}
void CMonster::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER)
	{
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER)
		{
			//_int iDammage = dynamic_cast<CPlayer*>(pOther->Get_Owner())->
			Vec3 vPos = {};

			vPos = pOther->Get_Owner()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			_float fForce = 1.0f;

			Send_Collision(1, vPos,STATUSEFFECT::EFFECTEND, fForce,0.f);

			Show_Damage();
		}
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{

		}
	}
	if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER)
	{
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER)
		{
			//_int iDammage = dynamic_cast<CPlayer*>(pOther->Get_Owner())->
			Vec3 vPos = {};

			vPos = pOther->Get_Owner()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			_float fForce = 25.0f;

			Send_Collision(1, vPos, STATUSEFFECT::EFFECTEND, fForce, 0.f);

			Show_Damage();
		}
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{

		}
	}
	else if (iColLayer == (_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)
	{
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{
		}
		if (pOther->Get_ColLayer() == (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER)
		{
		}
	}
}

void CMonster::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CMonster::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{

}

void CMonster::Update_StatusEffect(_float fTimeDelta)
{
	for (size_t i = 0; i < (_uint)STATUSEFFECT::EFFECTEND; i++)
	{
		if (m_fStatusEffects[i] > 0)
			m_fStatusEffects[i] -= fTimeDelta;
		else
			m_fStatusEffects[i] = 0.f;
	}
}

int iTemp =0;

void CMonster::Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration)
{
	m_iHp -= iDamage;

	Vec3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	Vec3 vBack = -vLook;
	vBack.Normalize();
	if (!m_IsSuperArmor)
	{
		m_pTransformCom->LookAt(vHitPos);
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
			fForce = 1.f;
			m_pRigidBody->AddForce(vBack * fForce, ForceMode::FORCE);
		}
		else if (fForce >= 30.f)
		{
			m_pRigidBody->ClearForce(ForceMode::FORCE);
			m_pRigidBody->ClearForce(ForceMode::VELOCITY_CHANGE);
			fForce = 1.f;
			m_pRigidBody->AddForce(vBack * fForce, ForceMode::FORCE);
		}
	}
	m_fStatusEffects[iStatusEffect] += fDuration;
	cout << "CollisionCount	: " << iTemp++ << endl;


	Set_RimLight(0.05f);
}

void CMonster::Send_Collision(_uint iDamage, Vec3 vHitPos, STATUSEFFECT eEffect, _float fForce, _float fDuration)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	pGameInstance->AddRef();

	Protocol::S_COLLISION pkt;

	pkt.set_ilevel(pGameInstance->Get_CurrLevelIndex());
	pkt.set_ilayer((_uint)LAYER_TYPE::LAYER_MONSTER);
	pkt.set_iobjectid(m_iObjectID);

	pkt.set_idamage(iDamage);
	pkt.set_istatuseffect((_uint)eEffect);
	pkt.set_fforce(fForce);
	pkt.set_fduration(fDuration);
	
	auto vSendHitPos = pkt.mutable_vhitpos();
	vSendHitPos->Resize(3, 0.0f);
	memcpy(vSendHitPos->mutable_data(), &vHitPos, sizeof(Vec3));

	SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
	CServerSessionManager::GetInstance()->Send(pSendBuffer);

	Safe_Release(pGameInstance);
}

void CMonster::Find_NearTarget()
{
	m_pNearTarget = nullptr;
	m_pNearTarget = CGameInstance::GetInstance()->Find_NearGameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_PLAYER, this);
}



void CMonster::Follow_ServerPos(_float fDistance, _float fLerpSpeed)
{
	Vec3 vCurrPos = m_pTransformCom->Get_State(CTransform::STATE::STATE_POSITION);
	Matrix matTargetWorld = m_matTargetWorld;
	Vec3 vServerPos(matTargetWorld.m[3]);

	Vec3 vDistance = vServerPos - vCurrPos;
	if (vDistance.Length() > fDistance)
	{
		vCurrPos = Vec3::Lerp(vCurrPos, vServerPos, fLerpSpeed);
		m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCurrPos);
	}
}

void CMonster::Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta)
{
	m_pTransformCom->LookAt_Lerp(vDir, 5.0f, fTimeDelta);
	m_pTransformCom->Go_Straight(fSpeed, fTimeDelta);
}


_float CMonster::Get_Target_Distance()
{
	if (m_pNearTarget == nullptr)
		return 99999.f;

	Vec3 vTargetPosition = m_pNearTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	
	return (vTargetPosition - vCurrentPosition).Length();
}

Vec3 CMonster::Get_Target_Direction()
{
	Vec3 vTargetPosition = m_pNearTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	return (vTargetPosition - vCurrentPosition);
}

void CMonster::Set_RandomPosition()
{
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_vRandomPosition = Vec3(vCurrentPosition.x + CGameInstance::GetInstance()->Get_RandomFloat(-5.f, 5.f), vCurrentPosition.y, vCurrentPosition.z + CGameInstance::GetInstance()->Get_RandomFloat(-5.f, 5.f));

}

void CMonster::Move_to_RandomPosition(_float fTimeDelta)
{
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Move_Dir(m_vRandomPosition - vCurrentPosition, 1.f, fTimeDelta);
}

_bool CMonster::Is_Close_To_RandomPosition()
{
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if((m_vRandomPosition - vCurrentPosition).Length()<0.2f)
	return true;
	else
	return false;
}

void CMonster::LookAt_Target_Direction_Lerp(_float fTimeDelta)
{
	if (m_pNearTarget == nullptr)
		return;

	Vec3 vTargetPosition = m_pNearTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->LookAt_Lerp(vTargetPosition - vCurrentPosition, 5.0f, fTimeDelta);
}

void CMonster::LookAt_Target_Direction()
{
	if (m_pNearTarget == nullptr)
		return;

	Vec3 vTargetPosition = m_pNearTarget->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vCurrentPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->LookAt_Dir(vTargetPosition - vCurrentPosition);
}


void CMonster::Set_Die()
{
	for (auto& Collider : m_Coliders)
		Collider.second->SetActive(false);

	m_bDie = true;
}

void CMonster::Set_AttackRange(_int iRangeIndex)
{
	if (iRangeIndex > m_vecAttackRanges.size())
		return;
	m_fAttackRange = m_vecAttackRanges[iRangeIndex];

}


void CMonster::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

void CMonster::Set_Collider_Active(_uint eColliderType, _bool IsActive)
{
	if (m_Coliders.count(eColliderType) != 0)
		m_Coliders[eColliderType]->SetActive(IsActive);
}

void CMonster::Set_Collider_Info(_uint eColliderType, Vec3 _vCenter, _float fRadius)
{
	if (m_Coliders.count(eColliderType) != 0)
	{
		m_Coliders[eColliderType]->Set_Center(_vCenter);
		m_Coliders[eColliderType]->Set_Radius(fRadius);
	}
}

void CMonster::Show_Damage()
{
	int iTemp = rand() % 10;


	int iTestDamage = (rand() % 300) * 26789;

	if (iTemp < 3)
	{
		iTestDamage *= 2;
		CDamage_Manager::GetInstance()->Print_DamageFont(0.4f, 1.0f, m_pTransformCom->Get_TransformCom()->Get_State(CTransform::STATE_POSITION), 2.0f, true, iTestDamage);
	}
	else
	{
		CDamage_Manager::GetInstance()->Print_DamageFont(0.4f, 1.0f, m_pTransformCom->Get_TransformCom()->Get_State(CTransform::STATE_POSITION), 2.0f, false, iTestDamage);
	}


}

HRESULT CMonster::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_State */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateMachine)))
		return E_FAIL;

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_" + m_strObjectTag;
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = true;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_SphereColider"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;

		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER, pCollider);
	}

	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderAttack"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;

		if (pCollider)
			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, pCollider);
	}


	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}

HRESULT CMonster::Ready_HP_UI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	//Prototype_GameObject_UI_HP_Monster

	Safe_Release(pGameInstance);
	return S_OK;
}

void CMonster::CullingObject()
{
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_tCullingSphere.Center = vPos;

	const BoundingFrustum& tCamFrustum = CGameInstance::GetInstance()->Get_CamFrustum();

	if (tCamFrustum.Intersects(m_tCullingSphere) == false)
		return;
		
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
		m_pRendererCom->Add_DebugObject(this);
	}
		
}

void CMonster::Set_to_RootPosition(_float fTimeDelta, _float _TargetDistance)
{
	if (Get_Target_Distance() > _TargetDistance)
		m_pModelCom->Set_ToRootPos(m_pTransformCom, fTimeDelta);
}

void CMonster::Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame)
{
	m_pModelCom->Reserve_NextAnimation(iAnimIndex, fChangeTime, iStartFrame, iChangeFrame);
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pBehaviorTree);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
