#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Weapon_WR.h"

CWeapon_WR::CWeapon_WR(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject(pDevice, pContext, L"WR_Weapon", OBJ_TYPE::PART)
{

}

CWeapon_WR::CWeapon_WR(const CWeapon_WR& rhs)
	: CPartObject(rhs)
{

}

HRESULT CWeapon_WR::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CWeapon_WR::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 부모 소켓행렬을 기준으로 자식의 상태를 제어한다.  */
	m_pTransformCom->My_Rotation(Vec3(0.f, 90.f, -90.f));

	return S_OK;
}

void CWeapon_WR::Tick(_float fTimeDelta)
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

void CWeapon_WR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWeapon_WR::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWeapon_WR::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

void CWeapon_WR::Store_Socket()
{
	m_pTransformCom->Set_WorldMatrix(XMMatrixIdentity());
	m_pTransformCom->Set_Scale(Vec3(1.f, 1.f, 1.f));
	m_pTransformCom->My_Rotation(Vec3(-110.f, -18.f, 93.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(-0.115f, -0.225f, 0.06f));

	__super::Store_Socket();
}

void CWeapon_WR::UnStore_Socket()
{
	m_pTransformCom->Set_WorldMatrix(XMMatrixIdentity());
	m_pTransformCom->Set_Scale(Vec3(1.f, 1.f, 1.f));
	m_pTransformCom->My_Rotation(Vec3(0.f, 90.f, 90.f));

	__super::UnStore_Socket();
}

HRESULT CWeapon_WR::Ready_Components()
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

HRESULT CWeapon_WR::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_WorldMatrix, sizeof(Matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Push_GlobalVP()))
		return E_FAIL;

	return S_OK;
}

CWeapon_WR* CWeapon_WR::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_WR* pInstance = new CWeapon_WR(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_WR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_WR::Clone(void* pArg)
{
	__super::Clone(pArg);

	CWeapon_WR* pInstance = new CWeapon_WR(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_WR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_WR::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
