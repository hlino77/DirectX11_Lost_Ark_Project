#include "stdafx.h"
#include "SKill_Valtan_RainingAxe.h"
#include "GameInstance.h"
#include <ColliderFrustum.h>
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include <ColliderDoughnut.h>
#include <Boss.h>

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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	m_fLastTime = 2.2f;
	m_fBlinkTime = 2.f;
	m_SkillDesc.iAtk = 20;
	m_SkillDesc.fForce = 12.f;
	m_vOffset = { 0.f,5.f,-3.f};
	return S_OK;
}

void CSKill_Valtan_RainingAxe::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fBlinkTime -= fTimeDelta;
	if (m_fBlinkTime < 0.f && m_fLastTime > 0.f)
		m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->SetActive(!m_Coliders[(_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS]->IsActive());

	m_vOffset = Vec3::Lerp(Vec3(0.f, 0.9f, 0.f), Vec3(0.f, 20.f, CGameInstance::GetInstance()->Random_Float(-20.f, 0.f)),  max(0.f, m_fBlinkTime));
}

void CSKill_Valtan_RainingAxe::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
	m_WorldMatrix = Matrix::CreateRotationY(XMConvertToRadians(180.f))* Matrix::CreateRotationX(XMConvertToRadians(-210.f+ max(0.f, m_fBlinkTime) * -1440.f))* Matrix::CreateTranslation(m_vOffset) * m_pTransformCom->Get_WorldMatrix();
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

	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_WorldMatrix, sizeof(Matrix))))
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

