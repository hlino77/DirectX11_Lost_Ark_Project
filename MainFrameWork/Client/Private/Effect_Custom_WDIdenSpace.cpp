#include "stdafx.h"
#include "Effect_Custom_WDIdenSpace.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"
#include "AsUtils.h"
#include "Controller_WDR.h"
#include "Player_Destroyer.h"

CEffect_Custom_WDIdenSpace::CEffect_Custom_WDIdenSpace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Custom_WDIdenSpace::CEffect_Custom_WDIdenSpace(const CEffect_Custom_WDIdenSpace& rhs)
	: Super(rhs)
{
}

HRESULT CEffect_Custom_WDIdenSpace::Initialize_Prototype(EFFECTDESC* pDesc)
{
	return S_OK;
}

HRESULT CEffect_Custom_WDIdenSpace::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	CustomEffectDesc* pDesc = static_cast<CustomEffectDesc*>(pArg);
	m_pOwner = pDesc->pOwner;

	m_tNoisMaskEmisDslv.NoisMaskEmisDslv = Vec4(1.0f, 1.0f, 1.0f, 0.0f);
	m_Variables.vColor_Offset = Vec4(1.0f, 1.0f, 1.0f, 0.0f);
	m_Variables.vColor_Clip = Vec4(0.1f, 0.1f, 0.1f, 0.1f);
	m_Variables.vColor_Mul = Vec4(0.1f, 1.1f, 3.0f, 1.5f);
	m_Variables.vUV_TileCount = Vec2(0.5f, 0.5f);
	m_Intensity.fBloom = 0.02f;

	m_pTransformCom->Turn_Axis(Vec3(1.0f, 0.0f, 0.0f), XM_PI * 1.5f);


	m_vTargetScale = Vec3(11.0f, 11.0f, 0.7f);
	m_pTransformCom->Set_Scale(Vec3(0.1f, 0.1f, 0.1f));

	m_fUVSpeed = 10.0f;

	m_eState = WDIDENSTATE::START;

	return S_OK;
}

void CEffect_Custom_WDIdenSpace::Tick(_float fTimeDelta)
{
	switch (m_eState)
	{
	case WDIDENSTATE::START:
		Tick_Start(fTimeDelta);
		break;
	case WDIDENSTATE::IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case WDIDENSTATE::IDENEND:
		Tick_End(fTimeDelta);
		break;
	}
}

void CEffect_Custom_WDIdenSpace::LateTick(_float fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	Vec3 vPos = m_pOwner->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	vPos.y += 0.2f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

HRESULT CEffect_Custom_WDIdenSpace::Render()
{
	if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Variables", &m_Variables, sizeof(tagFX_Variables))))
		return E_FAIL;

#pragma region GlobalData

	if (FAILED(m_pShaderCom->Push_GlobalVP()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_CBuffer("EffectMaterialFlag", &m_tNoisMaskEmisDslv.NoisMaskEmisDslv, sizeof(EffectMaterialFlag))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Intensity", &m_Intensity, sizeof(tagFX_Intensity))))
		return E_FAIL;

	if (m_pDiffuseTexture)
	{
		if (FAILED(m_pDiffuseTexture->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else
		__debugbreak();

	if (m_pMaskTexture)
	{
		if (FAILED(m_pMaskTexture->Set_SRV(m_pShaderCom, "g_MaskTexture")))
			return E_FAIL;
	}

	if (m_pNoiseTexture)
	{
		if (FAILED(m_pNoiseTexture->Set_SRV(m_pShaderCom, "g_NoiseTexture")))
			return E_FAIL;
	}


	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_pTransformCom->Get_WorldMatrix(), sizeof(Matrix))))
		return E_FAIL;

	_int iMeshCount = m_pModelCom->Get_Meshes().size();
	for (_int i = 0; i < iMeshCount; ++i)
	{
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, "WDIden")))
			return E_FAIL;
	}

	return S_OK;
}


HRESULT CEffect_Custom_WDIdenSpace::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectMesh"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_Effect_Custom_Cone2";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Default"),
		TEXT("Com_TextureDiffuse"), (CComponent**)&m_pDiffuseTexture)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise1"),
		TEXT("Com_TextureNoise"), (CComponent**)&m_pNoiseTexture)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mask1"),
		TEXT("Com_TextureMask"), (CComponent**)&m_pMaskTexture)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Custom_WDIdenSpace::Tick_Start(_float fTimeDelta)
{
	Vec3 vScale = m_pTransformCom->Get_Scale();

	vScale = Vec3::Lerp(vScale, m_vTargetScale, 3.0f * fTimeDelta);
	m_pTransformCom->Set_Scale(vScale);
	
	m_fUVSpeed = CAsUtils::Lerpf(m_fUVSpeed, 0.2f, 3.0f * fTimeDelta);

	m_Variables.vUV_Offset.y += -m_fUVSpeed * fTimeDelta;

	if (m_fUVSpeed - 0.2f < 0.01f)
		m_eState = WDIDENSTATE::IDLE;
}

void CEffect_Custom_WDIdenSpace::Tick_Idle(_float fTimeDelta)
{
	m_Variables.vUV_Offset.y += -m_fUVSpeed * fTimeDelta;

	if (m_fUVSpeed - 0.2f > 0.01f)
	{
		m_fUVSpeed = CAsUtils::Lerpf(m_fUVSpeed, 0.2f, 2.0f * fTimeDelta);
	}

	if (false == m_pOwner->Get_WDR_Controller()->Is_In_Identity())
	{
		m_eState = WDIDENSTATE::IDENEND;
		m_fUVSpeed = 10.0f;
	}
}

void CEffect_Custom_WDIdenSpace::Tick_End(_float fTimeDelta)
{
	Vec3 vScale = m_pTransformCom->Get_Scale();

	vScale = Vec3::Lerp(vScale, Vec3(0.1f, 0.1f, 0.1f), 5.0f * fTimeDelta);
	m_pTransformCom->Set_Scale(vScale);

	m_fUVSpeed = CAsUtils::Lerpf(m_fUVSpeed, 1.0f, 2.0f * fTimeDelta);

	m_Variables.vUV_Offset.y += m_fUVSpeed * fTimeDelta;

	if (vScale.x < 0.2f)
	{
		Set_Dead(true);
	}
}

CEffect_Custom_WDIdenSpace* CEffect_Custom_WDIdenSpace::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Custom_WDIdenSpace* pInstance = new CEffect_Custom_WDIdenSpace(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(nullptr)))
	{
		MSG_BOX("Failed To Created : CEffect_Custom_WDIdenSpace");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Custom_WDIdenSpace::Clone(void* pArg)
{
	CEffect_Custom_WDIdenSpace* pInstance = new CEffect_Custom_WDIdenSpace(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Custom_WDIdenSpace");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Custom_WDIdenSpace::Free()
{
	Super::Free();
}
