#include "stdafx.h"
#include "Boss_Valtan_RunningGhost.h"
#include "BehaviorTree.h"
#include "GameInstance.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include "BehaviorTree.h"
#include <Skill.h>
#include "ColliderOBB.h"
#include "RigidBody.h"
#include "NavigationMgr.h"
#include "Effect.h"
#include "Effect_Manager.h"
#include "Effect_Particle.h"

CBoss_Valtan_RunningGhost::CBoss_Valtan_RunningGhost(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBoss(pDevice, pContext)
{
}

CBoss_Valtan_RunningGhost::CBoss_Valtan_RunningGhost(const CBoss_Valtan_RunningGhost& rhs)
	: CBoss(rhs)
{
}

HRESULT CBoss_Valtan_RunningGhost::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Valtan_RunningGhost::Initialize(void* pArg)
{
	m_iMaxGroggyGauge = 1000;
	m_iGroggyGauge = m_iMaxGroggyGauge;
	m_iMaxHp = 1991561183;
	m_iHp = m_iMaxHp;
	m_bDummy = true;
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;
	m_iCurrLevel = Desc->iLevel;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	if (FAILED(Ready_DissolveTexture()))
		return E_FAIL;
	m_pRigidBody->SetMass(2.0f);
	m_pRigidBody->Set_Gravity(false);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Desc->vPos);
	m_vSpawnPosition = Desc->vPos;

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	m_tCullingSphere.Radius = 2.0f;

	if (FAILED(Ready_BehaviourTree()))
		return E_FAIL;
	m_vecAttackRanges.clear();
	m_fMoveSpeed = 4.f;
	m_vecAttackRanges.push_back(2.f);
	m_vecAttackRanges.push_back(3.5f);
	m_IsSuperArmor = true;
	m_fAttackRange = m_vecAttackRanges[0];
	m_fRootTargetDistance = 0.f;
	m_fNoticeRange = 150.f;
	m_iArmor = 2;
	m_iPhase = 1;
	m_fFontScale = 0.55f;
	m_bRender = false;
	m_pModelCom->Reserve_NextAnimation(m_pModelCom->Initailize_FindAnimation(TEXT("att_battle_18_02"), 0.9f), 0.2f, 0, 0);

	m_IsAlphaBlend = true;

	return S_OK;
}

void CBoss_Valtan_RunningGhost::Tick(_float fTimeDelta)
{
	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
	Update_StatusEffect(fTimeDelta);

	_float fDistance = (m_vTargetPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)).Length();
	if (m_bRender && m_pModelCom->Get_Anim_Frame(m_pModelCom->Get_CurrAnim()) > m_pModelCom->Get_Anim_MaxFrame(m_pModelCom->Get_CurrAnim()) - 5 && !m_pModelCom->IsNext())
	{
		for (auto& Effect : m_Effects)
		{
			Effect->EffectEnd();
		}
		m_Effects.clear();

		for (auto& Particle : m_Particles)
		{
			dynamic_cast<CEffect_Particle*>(Particle)->ParticleEnd();
		}
		m_Particles.clear();
		m_pRushEffect->EffectEnd();
		Set_Die();
	}
		
	
	if (m_bEffect == false)
	{
		Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		Vec3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vPos += vLook * 12.f;

		{
			Matrix matWorld = Matrix::CreateWorld(vPos + vLook * 7.5f, -vLook, Vec3(0.0f, 1.0f, 0.0f));

			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &matWorld;

			EFFECT_START_OUTLIST(L"VT_ChainDoor", &tDesc, m_Effects);
			EFFECT_START_OUTLIST(L"VT_ChainDoorParticle", &tDesc, m_Particles);
		}

		{
			Matrix matWorld = Matrix::CreateWorld(vPos + vLook * -7.5f, vLook, Vec3(0.0f, 1.0f, 0.0f));

			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &matWorld;

			EFFECT_START_OUTLIST(L"VT_ChainDoor", &tDesc, m_Effects);
			EFFECT_START_OUTLIST(L"VT_ChainDoorParticle", &tDesc, m_Particles);
		}

		m_bEffect = true;
	}
	
	if (m_pRushEffect != nullptr)
	{
		m_pRushEffect->Update_Pivot(m_pTransformCom->Get_WorldMatrix());
	}

	if (fDistance < 15.f && !m_bRender)
	{
		m_bRender = true;
		m_pModelCom->Reserve_NextAnimation(m_pModelCom->Initailize_FindAnimation(TEXT("att_battle_18_02"), 0.9f), 0.f, 0, 0);

		if (m_pRushEffect == nullptr)
		{
			vector<CEffect*> Effects;
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pTransformCom->Get_WorldMatrix();
			EFFECT_START_OUTLIST(L"ValtanRush", &tDesc, Effects);
		}

	}
}

void CBoss_Valtan_RunningGhost::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Set_Center_ToBone();
}

HRESULT CBoss_Valtan_RunningGhost::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return S_OK;
	_int	iDissolve = false;
	if (m_bDissolveIn || m_bDissolveOut)
	{
		_int   bDissolve = true;
		_int  bReverseDissovle = false;
		_float fDissolveDensity = 3.f;
		_float fDissolveValue = 0.1f;
		_float fDissolveColorValue = 0.02f;
		_int  bDissolveEmissive = true;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &bDissolve, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bReverseDissolve", &bReverseDissovle, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveDensity", &fDissolveDensity, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveValue", &fDissolveValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveColorValue", &fDissolveColorValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolveEmissive", &bDissolveEmissive, sizeof(_int))))
			return E_FAIL;

		_float g_fDissolveAmount = m_fDissolvetime / m_fMaxDissolvetime;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveAmount", &g_fDissolveAmount, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Texture("g_DissolveTexture", m_pDissolveTexture->Get_SRV())))
			return E_FAIL;

	}

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	_uint		iNumMeshes = m_pModelPartCom[(_uint)PARTS::GHOST]->Get_NumMeshes();

	for (_uint j = 0; j < iNumMeshes; ++j)
	{
		if (FAILED(m_pModelPartCom[(_uint)PARTS::GHOST]->Render_Alpha(m_pShaderCom, j)))
			return E_FAIL;

	}


	if (m_bDissolveIn || m_bDissolveOut)
	{
		_int bDissolve = false;
		_int   bReverseDissovle = false;
		_float fDissolveDensity = 1.f;
		_float fDissolveValue = 0.1f;
		_float fDissolveColorValue = 0.02f;
		_int   bDissolveEmissive = false;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &bDissolve, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bReverseDissolve", &bReverseDissovle, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveDensity", &fDissolveDensity, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveValue", &fDissolveValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveColorValue", &fDissolveColorValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolveEmissive", &bDissolveEmissive, sizeof(_int))))
			return E_FAIL;

		Vec4 vDissolveColor = Vec4::One;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomColor", &vDissolveColor, sizeof(Vec4))))
			return E_FAIL;
	}

	return S_OK;
}


HRESULT CBoss_Valtan_RunningGhost::Ready_Coliders()
{


	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderAttack"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;
		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, pCollider);
	}
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}

	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->SetActive(true);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Set_Radius(1.2f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Set_Offset(Vec3(0.46f, 0.f, -1.65f));
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS]->Set_BoneIndex(m_pModelCom->Find_BoneIndex(TEXT("bip001-spine")));
	Set_Atk(20);
	Set_Force(52.f);
	return S_OK;
}

HRESULT CBoss_Valtan_RunningGhost::Ready_Components()
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

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_Boss_Valtan";
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	Safe_Release(pGameInstance);
	strComName = L"Prototype_Component_Model_Boss_Valtan_Color_Ghost";
	if (FAILED(__super::Add_Component(CGameInstance::GetInstance()->Get_CurrLevelIndex(), strComName, TEXT("Com_Model_Valtan_Ghost"), (CComponent**)&m_pModelPartCom[(_uint)PARTS::GHOST])))
		return E_FAIL;

	m_vOriginScale.x = 1.f;
	m_vOriginScale.y = 1.f;
	m_vOriginScale.z = 1.f;

	m_pTransformCom->Set_Scale(m_vOriginScale);
	return S_OK;
}

HRESULT CBoss_Valtan_RunningGhost::Ready_BehaviourTree()
{
	return S_OK;
}

CBoss* CBoss_Valtan_RunningGhost::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Valtan_RunningGhost* pInstance = new CBoss_Valtan_RunningGhost(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_Valtan_RunningGhost");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Valtan_RunningGhost::Clone(void* pArg)
{
	CBoss_Valtan_RunningGhost* pInstance = new CBoss_Valtan_RunningGhost(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CBoss_Valtan_RunningGhost" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CBoss_Valtan_RunningGhost::Free()
{
	__super::Free();
	for (auto& Collider : m_Coliders)
		CCollisionManager::GetInstance()->Out_Colider(Collider.second);
}
