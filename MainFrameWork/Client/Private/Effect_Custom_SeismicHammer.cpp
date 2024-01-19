#include "stdafx.h"
#include "Effect_Custom_SeismicHammer.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"

CEffect_Custom_SeismicHammer::CEffect_Custom_SeismicHammer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Custom_SeismicHammer::CEffect_Custom_SeismicHammer(const CEffect_Custom_SeismicHammer& rhs)
	: Super(rhs)
{
}

HRESULT CEffect_Custom_SeismicHammer::Initialize_Prototype(EFFECTDESC* pDesc)
{
	return S_OK;
}

HRESULT CEffect_Custom_SeismicHammer::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	EffectDesc* pDesc = static_cast<EffectDesc*>(pArg);
	pDesc->vLook.Normalize();

	pDesc->vPos.y -= 0.2f;
	m_vTargetPos = pDesc->vPos;
	Vec3 vPos = m_vTargetPos;
	vPos.y -= 3.0f;
	vPos += -pDesc->vLook * 1.0f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);


	m_pTransformCom->LookAt(vPos + pDesc->vLook);
	m_pTransformCom->Turn_Axis(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), XM_PI * 0.5f);
	m_pTransformCom->Turn_Axis(m_pTransformCom->Get_State(CTransform::STATE_LOOK), XM_PI * 0.5f);

	m_fLifeTime = 1.3f;
	m_fTimeAcc = 0.0f;

	m_Intensity.fDissolveAmount = 0.0f;
	
	m_eState = SeismicHammerState::START;

	return S_OK;
}

void CEffect_Custom_SeismicHammer::Tick(_float fTimeDelta)
{
	switch (m_eState)
	{
	case SeismicHammerState::START:
		Tick_Start(fTimeDelta);
		break;
	case SeismicHammerState::IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case SeismicHammerState::DISAPPEAR:
		Tick_Disappear(fTimeDelta);
		break;
	}
	
}

void CEffect_Custom_SeismicHammer::LateTick(_float fTimeDelta)
{
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	}
}

HRESULT CEffect_Custom_SeismicHammer::Render()
{
	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	if (FAILED(m_pDissolveTexture->Set_SRV(m_pShaderCom, "g_DissolveTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveAmount", &m_Intensity.fDissolveAmount, sizeof(_float))))
		return E_FAIL;

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshes(); ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			return E_FAIL;

		MaterialFlag tFlag = { Vec4(0.f, 0.f, 0.f, 0.f) };

		if (FAILED(m_pShaderCom->Bind_CBuffer("MaterialFlag", &tFlag, sizeof(MaterialFlag))))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, "PBR_DISSOLVE")))
			return E_FAIL;
	}


	return S_OK;
}

void CEffect_Custom_SeismicHammer::Tick_Start(_float fTimeDelta)
{
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	
	_float fDistance = (m_vTargetPos - vPos).Length();
	
	if (fDistance < 0.1f)
	{
		m_eState = SeismicHammerState::IDLE;
	}
	else
	{
		vPos = Vec3::Lerp(vPos, m_vTargetPos, fTimeDelta * 20.0f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
}

void CEffect_Custom_SeismicHammer::Tick_Idle(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc >= m_fLifeTime)
	{
		m_eState = SeismicHammerState::DISAPPEAR;
	}

}

void CEffect_Custom_SeismicHammer::Tick_Disappear(_float fTimeDelta)
{
	m_Intensity.fDissolveAmount += fTimeDelta;

	if (m_Intensity.fDissolveAmount > 1.1f)
	{
		Set_Active(true);
		Set_Dead(true);
	}

	m_pTransformCom->Turn_Axis(m_pTransformCom->Get_State(CTransform::STATE_UP), -0.5f * fTimeDelta);
}

HRESULT CEffect_Custom_SeismicHammer::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;
	
	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Dissolve1"),
		TEXT("Com_TextureDissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_ModelEffect"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_SeismicHammer";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	Vec3 vScale;
	vScale.y = 1.0f;
	vScale.z = 1.0f;
	vScale.x = 1.0f;

	m_pTransformCom->Set_Scale(vScale);

	return S_OK;
}

CEffect_Custom_SeismicHammer* CEffect_Custom_SeismicHammer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Custom_SeismicHammer* pInstance = new CEffect_Custom_SeismicHammer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(nullptr)))
	{
		MSG_BOX("Failed To Created : CEffect_Custom_SeismicHammer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Custom_SeismicHammer::Clone(void* pArg)
{
	CEffect_Custom_SeismicHammer* pInstance = new CEffect_Custom_SeismicHammer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Custom_SeismicHammer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Custom_SeismicHammer::Free()
{
	Super::Free();
}
