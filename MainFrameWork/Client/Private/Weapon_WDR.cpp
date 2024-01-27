#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Weapon_WDR.h"

CWeapon_WDR::CWeapon_WDR(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject(pDevice, pContext, L"WDR_Weapon", OBJ_TYPE::PART)
{

}

CWeapon_WDR::CWeapon_WDR(const CWeapon_WDR& rhs)
	: CPartObject(rhs)
{

}

HRESULT CWeapon_WDR::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CWeapon_WDR::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 부모 소켓행렬을 기준으로 자식의 상태를 제어한다.  */
	m_pTransformCom->My_Rotation(Vec3(-10.f, 100.f, -90.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(0.f, -0.03f, 0.f));

	return S_OK;
}

void CWeapon_WDR::Tick(_float fTimeDelta)
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

void CWeapon_WDR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWeapon_WDR::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWeapon_WDR::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

HRESULT CWeapon_WDR::Ready_Components()
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

	return S_OK;
}

HRESULT CWeapon_WDR::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_WorldMatrix, sizeof(Matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Push_GlobalVP()))
		return E_FAIL;

	return S_OK;
}

CWeapon_WDR* CWeapon_WDR::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_WDR* pInstance = new CWeapon_WDR(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_WDR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_WDR::Clone(void* pArg)
{
	__super::Clone(pArg);

	CWeapon_WDR* pInstance = new CWeapon_WDR(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_WDR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_WDR::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
