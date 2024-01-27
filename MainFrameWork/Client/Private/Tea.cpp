#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Tea.h"

CTea::CTea(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject(pDevice, pContext, L"Tea", OBJ_TYPE::PART)
{

}

CTea::CTea(const CTea& rhs)
	: CPartObject(rhs)
{

}

HRESULT CTea::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CTea::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 부모 소켓행렬을 기준으로 자식의 상태를 제어한다.  */
	switch (m_ePart)
	{
	case CPartObject::PARTS::WEAPON_1:
		m_pTransformCom->Set_Scale(Vec3(1.5f, 1.5f, 1.5f));
		m_pTransformCom->My_Rotation(Vec3(0.f, 0.f, 172.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(-0.02f, -0.06f, 0.025f));
		break;
	case CPartObject::PARTS::WEAPON_2:
		m_pTransformCom->Set_Scale(Vec3(1.5f, 1.5f, 1.5f));
		m_pTransformCom->My_Rotation(Vec3(0.f, -35.f, 0.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(-0.03f, 0.01f, 0.02f));
		break;
	case CPartObject::PARTS::WEAPON_3:
		m_pTransformCom->Set_Scale(Vec3(1.5f, 1.5f, 1.5f));
		m_pTransformCom->My_Rotation(Vec3(0.f, 0.f, 0.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(-0.025f, 0.03f, -0.05f));
		break;
	case CPartObject::PARTS::WEAPON_4:
		m_pTransformCom->Set_Scale(Vec3(1.5f, 1.5f, 1.5f));
		m_pTransformCom->My_Rotation(Vec3(0.f, 100.f, 0.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(-0.02f, 0.01f, -0.01f));
		break;
	}

	return S_OK;
}

void CTea::Tick(_float fTimeDelta)
{
	if (false == Is_Render() || true == m_bStopUpdate)
		return;

	XMMATRIX	WorldMatrix;

	WorldMatrix = m_pParentModel->Get_CombinedMatrix(m_iSocketBoneIndex) * m_SocketPivotMatrix;

	WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
	WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
	WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix() * WorldMatrix);
}

void CTea::LateTick(_float fTimeDelta)
{
	if (true == Is_Render() && true == m_pOwner->Is_Render())
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_SHADOW, this);
	}
}

HRESULT CTea::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CTea::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

HRESULT CTea::Ready_Components()
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
	wstring strComName = L"Prototype_Component_Model_Tea";
	if (FAILED(__super::Add_Component(LEVEL_LOBBY, strComName,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTea::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_WorldMatrix, sizeof(Matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Push_GlobalVP()))
		return E_FAIL;

	return S_OK;
}

CTea* CTea::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTea* pInstance = new CTea(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTea");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTea::Clone(void* pArg)
{
	__super::Clone(pArg);

	CTea* pInstance = new CTea(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTea");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTea::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
