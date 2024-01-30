#include "stdafx.h"
#include "GameInstance.h"
#include "Deco_Npc.h"
#include "Npc.h"
#include "UI_SpeechBubble.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "CollisionManager.h"

CDeco_Npc::CDeco_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CNpc(pDevice, pContext)
{
}

CDeco_Npc::CDeco_Npc(const CDeco_Npc& rhs)
	: CNpc(rhs)
{
}

HRESULT CDeco_Npc::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CDeco_Npc::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	__super::Initialize(pArg);

	return S_OK;
}

void CDeco_Npc::Tick(_float fTimeDelta)
{
	Find_SameSequence_Npc();

	__super::Tick(fTimeDelta);

	if (true == m_NpcDesc.IsMove)
	{
		Move(fTimeDelta);
	}

	if (true == m_NpcDesc.IsTalk)
	{
		if (m_fTalkDist >= m_fPlayerDist)
		{
			m_bTalkReady = true;
		}
		else
		{
			m_bTalkReady = false;
		}

		if (true == Check_True_All_Sequence_Npc())
		{
			Talk(fTimeDelta);
		}
		if( false == Check_False_All_Sequence_Npc())
		{
			m_iCurrTalk = 0;
			m_fTalkStartAcc = -2.0f;
			m_IsTalkStart = false;
		}
	}
}

void CDeco_Npc::LateTick(_float fTimeDelta)
{
	Set_Colliders(fTimeDelta);

	__super::LateTick(fTimeDelta);

	Set_EffectPos();
}

HRESULT CDeco_Npc::Render()
{
	__super::Render();

	if (NPCSHAPE::SOL != (NPCSHAPE)m_NpcDesc.iNpcShape && NPCSHAPE::SP != (NPCSHAPE)m_NpcDesc.iNpcShape)
	{
		if (FAILED(Render_PartModel()))
			return E_FAIL;
	}
	else
	{
		if (FAILED(Render_Model()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDeco_Npc::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	if (NPCSHAPE::SOL != (NPCSHAPE)m_NpcDesc.iNpcShape && NPCSHAPE::SP != (NPCSHAPE)m_NpcDesc.iNpcShape)
	{
		if (FAILED(Render_PartModel_Shadow()))
			return E_FAIL;
	}
	else
	{
		if (FAILED(Render_Model_Shadow()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDeco_Npc::Render_Debug()
{
	__super::Render_Debug();

	return S_OK;
}

HRESULT CDeco_Npc::Ready_Components()
{
	__super::Ready_Components();

	if ((_uint)LEVELID::LEVEL_TOOL_NPC == m_iCurrLevel)
	{
		{
			m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_NPC]->SetActive(true);
			m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_NPC]->Set_Radius(0.8f);
			m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_NPC]->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));


			/*COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_NPC]->Get_Child());
			pChildCollider->Set_Scale(Vec3(0.4f, 0.6f, 0.4f));
			pChildCollider->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));
			pChildCollider->SetActive(true);*/
		}

		for (auto& Collider : m_Coliders)
		{
			if (Collider.second)
			{
				Collider.second->Update_Collider();
				CCollisionManager::GetInstance()->Add_Colider(Collider.second);
			}
		}
	}

	return S_OK;
}

HRESULT CDeco_Npc::Ready_Parts()
{
	__super::Ready_Parts();

	if (NPCSHAPE::SOL != (NPCSHAPE)m_NpcDesc.iNpcShape && NPCSHAPE::SP != (NPCSHAPE)m_NpcDesc.iNpcShape)
	{
		/* 초기 장비 및 얼굴 설정 */
		wstring strComName = TEXT("Prototype_Component_Model_") + m_NpcDesc.strNpcHead;
		if (FAILED(__super::Add_Component(LEVEL_BERN, strComName, TEXT("Com_Model_Face"), (CComponent**)&m_pNpcPartCom[(_uint)PART::FACE])))
		{
			if (FAILED(__super::Add_Component(LEVEL_TOOL_NPC, strComName, TEXT("Com_Model_Face"), (CComponent**)&m_pNpcPartCom[(_uint)PART::FACE])))
				return E_FAIL;
		}

		m_IsHair = m_pNpcPartCom[(_uint)PART::FACE]->Is_HairTexture();

		strComName = TEXT("Prototype_Component_Model_") + m_NpcDesc.strNpcBody;
		if (FAILED(__super::Add_Component(LEVEL_BERN, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pNpcPartCom[(_uint)PART::BODY])))
		{
			if (FAILED(__super::Add_Component(LEVEL_TOOL_NPC, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pNpcPartCom[(_uint)PART::BODY])))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CDeco_Npc::Render_Model()
{
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Render_SingleMesh(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDeco_Npc::Render_Model_Shadow()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, "ShadowPass")))
			return S_OK;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CDeco_Npc::Render_PartModel()
{
	for (size_t i = 0; i < (_uint)PART::_END; i++)
	{
		if (nullptr == m_pNpcPartCom[i]) continue;

		_uint		iNumMeshes = m_pNpcPartCom[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (i == (_uint)PART::FACE && j == m_IsHair)
			{
				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &m_NpcDesc.vHairColor1, sizeof(Vec4))))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &m_NpcDesc.vHairColor2, sizeof(Vec4))))
					return E_FAIL;

				if (FAILED(m_pNpcPartCom[i]->Render_SingleMesh(m_pShaderCom, j)))
					return E_FAIL;

				Vec4 vResetColor;
				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &vResetColor, sizeof(Vec4))))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &vResetColor, sizeof(Vec4))))
					return E_FAIL;
			}
			else
			{
				if (FAILED(m_pNpcPartCom[i]->Render_SingleMesh(m_pShaderCom, j)))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CDeco_Npc::Render_PartModel_Shadow()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (size_t i = 0; i < (_uint)PART::_END; i++)
	{
		if (nullptr == m_pNpcPartCom[i]) continue;

		_uint		iNumMeshes = m_pNpcPartCom[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{

			if (FAILED(m_pNpcPartCom[i]->Render(m_pShaderCom, j, "ShadowPass")))
				return S_OK;
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CDeco_Npc::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

_bool CDeco_Npc::Check_True_All_Sequence_Npc()
{
	for (auto& pNpc : m_vecSameSequenceNpc)
	{
		if (false == pNpc->Is_TalkReady())
			return false;
	}

	return true;
}

_bool CDeco_Npc::Check_False_All_Sequence_Npc()
{
	for (auto& pNpc : m_vecSameSequenceNpc)
	{
		if (true == pNpc->Is_TalkReady())
			return true;
	}

	return false;
}

void CDeco_Npc::Move(const _float& fTimeDelta)
{
	Vec3 vMove;
	if (m_NpcDesc.vecMovePos.size() == m_iMoveCnt)
	{
		vMove = m_vStartPos;
	}
	else
	{
		vMove = m_NpcDesc.vecMovePos[m_iMoveCnt];
	}

	Vec3 vDir = vMove - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Move_ToPos(vDir, 10.f, 1.f, fTimeDelta);

	if (Vec3(m_pTransformCom->Get_State(CTransform::STATE_POSITION) - vMove).Length() <= 0.05f)
	{
		m_iMoveCnt++;
		if (m_NpcDesc.vecMovePos.size() < m_iMoveCnt)
		{
			m_iMoveCnt = 0;
		}
	}
}

void CDeco_Npc::Talk(const _float& fTimeDelta)
{
	if (true == m_pSpeechBuble->Is_Active())
		return;

	if (false == m_IsTalkStart)
	{
		m_fTalkStartAcc += fTimeDelta;

		if (m_NpcDesc.fTalkStartTime <= m_fTalkStartAcc)
		{
			m_IsTalkStart = true;
		}

		return;
	}
		
	Show_SpeechBuble(m_NpcDesc.vecTalks[m_iCurrTalk]);

	m_iCurrTalk++;
	if (m_NpcDesc.vecTalks.size() <= m_iCurrTalk)
	{
		m_iCurrTalk = 0;
		m_fTalkStartAcc = -5.f;
		m_IsTalkStart = false;
	}
}

CDeco_Npc* CDeco_Npc::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDeco_Npc* pInstance = new CDeco_Npc(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CDeco_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDeco_Npc::Clone(void* pArg)
{
	CDeco_Npc* pInstance = new CDeco_Npc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CDeco_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDeco_Npc::Free()
{
	__super::Free();
}
