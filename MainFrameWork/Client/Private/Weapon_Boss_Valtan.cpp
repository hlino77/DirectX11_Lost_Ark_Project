#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Weapon_Boss_Valtan.h"


CWeapon_Boss_Valtan::CWeapon_Boss_Valtan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject(pDevice, pContext, L"Weapon_Boss_King", OBJ_TYPE::PART)
{

}

CWeapon_Boss_Valtan::CWeapon_Boss_Valtan(const CWeapon_Boss_Valtan& rhs)
	: CPartObject(rhs)
{

}

HRESULT CWeapon_Boss_Valtan::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CWeapon_Boss_Valtan::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->My_Rotation(Vec3(60.5, -150.2f , 147.4f));
	m_fAnimationSpeed = 1.f;
	return S_OK;
}

void CWeapon_Boss_Valtan::Tick(_float fTimeDelta)
{
	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
	
	XMMATRIX	WorldMatrix = m_pParentModel->Get_CombinedMatrix(m_iSocketBoneIndex) * m_SocketPivotMatrix;

	WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
	WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
	WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix() * WorldMatrix);
}

void CWeapon_Boss_Valtan::LateTick(_float fTimeDelta)
{
	if (m_PlayAnimation.valid())
	{
		m_PlayAnimation.get();
	}
	if (true == Is_Render() && true == m_pOwner->Is_Render())
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_SHADOW, this);
	}
}

HRESULT CWeapon_Boss_Valtan::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Boss_Valtan::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

HRESULT CWeapon_Boss_Valtan::Ready_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_Wp_Boss_Valtan";
	if (FAILED(__super::Add_Component(CGameInstance::GetInstance()->Get_CurrLevelIndex(), strComName,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Boss_Valtan::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_WorldMatrix, sizeof(Matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Push_GlobalVP()))
		return E_FAIL;

	Color vValtanBloom = Color(0.1f, 1.0f, 0.6f, 1.f);
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomColor", &vValtanBloom, sizeof(Color))))
		return E_FAIL;

	return S_OK;
}

CWeapon_Boss_Valtan* CWeapon_Boss_Valtan::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_Boss_Valtan* pInstance = new CWeapon_Boss_Valtan(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Boss_Valtan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Boss_Valtan::Clone(void* pArg)
{
	__super::Clone(pArg);

	CWeapon_Boss_Valtan* pInstance = new CWeapon_Boss_Valtan(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_Boss_Valtan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Boss_Valtan::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
