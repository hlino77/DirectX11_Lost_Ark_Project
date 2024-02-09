#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "MannequinPart.h"

CMannequinPart::CMannequinPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject(pDevice, pContext, L"Mannequin_Part", OBJ_TYPE::PART)
{

}

CMannequinPart::CMannequinPart(const CMannequinPart& rhs)
	: CPartObject(rhs)
{

}

HRESULT CMannequinPart::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CMannequinPart::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom = nullptr;

	return S_OK;
}

void CMannequinPart::Tick(_float fTimeDelta)
{
	if (nullptr == m_pModelCom)
		return;

	if (true == m_IsAnim)
	{
		m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
	}
	
	XMMATRIX	WorldMatrix = m_pParentModel->Get_CombinedMatrix(m_iSocketBoneIndex) * m_SocketPivotMatrix;

	WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
	WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
	WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix() * WorldMatrix);
}

void CMannequinPart::LateTick(_float fTimeDelta)
{
	if (true == m_IsAnim)
	{
		if (m_PlayAnimation.valid())
		{
			m_PlayAnimation.get();
		}
	}
	if (nullptr != m_pModelCom)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_SHADOW, this);
	}
}

HRESULT CMannequinPart::Render()
{
	if (false == m_IsAnim)
	{
		if (FAILED(Render_Static()))
			return E_FAIL;
	}
	else
	{
		if (FAILED(Render_Anim()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMannequinPart::Render_Static()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMannequinPart::Render_Anim()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUpAnimation_OnShader(m_pAnimShaderCom)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pAnimShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMannequinPart::Render_ShadowDepth()
{
	if (false == m_IsAnim)
	{
		__super::Render_ShadowDepth();
	}
	else
	{
		if (FAILED(m_pAnimShaderCom->Bind_CBuffer("TransformBuffer", &m_WorldMatrix, sizeof(Matrix))))
			return E_FAIL;

		if (FAILED(m_pAnimShaderCom->Push_ShadowVP()))
			return S_OK;

		_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelCom->Render(m_pAnimShaderCom, i, "ShadowPass")))
				return S_OK;
		}
	}

	return S_OK;
}

HRESULT CMannequinPart::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_Shader_Anim"), (CComponent**)&m_pAnimShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMannequinPart::Bind_ShaderResources()
{
	if (false == m_IsAnim)
	{
		if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_WorldMatrix, sizeof(Matrix))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Push_GlobalVP()))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pAnimShaderCom->Bind_CBuffer("TransformBuffer", &m_WorldMatrix, sizeof(Matrix))))
			return E_FAIL;
		if (FAILED(m_pAnimShaderCom->Push_GlobalVP()))
			return E_FAIL;
	}

	return S_OK;
}

CMannequinPart* CMannequinPart::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMannequinPart* pInstance = new CMannequinPart(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMannequinPart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMannequinPart::Clone(void* pArg)
{
	__super::Clone(pArg);

	CMannequinPart* pInstance = new CMannequinPart(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMannequinPart");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMannequinPart::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pAnimShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
