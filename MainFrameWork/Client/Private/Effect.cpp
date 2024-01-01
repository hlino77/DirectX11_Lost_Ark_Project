#include "stdafx.h"
#include "Effect.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "RigidBody.h"


CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Effect", OBJ_TYPE::EFFECT)
{
}

CEffect::CEffect(const CEffect& rhs)
	: Super(rhs)
	, m_vPosition_Start(rhs.m_vPosition_Start)
	, m_vPosition_End(rhs.m_vPosition_End)
	, m_bPosition_Lerp(rhs.m_bPosition_Lerp)
	, m_vRotation_Start(rhs.m_vRotation_Start)
	, m_vRotation_End(rhs.m_vRotation_End)
	, m_bRotation_Lerp(rhs.m_bRotation_Lerp)
	, m_vScaling_Start(rhs.m_vScaling_Start)
	, m_vScaling_End(rhs.m_vScaling_End)
	, m_bScaling_Lerp(rhs.m_bScaling_Lerp)
	, m_vVelocity_Start(rhs.m_vVelocity_Start)
	, m_vVelocity_End(rhs.m_vVelocity_End)
	, m_bVelocity_Lerp(rhs.m_bVelocity_Lerp)
	, m_vColor_Start(rhs.m_vColor_Start)
	, m_vColor_End(rhs.m_vColor_End)
	, m_bColor_Lerp(rhs.m_bColor_Lerp)
	, m_fLifeTime(rhs.m_fLifeTime)
	, m_vUV_Start(rhs.m_vUV_Start)
	, m_vUV_Speed(rhs.m_vUV_Speed)
	, m_IsSequence(rhs.m_IsSequence)
	, m_fSequenceTerm(rhs.m_fSequenceTerm)
	, m_Variables(rhs.m_Variables)
	, m_Intensity(rhs.m_Intensity)
	, m_pDiffuseTexture(rhs.m_pDiffuseTexture)
	, m_pNoiseTexture(rhs.m_pNoiseTexture)
	, m_pMaskTexture(rhs.m_pMaskTexture)
	, m_pEmissiveTexture(rhs.m_pEmissiveTexture)
	, m_pDissolveTexture(rhs.m_pDissolveTexture)
	, m_tNoisMaskEmisDslv(rhs.m_tNoisMaskEmisDslv)
{
	Safe_AddRef(m_pDiffuseTexture);
	Safe_AddRef(m_pNoiseTexture);
	Safe_AddRef(m_pMaskTexture);
	Safe_AddRef(m_pEmissiveTexture);
	Safe_AddRef(m_pDissolveTexture);
}

HRESULT CEffect::Initialize_Prototype(EFFECTDESC* pDesc)
{
	m_vPosition_Start = pDesc->vPosition_Start;
	m_vPosition_End = pDesc->vPosition_End;
	m_bPosition_Lerp = pDesc->bPosition_Lerp;

	m_vRotation_Start = pDesc->vRotation_Start;
	m_vRotation_End = pDesc->vRotation_End;
	m_bRotation_Lerp = pDesc->bRotation_Lerp;

	m_vScaling_Start = pDesc->vScaling_Start;
	m_vScaling_End = pDesc->vScaling_End;
	m_bScaling_Lerp = pDesc->bScaling_Lerp;

	m_vVelocity_Start = pDesc->vVelocity_Start;
	m_vVelocity_End = pDesc->vVelocity_End;
	m_bVelocity_Lerp = pDesc->bVelocity_Lerp;

	m_vColor_Start = pDesc->vColor_Start;
	m_vColor_End = pDesc->vColor_End;
	m_bColor_Lerp = pDesc->bColor_Lerp;

	m_fLifeTime = pDesc->fLifeTime;

	m_vUV_Start = pDesc->vUV_Start;
	m_vUV_Speed = pDesc->vUV_Speed;

	m_IsSequence = pDesc->IsSequence;
	m_fSequenceTerm = pDesc->fSequenceTerm;

	m_Variables.vUV_Offset = pDesc->vUV_Offset;
	m_Variables.vUV_Direction = pDesc->vUV_Direction;
	m_Variables.vUV_TileCount = pDesc->vUV_TileCount;
	m_Variables.vUV_TileIndex = pDesc->vUV_TileIndex;
	m_Variables.vColor_Offset = pDesc->vColor_Offset;

	m_Intensity.fBloom = pDesc->fBloom;
	m_Intensity.fRadial = pDesc->fRadial;

	// DiffuseTexture
	m_pDiffuseTexture = CTexture::Create(m_pDevice, m_pContext, pDesc->protoDiffuseTexture);
	if (nullptr == m_pDiffuseTexture) return E_FAIL;

	// NoiseTexture
	if (TEXT("") != pDesc->protoNoiseTexture)
	{
		m_pNoiseTexture = CTexture::Create(m_pDevice, m_pContext, pDesc->protoNoiseTexture);
		if (nullptr == m_pNoiseTexture) return E_FAIL;
		m_tNoisMaskEmisDslv.NoisMaskEmisDslv.x = 1.f;
	}
	// MaskTexture
	if (TEXT("") != pDesc->protoMaskTexture)
	{
		m_pMaskTexture = CTexture::Create(m_pDevice, m_pContext, pDesc->protoMaskTexture);
		if (nullptr == m_pMaskTexture) return E_FAIL;
		m_tNoisMaskEmisDslv.NoisMaskEmisDslv.y = 1.f;
	}
	// EmissiveTexture
	if (TEXT("") != pDesc->protoEmissiveTexture)
	{
		m_pEmissiveTexture = CTexture::Create(m_pDevice, m_pContext, pDesc->protoEmissiveTexture);
		if (nullptr == m_pEmissiveTexture) return E_FAIL;
		m_tNoisMaskEmisDslv.NoisMaskEmisDslv.z = 1.f;
	}
	// DissolveTexture
	if (TEXT("") != pDesc->protoDissolveTexture)
	{
		m_pDissolveTexture = CTexture::Create(m_pDevice, m_pContext, pDesc->protoDissolveTexture);
		if (nullptr == m_pDissolveTexture) return E_FAIL;
		m_tNoisMaskEmisDslv.NoisMaskEmisDslv.w = 1.f;
	}

	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect::Tick(_float fTimeDelta)
{
	if (m_fLifeTime < m_fTimeAcc)
	{
		Set_Active(false);
		Set_Dead(true);
		return;
	}

	m_fTimeAcc += fTimeDelta;
	m_fLifeTimeRatio = m_fTimeAcc / m_fLifeTime;
	
	Vec3 vOffsetScaling;
	Vec3 vOffsetRotation;
	Vec3 vOffsetPosition;
	Vec3 vVelocity;

	if (m_bScaling_Lerp)
		vOffsetScaling = Vec3::Lerp(m_vScaling_Start, m_vScaling_End, m_fLifeTimeRatio);
	else
		vOffsetScaling = m_vScaling_Start;

	if (m_bRotation_Lerp)
		vOffsetRotation = Vec3::Lerp(m_vRotation_Start, m_vRotation_End, m_fLifeTimeRatio);
	else
		vOffsetRotation = m_vRotation_Start;

	if (m_bPosition_Lerp)
		vOffsetPosition = Vec3::Lerp(m_vPosition_Start, m_vPosition_End, m_fLifeTimeRatio);
	else
		vOffsetPosition = m_vPosition_Start;

	if (m_bVelocity_Lerp)
		vVelocity = 0.5f * m_fTimeAcc * Vec3::Lerp(m_vVelocity_Start, m_vVelocity_End, m_fLifeTimeRatio);
	else
		vVelocity = 0.5f * m_fTimeAcc * m_vVelocity_Start;

	Quaternion vOffsetRotationQuaternion(Quaternion::CreateFromYawPitchRoll(vOffsetRotation.y, vOffsetRotation.x, vOffsetRotation.z));
	vOffsetPosition += vVelocity;
	 
	XMStoreFloat4x4(&m_matOffset, XMMatrixScaling(vOffsetScaling.x, vOffsetScaling.y, vOffsetScaling.z)
		* XMMatrixRotationQuaternion(vOffsetRotationQuaternion) * XMMatrixTranslation(vOffsetPosition.x, vOffsetPosition.y, vOffsetPosition.z));
}

void CEffect::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this)))
		__debugbreak();
}

HRESULT CEffect::Render()
{
	m_Variables.vUV_Offset.x += m_vUV_Speed.x * m_fTimeAcc * 0.001f;
	m_Variables.vUV_Offset.y += m_vUV_Speed.y * m_fTimeAcc * 0.001f;

	if (m_Variables.vUV_Offset.x > 1.f) m_Variables.vUV_Offset.x -= 1.f;
	if (m_Variables.vUV_Offset.y > 1.f) m_Variables.vUV_Offset.y -= 1.f;

	if(m_bColor_Lerp)
		m_Variables.vColor_Offset = Vec4::Lerp(m_vColor_Start, m_vColor_End, m_fLifeTimeRatio);
	else
		m_Variables.vColor_Offset = m_vColor_Start;

	if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Variables", &m_Variables, sizeof(tagFX_Variables))))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Intensity", &m_Intensity, sizeof(tagFX_Intensity))))
		return E_FAIL;*/

#pragma region GlobalData
	Matrix& matWorld = m_pTransformCom->Get_WorldMatrix();
	m_matCombined = m_matOffset * m_matPivot;

	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_matCombined, sizeof(Matrix))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Push_GlobalVP()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_CBuffer("EffectMaterialFlag", &m_tNoisMaskEmisDslv.NoisMaskEmisDslv, sizeof(EffectMaterialFlag))))
		return E_FAIL;

	if (m_pDiffuseTexture)
	{
		if (FAILED(m_pDiffuseTexture->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else
		__debugbreak();

	if (m_pNoiseTexture)
	{
		if (FAILED(m_pNoiseTexture->Set_SRV(m_pShaderCom, "g_NoiseTexture")))
			return E_FAIL;
	}
	if (m_pMaskTexture)
	{
		if (FAILED(m_pMaskTexture->Set_SRV(m_pShaderCom, "g_MaskTexture")))
			return E_FAIL;
	}
	if (m_pEmissiveTexture)
	{
		if (FAILED(m_pEmissiveTexture->Set_SRV(m_pShaderCom, "g_EmissiveTexture")))
			return E_FAIL;
	}
	if (m_pDissolveTexture)
	{
		if (FAILED(m_pDissolveTexture->Set_SRV(m_pShaderCom, "g_DissolveTexture")))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect::Ready_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	return S_OK;
}

void CEffect::Free()
{
	Super::Free();
	Safe_Release(m_pDiffuseTexture);
	Safe_Release(m_pNoiseTexture);
	Safe_Release(m_pMaskTexture);
	Safe_Release(m_pEmissiveTexture);
	Safe_Release(m_pDissolveTexture);
}
