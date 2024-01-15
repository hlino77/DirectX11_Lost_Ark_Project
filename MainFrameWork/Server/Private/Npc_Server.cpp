#include "stdafx.h"
#include "GameInstance.h"
#include "Npc_Server.h"
#include "AsUtils.h"

CNpc_Server::CNpc_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Npc", OBJ_TYPE::NPC)
{
}

CNpc_Server::CNpc_Server(const CNpc_Server& rhs)
	: CGameObject(rhs)
{
}

HRESULT CNpc_Server::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNpc_Server::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	return S_OK;
}

void CNpc_Server::Tick(_float fTimeDelta)
{


}

void CNpc_Server::LateTick(_float fTimeDelta)
{
	
}

HRESULT CNpc_Server::Render()
{

	return S_OK;
}

HRESULT CNpc_Server::Render_Debug()
{
	return S_OK;
}

void CNpc_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{

}

void CNpc_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CNpc_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{

}

HRESULT CNpc_Server::Ready_Components()
{
	//CGameInstance* pGameInstance = CGameInstance::GetInstance();
	//Safe_AddRef(pGameInstance);

	///* For.Com_Transform */
	//CTransform::TRANSFORMDESC		TransformDesc;
	//ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	//TransformDesc.fSpeedPerSec = 5.f;
	//TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	///* For.Com_Renderer */
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
	//	return E_FAIL;

	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
	//	return E_FAIL;

	/////* For.Com_State */
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateMachine)))
	//	return E_FAIL;

	/////* For.Com_RigidBody */
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
	//	return E_FAIL;

	/////* For.Com_Model */
	//wstring strComName = L"Prototype_Component_Model_Boss_" + m_strObjectTag;
	//if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
	//	return E_FAIL;


	//Safe_Release(pGameInstance);

	//Vec3 vScale;
	//vScale.x = 0.01f;
	//vScale.y = 0.01f;
	//vScale.z = 0.01f;

	//m_pTransformCom->Set_Scale(vScale);

	return S_OK;
}

void CNpc_Server::Set_Colliders(_float fTimeDelta)
{

}

HRESULT CNpc_Server::Ready_Coliders()
{


	return S_OK;
}

void CNpc_Server::Free()
{
	__super::Free();
}
