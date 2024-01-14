#include "stdafx.h"
#include "GameInstance.h"
#include "Guide_Npc_Server.h"
#include "AsUtils.h"

CGuide_Npc_Server::CGuide_Npc_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CNpc_Server(pDevice, pContext)
{
}

CGuide_Npc_Server::CGuide_Npc_Server(const CGuide_Npc_Server& rhs)
	: CNpc_Server(rhs)
{
}

HRESULT CGuide_Npc_Server::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGuide_Npc_Server::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	return S_OK;
}

void CGuide_Npc_Server::Tick(_float fTimeDelta)
{


}

void CGuide_Npc_Server::LateTick(_float fTimeDelta)
{

}

HRESULT CGuide_Npc_Server::Render()
{

	return S_OK;
}

HRESULT CGuide_Npc_Server::Render_Debug()
{
	return S_OK;
}

void CGuide_Npc_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{

}

void CGuide_Npc_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CGuide_Npc_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{

}

HRESULT CGuide_Npc_Server::Ready_Components()
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

void CGuide_Npc_Server::Set_Colliders(_float fTimeDelta)
{

}

HRESULT CGuide_Npc_Server::Ready_Coliders()
{


	return S_OK;
}

CGuide_Npc_Server* CGuide_Npc_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGuide_Npc_Server* pInstance = new CGuide_Npc_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CGuide_Npc_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGuide_Npc_Server::Clone(void* pArg)
{
	CGuide_Npc_Server* pInstance = new CGuide_Npc_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CGuide_Npc_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGuide_Npc_Server::Free()
{
	__super::Free();
}
