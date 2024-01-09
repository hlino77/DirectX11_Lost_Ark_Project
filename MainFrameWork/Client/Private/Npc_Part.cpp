#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Npc_Part.h"

CNpc_Part::CNpc_Part(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject(pDevice, pContext, L"Npc_Part", OBJ_TYPE::PART)
{

}

CNpc_Part::CNpc_Part(const CNpc_Part& rhs)
	: CPartObject(rhs)
{

}

HRESULT CNpc_Part::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CNpc_Part::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 부모 소켓행렬을 기준으로 자식의 상태를 제어한다.  */
	m_pTransformCom->Set_Scale(m_vPartScale);
	m_pTransformCom->My_Rotation(m_vPartRot);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vPartPos);

	return S_OK;
}

void CNpc_Part::Tick(_float fTimeDelta)
{
	if (false == Is_Render() || true == m_bStopUpdate)
		return;

	XMMATRIX	WorldMatrix;

	if (false == m_IsStored)
	{
		WorldMatrix = m_pParentModel->Get_CombinedMatrix(m_iSocketBoneIndex) * m_SocketPivotMatrix;
	}
	else if (true == m_IsStored)
	{
		WorldMatrix = m_pParentModel->Get_CombinedMatrix(m_iStoreSocketBoneIndex) * m_SocketPivotMatrix;
	}

	WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
	WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
	WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix() * WorldMatrix);
}

void CNpc_Part::LateTick(_float fTimeDelta)
{
	if (true == Is_Render() && true == m_pOwner->Is_Render())
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_SHADOW, this);
	}
}

HRESULT CNpc_Part::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CNpc_Part::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

HRESULT CNpc_Part::Ready_Components()
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

	///* For.Com_Model */
	wstring strComName = TEXT("Prototype_Component_Model_") + m_strPartModel;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	m_vOriginScale.x = 100.f;
	m_vOriginScale.y = 100.f;
	m_vOriginScale.z = 100.f;

	m_pTransformCom->Set_Scale(m_vOriginScale);

	return S_OK;
}

HRESULT CNpc_Part::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_WorldMatrix, sizeof(Matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Push_GlobalVP()))
		return E_FAIL;

	return S_OK;
}

CNpc_Part* CNpc_Part::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNpc_Part* pInstance = new CNpc_Part(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CNpc_Part");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNpc_Part::Clone(void* pArg)
{
	__super::Clone(pArg);

	CNpc_Part* pInstance = new CNpc_Part(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNpc_Part");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpc_Part::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
