#include "stdafx.h"
#include "Effect.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "RigidBody.h"
#include "Effect_Manager.h"
#include "PartObject.h"

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
	, m_vRevolution_Start(rhs.m_vRevolution_Start)
	, m_vRevolution_End(rhs.m_vRevolution_End)
	, m_bRevolution_Lerp(rhs.m_bRevolution_Lerp)
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
	, m_fDissolveStart(rhs.m_fDissolveStart)
	, m_Variables(rhs.m_Variables)
	, m_Intensity(rhs.m_Intensity)
	, m_pDiffuseTexture(rhs.m_pDiffuseTexture)
	, m_pNoiseTexture(rhs.m_pNoiseTexture)
	, m_pMaskTexture(rhs.m_pMaskTexture)
	, m_pEmissiveTexture(rhs.m_pEmissiveTexture)
	, m_pDissolveTexture(rhs.m_pDissolveTexture)
	, m_tNoisMaskEmisDslv(rhs.m_tNoisMaskEmisDslv)
	, m_fWaitingTime(rhs.m_fWaitingTime)
	, m_fRemainTime(rhs.m_fRemainTime)
	, m_IsLoop(rhs.m_IsLoop)
	, m_strPassName(rhs.m_strPassName)
	, m_fRadialTime(rhs.m_fRadialTime)
	, m_fRadialIntensity(rhs.m_fRadialIntensity)
	, m_bPosition_Pass(rhs.m_bPosition_Pass)
	, m_bRotation_Pass(rhs.m_bRotation_Pass)
	, m_bRevolution_Pass(rhs.m_bRevolution_Pass)
	, m_bScaling_Pass(rhs.m_bScaling_Pass)
	, m_bVelocity_Pass(rhs.m_bVelocity_Pass)
{
	m_szModelName = rhs.m_szModelName;

	Safe_AddRef(m_pDiffuseTexture);
	Safe_AddRef(m_pNoiseTexture);
	Safe_AddRef(m_pMaskTexture);
	Safe_AddRef(m_pEmissiveTexture);
	Safe_AddRef(m_pDissolveTexture);
}

HRESULT CEffect::Initialize_Prototype(EFFECTDESC* pDesc)
{
	m_szModelName = pDesc->EffectTag;

	m_vPosition_Start = pDesc->vPosition_Start;
	m_vPosition_End = pDesc->vPosition_End;
	m_bPosition_Lerp = pDesc->bPosition_Lerp;
	if (!m_bPosition_Lerp)
	{
		if (XMVector3Equal(m_vPosition_Start, Vec3::Zero))
			m_bPosition_Pass = true;
	}

	m_vRotation_Start = pDesc->vRotation_Start;
	m_vRotation_End = pDesc->vRotation_End;
	m_bRotation_Lerp = pDesc->bRotation_Lerp;
	if (!m_bRotation_Lerp)
	{
		if (XMVector3Equal(m_vRotation_Start, Vec3::Zero))
			m_bRotation_Pass = true;
	}

	m_vRevolution_Start = pDesc->vRevolution_Start;
	m_vRevolution_End = pDesc->vRevolution_End;
	m_bRevolution_Lerp = pDesc->bRevolution_Lerp;
	if (!m_bRevolution_Lerp)
	{
		if (XMVector3Equal(m_vRevolution_Start, Vec3::Zero))
			m_bRevolution_Pass = true;
	}

	m_vScaling_Start = pDesc->vScaling_Start;
	m_vScaling_End = pDesc->vScaling_End;
	m_bScaling_Lerp = pDesc->bScaling_Lerp;
	if (!m_bScaling_Lerp)
	{
		if (XMVector3Equal(m_vScaling_Start, Vec3::Zero))
			m_bScaling_Pass = true;
	}

	m_vVelocity_Start = pDesc->vVelocity_Start;
	m_vVelocity_End = pDesc->vVelocity_End;
	m_bVelocity_Lerp = pDesc->bVelocity_Lerp;
	if (!m_bVelocity_Lerp)
	{
		if (XMVector3Equal(m_vVelocity_Start, Vec3::Zero))
			m_bVelocity_Pass = true;
	}

	m_vColor_Start = pDesc->vColor_Start;
	m_vColor_End = pDesc->vColor_End;
	m_bColor_Lerp = pDesc->bColor_Lerp;

	m_fDissolveStart = pDesc->fDissolveStart;

	m_fLifeTime = pDesc->fLifeTime;
	m_fWaitingTime = pDesc->fWaitingTime;
	m_fRemainTime = pDesc->fRemainTime;

	m_vUV_Start = pDesc->vUV_Start;
	m_vUV_Speed = pDesc->vUV_Speed;

	m_IsSequence = pDesc->IsSequence;
	m_IsLoop = pDesc->IsLoop;
	m_fSequenceTerm = pDesc->fSequenceTerm;

	m_Variables.vUV_Offset = pDesc->vUV_Offset;
	m_Variables.iUV_Wave = pDesc->iUV_Wave;
	m_Variables.fUV_WaveSpeed = pDesc->fUV_WaveSpeed;
	m_Variables.vUV_TileCount = pDesc->vUV_TileCount;
	m_Variables.vColor_Offset = pDesc->vColor_Offset;
	m_Variables.vColor_Clip = pDesc->vColor_Clip;
	m_Variables.vColor_Mul = pDesc->vColor_Mul;

	m_Intensity.fBloom = pDesc->fBloom;
	m_Intensity.fDistortion = pDesc->fDistortion;
	m_Intensity.fDissolveAmount = pDesc->fDissolveAmount;
	
	m_fRadialIntensity = pDesc->fRadialIntensity;
	m_fRadialTime = pDesc->fRadialTime;

	m_fRadialTime = pDesc->fRadialTime;
	m_fRadialIntensity = pDesc->fRadialIntensity;

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

	m_strPassName = pDesc->strPassName;

	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	Set_Active(false);

	return S_OK;
}

void CEffect::Tick(_float fTimeDelta)
{
	if (m_fWaitingAcc < m_fWaitingTime)
	{
		m_fWaitingAcc += fTimeDelta;
		if (m_fWaitingAcc >= m_fWaitingTime)
			m_bRender = true;
		else
			return;
	}

	if (m_fTimeAcc >= m_fLifeTime + m_fRemainTime)
	{
		EffectEnd();
		return;
	}

	m_fTimeAcc += fTimeDelta;
	m_fLifeTimeRatio = min(1.0f, m_fTimeAcc / m_fLifeTime);

	Matrix matOffset(Matrix::Identity);

	if (!m_bScaling_Pass)
	{
		if (m_bScaling_Lerp)
			m_vOffsetScaling = Vec3::Lerp(m_vScaling_Start, m_vScaling_End, m_fLifeTimeRatio);
		else
			m_vOffsetScaling = m_vScaling_Start;

		m_matOffset = XMMatrixScaling(m_vOffsetScaling.x, m_vOffsetScaling.y, m_vOffsetScaling.z);
	}
	else
	{
		m_matOffset = Matrix::Identity;		
	}

	if (!m_bRotation_Pass)
	{
		if (m_bRotation_Lerp)
			m_vOffsetRotation = Vec3::Lerp(m_vRotation_Start, m_vRotation_End, m_fLifeTimeRatio);
		else
			m_vOffsetRotation = m_vRotation_Start;

		Quaternion qRotation;
		XMStoreFloat4(&qRotation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(m_vOffsetRotation.x), XMConvertToRadians(m_vOffsetRotation.y), XMConvertToRadians(m_vOffsetRotation.z)));
		Matrix::Transform(m_matOffset, qRotation, m_matOffset);
	}

	if (!m_bPosition_Pass)
	{
		if (m_bPosition_Lerp)
			m_vOffsetPosition = Vec3::Lerp(m_vPosition_Start, m_vPosition_End, m_fLifeTimeRatio);
		else
			m_vOffsetPosition = m_vPosition_Start;
	}

	if (!m_bVelocity_Pass)
	{
		if (m_bVelocity_Lerp)
			m_vVelocity = 0.5f * m_fLifeTimeRatio * Vec3::Lerp(m_vVelocity_Start, m_vVelocity_End, m_fLifeTimeRatio);
		else
			m_vVelocity = 0.5f * m_fLifeTimeRatio * m_vVelocity_Start;
	}

	if (!m_bPosition_Pass)
	{
		if (!m_bVelocity_Pass)
			m_vOffsetPosition += m_vVelocity;

		m_matOffset._41 += m_vOffsetPosition.x;
		m_matOffset._42 += m_vOffsetPosition.y;
		m_matOffset._43 += m_vOffsetPosition.z;
	}
	else
	{
		if (!m_bVelocity_Pass)
			m_vOffsetPosition = m_vVelocity;

		m_matOffset._41 += m_vOffsetPosition.x;
		m_matOffset._42 += m_vOffsetPosition.y;
		m_matOffset._43 += m_vOffsetPosition.z;
	}

	if (!m_bRevolution_Pass)
	{
		if (m_fTimeAcc >= m_fLifeTime && m_fTimeAcc <= m_fLifeTime + m_fRemainTime)
			m_fLifeTimeRatio = (fmodf(m_fTimeAcc, m_fLifeTime) / m_fLifeTime);

		if (m_bRevolution_Lerp)
			m_vOffsetRevolution = Vec3::Lerp(m_vRevolution_Start, m_vRevolution_End, m_fLifeTimeRatio);
		else
			m_vOffsetRevolution = m_vRevolution_Start;

		Quaternion qRotation;
		XMStoreFloat4(&qRotation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(m_vOffsetRevolution.x), XMConvertToRadians(m_vOffsetRevolution.y), XMConvertToRadians(m_vOffsetRevolution.z)));
		Matrix::Transform(m_matOffset, qRotation, m_matOffset);
	}
}

void CEffect::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	if (m_bRender)
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this)))
			__debugbreak();
	}

	m_matCombined = m_matOffset * m_matPivot;
}

HRESULT CEffect::Render()
{
	m_Variables.vUV_Offset.x = m_vUV_Speed.x * m_fTimeAcc;
	m_Variables.vUV_Offset.y = m_vUV_Speed.y * m_fTimeAcc;

	if (m_Variables.vUV_Offset.x > 1.f) m_Variables.vUV_Offset.x -= 1.f;
	if (m_Variables.vUV_Offset.y > 1.f) m_Variables.vUV_Offset.y -= 1.f;

	if(m_bColor_Lerp)
		m_Variables.vColor_Offset = Vec4::Lerp(m_vColor_Start, m_vColor_End, m_fLifeTimeRatio);
	else
		m_Variables.vColor_Offset = m_vColor_Start;

	if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Variables", &m_Variables, sizeof(tagFX_Variables))))
		return E_FAIL;

#pragma region GlobalData

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
		_float fRatio = m_fTimeAcc / (m_fLifeTime + m_fRemainTime);
		if (fRatio >= m_fDissolveStart)
		{
			m_Intensity.fDissolveAmount = (fRatio - m_fDissolveStart) / (1.f - m_fDissolveStart);

			if (FAILED(m_pDissolveTexture->Set_SRV(m_pShaderCom, "g_DissolveTexture")))
				return E_FAIL;
		}	
		else
			m_Intensity.fDissolveAmount = 0.f;
	}

	if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Intensity", &m_Intensity, sizeof(tagFX_Intensity))))
		return E_FAIL;

	if (FLT_EPSILON < m_fRadialTime)
	{
		if (m_fTimeAcc < m_fRadialTime)
			m_pRendererCom->Set_RadialBlurData(m_matCombined.Translation(), m_fRadialIntensity);
	}

	return S_OK;
}

void CEffect::Reset(CEffect_Manager::EFFECTPIVOTDESC& tEffectDesc)
{
	if (tEffectDesc.pPivotTransform)
	{
		if (tEffectDesc.bParentPivot)
			m_matPivot = static_cast<CPartObject*>(tEffectDesc.pPivotTransform->Get_GameObject())->Get_PartOwner()->Get_TransformCom()->Get_WorldMatrix();
		else
			m_matPivot = static_cast<CPartObject*>(tEffectDesc.pPivotTransform->Get_GameObject())->Get_Part_WorldMatrix();
	}
	else
		m_matPivot = *tEffectDesc.pPivotMatrix;

	Vec3 vRight = m_matPivot.Right();
	vRight.Normalize();
	m_matPivot.Right(vRight);

	Vec3 vUp = m_matPivot.Up();
	vUp.Normalize();
	m_matPivot.Up(vUp);

	Vec3 vLook = m_matPivot.Backward();
	vLook.Normalize();
	m_matPivot.Backward(vLook);

	//Reset
	m_fSequenceTimer = 0.0f;
	m_Variables.vUV_TileIndex = Vec2(0.0f, 0.0f);
	m_fTimeAcc = 0.0f;
	m_bRender = true;

	if (m_fWaitingTime > 0.0f)
	{
		m_fWaitingAcc = 0.0f;
		m_bRender = false;
	}
}

void CEffect::EffectEnd()
{
	Set_Active(false);
	CEffect_Manager::GetInstance()->Return_Effect(this);
}

void CEffect::Update_Pivot(Matrix& matPivot)
{
	m_matPivot = matPivot;

	Vec3 vRight = m_matPivot.Right();
	vRight.Normalize();
	m_matPivot.Right(vRight);

	Vec3 vUp = m_matPivot.Up();
	vUp.Normalize();
	m_matPivot.Up(vUp);

	Vec3 vLook = m_matPivot.Backward();
	vLook.Normalize();
	m_matPivot.Backward(vLook);
}


HRESULT CEffect::Ready_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
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
