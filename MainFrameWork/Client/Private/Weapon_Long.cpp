#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Weapon_Long.h"

CWeapon_Long::CWeapon_Long(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject(pDevice, pContext, L"GN_WP_Long", OBJ_TYPE::PART)
{

}

CWeapon_Long::CWeapon_Long(const CWeapon_Long& rhs)
	: CPartObject(rhs)
{

}

HRESULT CWeapon_Long::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CWeapon_Long::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 부모 소켓행렬을 기준으로 자식의 상태를 제어한다.  */
	/*m_pTransformCom->Rotation(Vec3(1.f, 0.f, 0.f), XMConvertToRadians(-60.f));
	m_pTransformCom->Rotation(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(30.f));
	m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(-75.f));*/

	return S_OK;
}

void CWeapon_Long::Tick(_float fTimeDelta)
{
	XMMATRIX	WorldMatrix = m_pParentModel->Get_CurrBoneMatrix(m_iSocketBoneIndex);

	WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
	WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
	WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix() * WorldMatrix);
}

void CWeapon_Long::LateTick(_float fTimeDelta)
{
	if (true == m_IsRender || true == m_pOwner->Is_Render())
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
	}

}

HRESULT CWeapon_Long::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Long::Render_ShadowDepth()
{

	return S_OK;
}

HRESULT CWeapon_Long::Ready_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

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
	wstring strComName = L"Prototype_Component_Model_GN_WP_Long_Legend";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CWeapon_Long::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_WorldMatrix, sizeof(Matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Push_GlobalVP()))
		return E_FAIL;

	return S_OK;
}

CWeapon_Long* CWeapon_Long::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_Long* pInstance = new CWeapon_Long(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Long");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Long::Clone(void* pArg)
{
	__super::Clone(pArg);

	CWeapon_Long* pInstance = new CWeapon_Long(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_Long");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Long::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
