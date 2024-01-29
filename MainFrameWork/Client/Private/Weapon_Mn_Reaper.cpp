#include "stdafx.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "BindShaderDesc.h"
#include "Weapon_Mn_Reaper.h"
#include <Monster.h>

CWeapon_Mn_Reaper::CWeapon_Mn_Reaper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject(pDevice, pContext, L"Weapon_wp_Reaper", OBJ_TYPE::PART)
{

}

CWeapon_Mn_Reaper::CWeapon_Mn_Reaper(const CWeapon_Mn_Reaper& rhs)
	: CPartObject(rhs)
{

}

HRESULT CWeapon_Mn_Reaper::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CWeapon_Mn_Reaper::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 부모 소켓행렬을 기준으로 자식의 상태를 제어한다.  */
	m_pTransformCom->My_Rotation(Vec3(0.f, 0.f, 280.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(-0.08f, 0.015f, 0.f));

	return S_OK;
}

void CWeapon_Mn_Reaper::Tick(_float fTimeDelta)
{
	XMMATRIX	WorldMatrix = m_pParentModel->Get_CombinedMatrix(m_iSocketBoneIndex) * m_SocketPivotMatrix;

	WorldMatrix.r[0] = XMVector3Normalize(WorldMatrix.r[0]);
	WorldMatrix.r[1] = XMVector3Normalize(WorldMatrix.r[1]);
	WorldMatrix.r[2] = XMVector3Normalize(WorldMatrix.r[2]);

	Compute_RenderMatrix(m_pTransformCom->Get_WorldMatrix() * WorldMatrix);
}

void CWeapon_Mn_Reaper::LateTick(_float fTimeDelta)
{
	if (true == Is_Render() && true == m_pOwner->Is_Render())
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_SHADOW, this);
	}
}

HRESULT CWeapon_Mn_Reaper::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	_int	bDissolve = true;
	if (static_cast<CMonster*>(m_pOwner)->Get_DissolveOut() || static_cast<CMonster*>(m_pOwner)->Get_DissolveIn())
	{
		bDissolve = true;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &bDissolve, sizeof(_int))))
			return E_FAIL;

		_float g_fDissolveAmount = static_cast<CMonster*>(m_pOwner)->Get_Dissolvetime() / static_cast<CMonster*>(m_pOwner)->Get_fMaxDissolvetime();
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveAmount", &g_fDissolveAmount, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Texture("g_DissolveTexture", static_cast<CMonster*>(m_pOwner)->Get_DissolveTexture()->Get_SRV())))
			return E_FAIL;
	}
	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;
	bDissolve = false;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &bDissolve, sizeof(_int))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Mn_Reaper::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

HRESULT CWeapon_Mn_Reaper::Ready_Components()
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
	wstring strComName = L"Prototype_Component_Model_Reaper_Wp";
	if (FAILED(__super::Add_Component(CGameInstance::GetInstance()-> Get_CurrLevelIndex(), strComName, 
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWeapon_Mn_Reaper::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_WorldMatrix, sizeof(Matrix))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Push_GlobalVP()))
		return E_FAIL;

	return S_OK;
}

CWeapon_Mn_Reaper* CWeapon_Mn_Reaper::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_Mn_Reaper* pInstance = new CWeapon_Mn_Reaper(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CWeapon_Mn_Reaper");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Mn_Reaper::Clone(void* pArg)
{
	__super::Clone(pArg);

	CWeapon_Mn_Reaper* pInstance = new CWeapon_Mn_Reaper(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWeapon_Mn_Reaper");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Mn_Reaper::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
