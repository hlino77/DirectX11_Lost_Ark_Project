#include "stdafx.h"
#include "SKill_Valtan_RainingAxe.h"
#include "GameInstance.h"
#include <ColliderFrustum.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include <ColliderDoughnut.h>
#include <Boss.h>
#include "Effect_Manager.h"
#include "Effect_Trail.h"
#include "AsUtils.h"

CSKill_Valtan_RainingAxe::CSKill_Valtan_RainingAxe(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CSkill(pDevice,pContext)
{
}

CSKill_Valtan_RainingAxe::CSKill_Valtan_RainingAxe(const CSKill_Valtan_RainingAxe& rhs)
          : CSkill(rhs)
{
}

HRESULT CSKill_Valtan_RainingAxe::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSKill_Valtan_RainingAxe::Initialize(void* pArg)
{
	RainAxeDesc* pDesc = static_cast<RainAxeDesc*>(pArg);
	if (FAILED(__super::Initialize(&pDesc->tSkillDesc)))
		return E_FAIL;
	m_fLastTime = 0.6f;
	m_fBlinkTime = 0.4f;
	m_SkillDesc.iAtk = 20;
	m_SkillDesc.fForce = 12.f;

	m_vOffsetTargetPos = m_vTargetPos = pDesc->vTargetPos;

	Vec3 vOffset;
	vOffset.x = CGameInstance::GetInstance()->Get_RandomFloat(-1.0f, 1.0f);
	vOffset.z = CGameInstance::GetInstance()->Get_RandomFloat(-1.0f, 1.0f);
	vOffset.y = CGameInstance::GetInstance()->Get_RandomFloat(0.6f, 1.0f);
	vOffset.Normalize();
	vOffset *= 20.0f;

	Vec3 vDir = -vOffset;
	vDir.y = 0.0f;
	vDir.Normalize();

	m_vOffsetTargetPos.y += 1.0f;
	m_vOffsetTargetPos += -vDir * 0.6f;

	m_vStartPos = m_vTargetPos + vOffset;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vStartPos);
	m_pTransformCom->LookAt(m_vStartPos + vDir);
	
	m_pTransformCom->Turn_Axis(m_pTransformCom->Get_TransformCom()->Get_State(CTransform::STATE_UP), XM_PI);
	m_pTransformCom->Turn_Axis(m_pTransformCom->Get_TransformCom()->Get_State(CTransform::STATE_RIGHT), -XM_PI * 0.75f);
	m_OriginWorldMatrix = m_pTransformCom->Get_WorldMatrix();

	m_bAttack = false;

	m_fWaiting = 1.0f;

	auto func = bind(&CSKill_Valtan_RainingAxe::Load_EffectPivotMatrix, this, placeholders::_1);
	TRAIL_START_OUTLIST(L"VTRainAxeTrail", func, m_Trails);

	{
		Matrix matWorld = XMMatrixIdentity();
		matWorld.Translation(m_vTargetPos);
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &matWorld;
		EFFECT_START(L"VTAxeRainWarning", &tDesc);
	}

	m_fMaxRadian = XM_PI * 2.0f * 5.0f;

	return S_OK;
}

void CSKill_Valtan_RainingAxe::Tick(_float fTimeDelta)
{
	if (m_fWaiting > 0.0f)
	{
		m_fWaiting -= fTimeDelta;
		return;
	}

	__super::Tick(fTimeDelta);

	if (m_bAttack == false)
	{
		m_fBlinkTime -= fTimeDelta;
		m_fBlinkTime = max(0.0f, m_fBlinkTime);
		_float fTimeRatio = 1.0f - m_fBlinkTime / 0.4f;
		Vec3 vPos = Vec3::Lerp(m_vStartPos, m_vOffsetTargetPos, fTimeRatio);
		_float fRadian = CAsUtils::Lerpf(0.0f, m_fMaxRadian, fTimeRatio);

		m_pTransformCom->Set_WorldMatrix(m_OriginWorldMatrix);
		m_pTransformCom->Turn_Axis(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), -fRadian);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

		if (m_fBlinkTime <= 0.0f)
		{
			if (m_strSoundTag.empty() == false && !m_bSoundOn)
			{
				CGameInstance::GetInstance()->PlaySoundFile(m_strSoundTag, CHANNEL_EFFECT);
				m_bSoundOn = true;
			}

			m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(true);

			m_bAttack = true;

			Matrix matWorld = XMMatrixIdentity();
			matWorld.Translation(m_vTargetPos);
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &matWorld;
			EFFECT_START(L"VTRainAxeExplosion", &tDesc);

			for (auto& Trail : m_Trails)
			{
				dynamic_cast<CEffect_Trail*>(Trail)->TrailEnd(1.0f);
			}
		}
	}
}

void CSKill_Valtan_RainingAxe::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
	
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_DebugObject(this);
	}
}

HRESULT CSKill_Valtan_RainingAxe::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_pTransformCom->Get_WorldMatrix(), sizeof(Matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Push_GlobalVP()))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom)))
		return E_FAIL;

	_float fRimLight = 0.8f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLight, sizeof(_float))))
		return E_FAIL;
	_int	iDissolve = false;

	if (m_fLastTime < 0.f)
	{
		iDissolve = true;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &iDissolve, sizeof(_int))))
			return E_FAIL;

		_float fDissolveAmount = -m_fLastTime/ 5.f;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveAmount", &fDissolveAmount, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Texture("g_DissolveTexture", static_cast<CMonster*>(m_pSkillOwner)->Get_DissolveTexture()->Get_SRV())))
			return E_FAIL;
	}

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

	iDissolve = false;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &iDissolve, sizeof(_bool))))
		return E_FAIL;
	_float fRimLightColor = 0.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
		return E_FAIL;
    return S_OK;
}

void CSKill_Valtan_RainingAxe::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_Valtan_RainingAxe::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CSKill_Valtan_RainingAxe::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
}


HRESULT CSKill_Valtan_RainingAxe::Ready_Coliders()
{
	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = false;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderSkill"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;

		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS, pCollider);
	}
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Radius(2.f);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(false);
	m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));

	for (auto& Collider : m_Coliders)
	{
		if (Collider.second)
		{
			CCollisionManager::GetInstance()->Add_Colider(Collider.second);
		}
	}
	return S_OK;
}

void CSKill_Valtan_RainingAxe::Load_EffectPivotMatrix(Matrix& matWorld)
{
	matWorld = XMMatrixIdentity();
	matWorld.Translation(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
}

HRESULT CSKill_Valtan_RainingAxe::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
         return E_FAIL;
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;
	///* For.Com_Model */
	if (FAILED(__super::Add_Component(m_iCurrLevel, TEXT("Prototype_Component_Model_Skill_Boss_Valtan_Axe"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	Vec3 vScale;
	vScale.x = 1.5f;
	vScale.y = 1.5f;
	vScale.z = 1.5f;

	m_pTransformCom->Set_Scale(vScale);
    return S_OK;
}

CSKill_Valtan_RainingAxe* CSKill_Valtan_RainingAxe::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSKill_Valtan_RainingAxe* pInstance = new CSKill_Valtan_RainingAxe(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSKill_Valtan_RainingAxe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSKill_Valtan_RainingAxe::Clone(void* pArg)
{
    CSKill_Valtan_RainingAxe* pInstance = new CSKill_Valtan_RainingAxe(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSKill_Valtan_RainingAxe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSKill_Valtan_RainingAxe::Free()
{
	__super::Free();
}

