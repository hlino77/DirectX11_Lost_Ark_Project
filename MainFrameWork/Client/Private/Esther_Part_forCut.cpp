#include "stdafx.h"
#include "Client_Defines.h"
#include "Engine_Defines.h"
#include "GameInstance.h"
#include "Esther_Part_forCut.h"
#include "Esther_Cut.h"
#include "Camera_Cut.h"

CEsther_Part_forCut::CEsther_Part_forCut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject(pDevice, pContext, L"Esther_Part", OBJ_TYPE::PART)
{

}

CEsther_Part_forCut::CEsther_Part_forCut(const CEsther_Part_forCut& rhs)
	: CPartObject(rhs)
{

}

HRESULT CEsther_Part_forCut::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther_Part_forCut::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 부모 소켓행렬을 기준으로 자식의 상태를 제어한다.  */
	m_pTransformCom->Set_WorldMatrix(m_OffSetMatrix);

	return S_OK;
}

void CEsther_Part_forCut::Tick(_float fTimeDelta)
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

void CEsther_Part_forCut::LateTick(_float fTimeDelta)
{
	if (true == Is_Render() && true == m_pOwner->Is_Render())
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_SHADOW, this);
	}
}

HRESULT CEsther_Part_forCut::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEsther_Part_forCut::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
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

	return S_OK;
}

HRESULT CEsther_Part_forCut::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_WorldMatrix, sizeof(Matrix))))
		return E_FAIL;

	GlobalDesc gDesc = {
		static_cast<CEsther_Cut*>(m_pOwner)->Get_CutCamera()->Get_TransformCom()->Get_WorldMatrixInverse(),
		static_cast<CEsther_Cut*>(m_pOwner)->Get_CutCamera()->Get_ProjMatrix(),
		static_cast<CEsther_Cut*>(m_pOwner)->Get_CutCamera()->Get_TransformCom()->Get_WorldMatrixInverse() * static_cast<CEsther_Cut*>(m_pOwner)->Get_CutCamera()->Get_ProjMatrix(),
		static_cast<CEsther_Cut*>(m_pOwner)->Get_CutCamera()->Get_TransformCom()->Get_WorldMatrix()
	};

	if (FAILED(m_pShaderCom->Bind_CBuffer("GlobalBuffer", &gDesc, sizeof(GlobalDesc))))
		return E_FAIL;

	return S_OK;
}

void CEsther_Part_forCut::Set_EffectPos()
{
}

CEsther_Part_forCut* CEsther_Part_forCut::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEsther_Part_forCut* pInstance = new CEsther_Part_forCut(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEsther_Part_forCut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Part_forCut::Clone(void* pArg)
{
	__super::Clone(pArg);

	CEsther_Part_forCut* pInstance = new CEsther_Part_forCut(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEsther_Part_forCut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEsther_Part_forCut::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
