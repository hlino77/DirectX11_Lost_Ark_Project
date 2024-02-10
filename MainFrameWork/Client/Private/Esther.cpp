#include "stdafx.h"
#include "GameInstance.h"
#include "Esther.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "ColliderOBB.h"
#include "CollisionManager.h"
#include "PickingMgr.h"
#include "Struct.pb.h"
#include "NavigationMgr.h"
#include "Skill.h"
#include "Pool.h"

#include "BindShaderDesc.h"

#include "Player.h"
#include "Esther_Cut.h"
#include "Esther_Skill.h"
#include "Esther_Scene.h"


CEsther::CEsther(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Esther", OBJ_TYPE::ESTHER)
{
}

CEsther::CEsther(const CEsther& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEsther::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	{
		ESTHERDESC* pDesc = static_cast<ESTHERDESC*>(pArg);

		m_pLeaderPlayer = pDesc->pLeaderPlayer;
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_bActive = false;

	return S_OK;
}

void CEsther::Tick(_float fTimeDelta)
{
	Check_DeActive_Esther();

	if (false == m_bActive)
		return;

	if (nullptr != m_pEsther_Skill)
		m_pEsther_Skill->Tick(fTimeDelta);

	if (nullptr != m_pEsther_Cut)
		m_pEsther_Cut->Tick(fTimeDelta);

	if (nullptr != m_pEsther_Scene)
		m_pEsther_Scene->Tick(fTimeDelta);
}

void CEsther::LateTick(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	if (nullptr != m_pEsther_Skill)
		m_pEsther_Skill->LateTick(fTimeDelta);

	if (nullptr != m_pEsther_Cut)
		m_pEsther_Cut->LateTick(fTimeDelta);

	if (nullptr != m_pEsther_Scene)
		m_pEsther_Scene->LateTick(fTimeDelta);
}

HRESULT CEsther::Render()
{
	return S_OK;
}

void CEsther::Leader_Active_Esther()
{
	if(nullptr != m_pEsther_Skill)
	{
		m_pEsther_Skill->Set_CurrLevel(m_pLeaderPlayer->Get_CurrLevel());

		Vec3 vLook = m_pLeaderPlayer->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();

		Vec3 vPos = m_pLeaderPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		vPos += vLook * 1.f;
		m_pEsther_Skill->Get_TransformCom()->Set_WorldMatrix(XMMatrixIdentity());
		m_pEsther_Skill->Get_TransformCom()->Set_WorldMatrix(m_pLeaderPlayer->Get_TransformCom()->Get_WorldMatrix());
		m_pEsther_Skill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);

		CNavigationMgr::GetInstance()->Find_FirstCell(m_pLeaderPlayer->Get_CurrLevel(), m_pEsther_Skill);

		m_pEsther_Skill->Ready();
	}

	m_bActive = true;
}

void CEsther::NonLeader_Active_Esther()
{
}

void CEsther::Check_DeActive_Esther()
{
	if (nullptr != m_pEsther_Skill && nullptr != m_pEsther_Scene)
	{
		if (true == m_pEsther_Skill->Is_Finished() &&
			false == m_pEsther_Scene->Is_PlayFrame())
		{
			m_pEsther_Skill->Reset();

			m_bActive = false;
		}
	}
}

HRESULT CEsther::Ready_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CEsther::Free()
{
	__super::Free();

	Safe_Release(m_pEsther_Cut);
	Safe_Release(m_pEsther_Skill);
	Safe_Release(m_pEsther_Scene);
}
