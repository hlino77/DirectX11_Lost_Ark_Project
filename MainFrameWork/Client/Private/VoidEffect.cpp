#include "stdafx.h"
#include "GameInstance.h"
#include "VoidEffect.h"
#include "Camera_Player.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "BindShaderDesc.h"
#include "VIBuffer_Point.h"
#include "BindShaderDesc.h"

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

	if (FAILED(Ready_Components(pDesc)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(0.f, 0.f, 0.f));

    return S_OK;
}

void CVoidEffect::Tick(_float fTimeDelta)
{
}

void CVoidEffect::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	if(FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this)))
		__debugbreak();
}

HRESULT CVoidEffect::Render()
{
	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
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

	if (m_pModelCom)
	{
		_int iMeshCount = m_pModelCom->Get_Meshes().size();
		for (_int i = 0; i < iMeshCount; ++i)
			m_pModelCom->Render(m_pShaderCom, i, "Default");
	}
	else
	{
		m_pShaderCom->Begin("Default");
		m_pBuffer->Render();
	}

	return S_OK;
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

	if (TEXT("") != pDesc->protoModel)
	{
		/* For.Com_Model */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, pDesc->protoModel, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
			return E_FAIL;

		/* For.Com_Shader */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectMesh"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
			return E_FAIL;
	}
	else
	{
		/* For.Com_Model */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point"), TEXT("Com_VIBuffer"), (CComponent**)&m_pBuffer)))
			return E_FAIL;

		/* For.Com_Shader */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
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
}
