#include "stdafx.h"
#include "GameInstance.h"
#include "VoidEffect.h"
#include "Camera_Player.h"
#include "PartObject.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "BindShaderDesc.h"
#include "VIBuffer_Point.h"
#include "VIBuffer_Particle.h"
#include "VIBuffer_Cube.h"
#include "BindShaderDesc.h"
#include "Utils.h"
#include "Level_Tool.h"
//#include "DebugDraw.h"

CVoidEffect::CVoidEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, TEXT("VoidEffect"), -1)
{
}

CVoidEffect::CVoidEffect(const CVoidEffect& rhs)
	: Super(rhs)
{
}

HRESULT CVoidEffect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CVoidEffect::Initialize(void* pArg)
{
	tagVoidEffectDesc* pDesc = reinterpret_cast<tagVoidEffectDesc*>(pArg);
	m_iEffectType = pDesc->iEffectType;
	m_tVoidEffectDesc = *pDesc;

	if (FAILED(Ready_Components(pDesc)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(0.f, 0.f, 0.f));

	////////// Decal //////////

	if (3 == m_iEffectType)
	{
		/*m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);

		m_pEffect = new BasicEffect(m_pDevice);
		m_pEffect->SetVertexColorEnabled(true);

		const void* pShaderByteCodes = nullptr;
		size_t		iLength = 0;
		m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

		if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		{
			Safe_Delete(m_pBatch);
			Safe_Delete(m_pEffect);
			Safe_Release(m_pInputLayout);
			return E_FAIL;
		}*/
		m_vScaling_Start.y = 50.f;
	}

	////////// Decal //////////

    return S_OK;
}

void CVoidEffect::Tick(_float fTimeDelta)
{
	m_fLifeTime = ::max(0.001f, m_fLifeTime);

	if (m_fWaitingTime > 0.0f)
	{
		m_fWaitingAcc += fTimeDelta;
		if (m_fWaitingAcc >= m_fWaitingTime)
			m_bRender = true;
		else
			return;
	}

	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc >= m_fLifeTime + m_fRemainTime)
		Reset();
	

	m_fLifeTimeRatio = min(1.0f, m_fTimeAcc / m_fLifeTime);

	if (m_IsSequence)
	{
		m_Variables.vUV_TileCount.x = ::max(1.f, m_Variables.vUV_TileCount.x);
		m_Variables.vUV_TileCount.y = ::max(1.f, m_Variables.vUV_TileCount.y);
		m_fSequenceTerm = ::max(0.001f, m_fSequenceTerm);

		m_fSequenceTimer += fTimeDelta;

		while (m_fSequenceTimer >= m_fSequenceTerm)
		{
			m_fSequenceTimer -= m_fSequenceTerm;
			++m_Variables.vUV_TileIndex.x;

			if (m_Variables.vUV_TileIndex.x >= m_Variables.vUV_TileCount.x)
			{
				m_Variables.vUV_TileIndex.x = 0.0f;
				++m_Variables.vUV_TileIndex.y;
				if (m_Variables.vUV_TileIndex.y >= m_Variables.vUV_TileCount.y)
				{
					if (m_IsLoop)
						m_Variables.vUV_TileIndex.y = 0.0f;	
					else
						m_bRender = false;
				}
					
			}
		}
	}

	m_Particle.fGameTime = m_fTimeAcc;
	m_Particle.fTimeStep = fTimeDelta;

	Vec3 vOffsetScaling = Vec3::Lerp(m_vScaling_Start, m_vScaling_End, m_fLifeTimeRatio);
	Vec4 vOffsetRotation = Vec3::Lerp(m_vRotation_Start, m_vRotation_End, m_fLifeTimeRatio);
	Vec3 vOffsetPosition = Vec3::Lerp(m_vPosition_Start, m_vPosition_End, m_fLifeTimeRatio) + 0.5f * m_fLifeTimeRatio * Vec3::Lerp(m_vVelocity_Start, m_vVelocity_End, m_fLifeTimeRatio);

	XMStoreFloat4x4(&m_matOffset, XMMatrixScaling(vOffsetScaling.x, vOffsetScaling.y, vOffsetScaling.z)
		* XMMatrixRotationRollPitchYaw(XMConvertToRadians(vOffsetRotation.x), XMConvertToRadians(vOffsetRotation.y), XMConvertToRadians(vOffsetRotation.z))
		* XMMatrixTranslation(vOffsetPosition.x, vOffsetPosition.y, vOffsetPosition.z));
}

void CVoidEffect::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	if (m_bRender)
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this)))
			__debugbreak();
	}
}

HRESULT CVoidEffect::Render()
{
	m_Variables.vUV_Offset.x = m_vUV_Speed.x * m_fTimeAcc;
	m_Variables.vUV_Offset.y = m_vUV_Speed.y * m_fTimeAcc;

	if (m_Variables.vUV_Offset.x > 1.f) m_Variables.vUV_Offset.x -= 1.f;
	if (m_Variables.vUV_Offset.y > 1.f) m_Variables.vUV_Offset.y -= 1.f;

	m_Variables.vColor_Offset = Vec4::Lerp(m_vColor_Start, m_vColor_End, m_fLifeTimeRatio);

	if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Variables", &m_Variables, sizeof(tagFX_Variables))))
		return E_FAIL;

	//////////////////////////////

#pragma region GlobalData
	Matrix matCombined = m_matOffset * m_matPivot;

	m_Particle.vEmitPosition = matCombined.Translation();

	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &matCombined, sizeof(Matrix))))
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
		if (m_fLifeTimeRatio >= m_fDissolveStart)
		{
			m_Intensity.fDissolveAmount = (m_fLifeTimeRatio - m_fDissolveStart) / (1.f - m_fDissolveStart);

			if (FAILED(m_pDissolveTexture->Set_SRV(m_pShaderCom, "g_DissolveTexture")))
				return E_FAIL;
		}
	}

	if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Intensity", &m_Intensity, sizeof(tagFX_Intensity))))
		return E_FAIL;

	if (0 == m_iEffectType)
	{
		_int iMeshCount = m_pModelCom->Get_Meshes().size();
		for (_int i = 0; i < iMeshCount; ++i)
		{
			if (FAILED(m_pModelCom->Render(m_pShaderCom, i, "Default")))
				return E_FAIL;
		}
	}
	else if (1 == m_iEffectType)
	{
		if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Billboard", &m_Billboard, sizeof(tagFX_Billboard))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Begin("Default")))
			return E_FAIL;
		if (FAILED(static_cast<CVIBuffer_Point*>(m_pBuffer)->Render()))
			return E_FAIL;
	}
	else if (2 == m_iEffectType)
	{
		if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Billboard", &m_Billboard, sizeof(tagFX_Billboard))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_CBuffer("FX_Particle", &m_Particle, sizeof(tagFX_Particle))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_Texture("g_RandomTexture", m_pRandomTextureSRV)))
			return E_FAIL;
		if (FAILED(static_cast<CVIBuffer_Particle*>(m_pBuffer)->Render("Smoke")))
			return E_FAIL;
	}
	else if (3 == m_iEffectType)
	{
		Matrix matCombinedInv = matCombined.Invert();

		if (FAILED(m_pShaderCom->Bind_CBuffer("TransformInverse", &matCombinedInv, sizeof(Matrix))))
			return E_FAIL;
		
		if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ))) ||
			FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Bind_SRV(m_pShaderCom, TEXT("Target_NormalDepth"), "g_NormalDepthTarget")) ||
			FAILED(m_pGameInstance->Bind_SRV(m_pShaderCom, TEXT("Target_Normal"), "g_NormalTarget")) ||
			FAILED(m_pGameInstance->Bind_SRV(m_pShaderCom, TEXT("Target_Properties"), "g_PropertiesTarget")))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin("Default")))
			return E_FAIL;
		if (FAILED(static_cast<CVIBuffer_Cube*>(m_pBuffer)->Render()))
			return E_FAIL;

		////////// Bounding Box //////////

		/*m_pEffect->SetWorld(XMMatrixIdentity());

		m_pEffect->SetView(m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW));
		m_pEffect->SetProjection(m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ));

		m_pEffect->Apply(m_pContext);

		m_pContext->IASetInputLayout(m_pInputLayout);

		m_tBoundingBox.Center = matCombined.Translation();

		Vec3 vRight = matCombined.Right();
		Vec3 vUp = matCombined.Up();
		Vec3 vLook = matCombined.Backward();

		m_tBoundingBox.Extents.x = vRight.Length();
		m_tBoundingBox.Extents.y = vUp.Length();
		m_tBoundingBox.Extents.z = vLook.Length();

		Quaternion vQuat;
		matCombined.Decompose(Vec3(), vQuat, Vec3());

		m_tBoundingBox.Orientation = vQuat;

		m_pBatch->Begin();

		DX::Draw(m_pBatch, m_tBoundingBox, Vec4(1.f, 0.f, 1.f, 1.f));

		m_pBatch->End();*/
	}
#pragma endregion

	return S_OK;
}

HRESULT CVoidEffect::Render_Particle()
{


	return S_OK;
}

void CVoidEffect::Reset()
{
	m_fTimeAcc = 0.0f;
	m_Variables.vUV_TileIndex = Vec2(0.0f, 0.0f);
	m_fSequenceTimer = 0.0f;
	m_bRender = true;
	
	if (m_fWaitingTime > 0.0f)
	{
		m_bRender = false;
		m_fWaitingAcc = 0.0f;
	}

	if (nullptr != m_pEffectTool->GetLevelTool()->GetPivotObject())
	{
		const _char* szTypeID = typeid(*m_pEffectTool->GetLevelTool()->GetPivotObject()).name();

		if (!strcmp(szTypeID, "class Client::CMannequin"))
		{
			m_matPivot = m_pEffectTool->GetLevelTool()->GetPivotObject()->Get_TransformCom()->Get_WorldMatrix();
			m_bParentPivot = true;
		}
		else
		{
			m_matPivot = static_cast<CPartObject*>(m_pEffectTool->GetLevelTool()->GetPivotObject())->Get_Part_WorldMatrix();
			m_bParentPivot = false;
		}

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
	else
		m_matPivot = Matrix::Identity;
}

HRESULT CVoidEffect::Ready_Components(tagVoidEffectDesc* pDesc)
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

	//if (TEXT("") != pDesc->protoModel)
	if (0 == m_iEffectType)
	{
		/* For.Com_Model */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_") + pDesc->protoModel, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
			return E_FAIL;

		/* For.Com_Shader */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectMesh"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
			return E_FAIL;
	}
	else if(1 == m_iEffectType)
	{
		/* For.Com_Buffer */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point"), TEXT("Com_VIBuffer"), (CComponent**)&m_pBuffer)))
			return E_FAIL;

		/* For.Com_Shader */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
			return E_FAIL;
	}
	else if(2 == m_iEffectType)
	{
		/* For.Com_Buffer */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle"), TEXT("Com_VIBuffer"), (CComponent**)&m_pBuffer)))
			return E_FAIL;

		/* For.Com_Shader */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_ParticleSystem"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
			return E_FAIL;

		CUtils* pUtils = GET_INSTANCE(CUtils);
		pUtils->CreateRandomTexture1DSRV(m_pDevice, &m_pRandomTextureSRV);
		RELEASE_INSTANCE(CUtils);
	}
	else if(3 == m_iEffectType)
	{
		/* For.Com_Buffer */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_VIBuffer"), (CComponent**)&m_pBuffer)))
			return E_FAIL;

		/* For.Com_Shader */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectDecal"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
			return E_FAIL;
	}

	//////////

	// BaseTexture
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

CVoidEffect* CVoidEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVoidEffect* pInstance = new CVoidEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CVoidEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVoidEffect::Clone(void* pArg)
{
	CVoidEffect* pInstance = new CVoidEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVoidEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVoidEffect::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	if (3 == m_iEffectType)
	{
		/*Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
		Safe_Delete(m_pInputLayout);*/
	}
}
