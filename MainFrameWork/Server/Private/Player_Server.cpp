#include "stdafx.h"
#include "GameInstance.h"
#include "Player_Server.h"
#include "Key_Manager.h"
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include "ColliderSphere.h"
#include "GameSession.h"
#include "ColliderOBB.h"
#include "GameSessionManager.h"

CPlayer_Server::CPlayer_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Player", OBJ_TYPE::PLAYER)
{
}

CPlayer_Server::CPlayer_Server(const CPlayer_Server& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer_Server::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayer_Server::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;
	m_pGameSession = Desc->pGameSession;
	m_iClass = Desc->iClass;


	if (FAILED(Ready_Components()))
		return E_FAIL;


	if (FAILED(Ready_Animations()))
		return E_FAIL;



    return S_OK;
}

void CPlayer_Server::Tick(_float fTimeDelta)
{
}

void CPlayer_Server::LateTick(_float fTimeDelta)
{
	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta);

	Set_Colliders(fTimeDelta);


	if (m_bRender)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CPlayer_Server::Render()
{
	m_PlayAnimation.get();

    return S_OK;
}

void CPlayer_Server::OnCollisionEnter(const _uint iColLayer, CCollider* pOther)
{
	/*CGameInstance* pGameInstance = CGameInstance::GetInstance();
	pGameInstance->AddRef();


	Protocol::S_COLLISION pkt;

	pkt.set_benter(true);

	pkt.set_ilevel(pGameInstance->Get_CurrLevelIndex());
	pkt.set_ilayer((_uint)LAYER_TYPE::LAYER_PLAYER);
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_icollayer(iColLayer);


	CGameObject* pOtherObject = pOther->Get_Owner();
	pkt.set_iotherid(pOtherObject->Get_ObjectID());
	pkt.set_iotherlayer(pOtherObject->Get_ObjectLayer());
	pkt.set_iothercollayer(pOther->Get_ColLayer());

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);

	Safe_Release(pGameInstance);*/
}

void CPlayer_Server::OnCollisionStay(const _uint iColLayer, CCollider* pOther)
{
}

void CPlayer_Server::OnCollisionExit(const _uint iColLayer, CCollider* pOther)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	pGameInstance->AddRef();


	Protocol::S_COLLISION pkt;

	pkt.set_benter(false);

	pkt.set_ilevel(pGameInstance->Get_CurrLevelIndex());
	pkt.set_ilayer((_uint)LAYER_TYPE::LAYER_PLAYER);
	pkt.set_iobjectid(m_iObjectID);
	pkt.set_icollayer(iColLayer);


	CGameObject* pOtherObject = pOther->Get_Owner();
	pkt.set_iotherid(pOtherObject->Get_ObjectID());
	pkt.set_iotherlayer(pOtherObject->Get_ObjectLayer());
	pkt.set_iothercollayer(pOther->Get_ColLayer());

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);

	Safe_Release(pGameInstance);
}


void CPlayer_Server::Set_Colliders(_float fTimeDelta)
{

}

HRESULT CPlayer_Server::Ready_Animations()
{
	if (m_strObjectTag == L"Naruto")
		Ready_NarutoAnimation();
	else if (m_strObjectTag == L"Sasuke")
		Ready_SasukeAnimation();

	return S_OK;
}

HRESULT CPlayer_Server::Ready_SasukeAnimation()
{
	m_pModelCom->Initailize_FindAnimation(L"Idle_Loop", 1.0f);
	m_pModelCom->Initailize_FindAnimation(L"Run_End", 1.0f);
	m_pModelCom->Initailize_FindAnimation(L"Run_Loop", 1.0f);
	m_pModelCom->Initailize_FindAnimation(L"Attack_cmb01", 1.3f);
	m_pModelCom->Initailize_FindAnimation(L"Attack_cmb03", 1.0f);
	m_pModelCom->Initailize_FindAnimation(L"Attack_cmb06", 1.0f);
	m_pModelCom->Initailize_FindAnimation(L"Attack_cmb08", 1.0f);

	return S_OK;
}

HRESULT CPlayer_Server::Ready_NarutoAnimation()
{
	m_pModelCom->Initailize_FindAnimation(L"Idle_Loop", 1.0f);
	m_pModelCom->Initailize_FindAnimation(L"Run_End", 1.0f);
	m_pModelCom->Initailize_FindAnimation(L"Run_Loop", 1.0f);
	m_pModelCom->Initailize_FindAnimation(L"Attack_ElbowStrike", 2.0f);
	m_pModelCom->Initailize_FindAnimation(L"Attack_JumpDoubleKick", 2.0f);
	m_pModelCom->Initailize_FindAnimation(L"Attack_Punch_Left", 2.0f);
	m_pModelCom->Initailize_FindAnimation(L"Attack_Punch_Right", 2.0f);

	return S_OK;
}


HRESULT CPlayer_Server::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;


	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_" + m_strObjectTag;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	Safe_Release(pGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}

CPlayer_Server* CPlayer_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Server* pInstance = new CPlayer_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer_Server::Clone(void* pArg)
{
	CPlayer_Server* pInstance = new CPlayer_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Server::Free()
{
	__super::Free();

	for (auto& pPart : m_Parts)
		Safe_Release(pPart);

	m_Parts.clear();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
