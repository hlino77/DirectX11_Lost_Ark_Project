#include "stdafx.h"
#include "GameInstance.h"
#include "VoidEffect.h"
#include "Camera_Player.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "BindShaderDesc.h"
#include "VIBuffer_Point.h"

CVoidEffect::CVoidEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, TEXT("void"), -1)
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
	if (FAILED(m_pDiffuseTexture->Set_SRV(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pNoiseTexture->Set_SRV(m_pShaderCom, "g_NoiseTexture")))
		return E_FAIL;
	if (FAILED(m_pMaskTexture->Set_SRV(m_pShaderCom, "g_MaskTexture")))
		return E_FAIL;
	if (FAILED(m_pEmissiveTexture->Set_SRV(m_pShaderCom, "g_EmissiveTexture")))
		return E_FAIL;
	if (FAILED(m_pDissolveTexture->Set_SRV(m_pShaderCom, "g_DissolveTexture")))
		return E_FAIL;

	_int iMeshCount = m_pModelCom->Get_Meshes().size();

	if (m_pModelCom)
	{
		m_pModelCom->Render(m_pShaderCom, iMeshCount, "Default");
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
		if (FAILED(__super::Add_Component(LEVEL_STATIC, pDesc->protoModel, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectMesh"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point"), TEXT("Com_VIBuffer"), (CComponent**)&m_pBuffer)))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectTex"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
			return E_FAIL;
	}

	// DiffuseTexture
	if (FAILED(__super::Add_Component(LEVEL_STATIC, pDesc->protoDiffuseTexture, TEXT("Com_Texture"), (CComponent**)&m_pDiffuseTexture)))
		return E_FAIL;

	// NoiseTexture
	if (FAILED(__super::Add_Component(LEVEL_STATIC, pDesc->protoNoiseTexture, TEXT("Com_Texture"), (CComponent**)&m_pNoiseTexture)))
		return E_FAIL;

	// MaskTexture
	if (FAILED(__super::Add_Component(LEVEL_STATIC, pDesc->protoMaskTexture, TEXT("Com_Texture"), (CComponent**)&m_pMaskTexture)))
		return E_FAIL;

	// EmissiveTexture
	if (FAILED(__super::Add_Component(LEVEL_STATIC, pDesc->protoEmissiveTexture, TEXT("Com_Texture"), (CComponent**)&m_pEmissiveTexture)))
		return E_FAIL;

	// DissolveTexture
	if (FAILED(__super::Add_Component(LEVEL_STATIC, pDesc->protoDissolveTexture, TEXT("Com_Texture"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

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
