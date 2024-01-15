#include "stdafx.h"
#include "GameInstance.h"
#include "Function_Npc.h"
#include "Npc.h"
#include "UI_SpeechBubble.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "CollisionManager.h"

#include "Player.h"

CFunction_Npc::CFunction_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CNpc(pDevice, pContext)
{
}

CFunction_Npc::CFunction_Npc(const CFunction_Npc& rhs)
	: CNpc(rhs)
{
}

HRESULT CFunction_Npc::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CFunction_Npc::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	__super::Initialize(pArg);

	return S_OK;
}

void CFunction_Npc::Tick(_float fTimeDelta)
{
	Find_Control_Pc();

	__super::Tick(fTimeDelta);
}

void CFunction_Npc::LateTick(_float fTimeDelta)
{
	Set_Colliders(fTimeDelta);

	__super::LateTick(fTimeDelta);

	Set_EffectPos();
}

HRESULT CFunction_Npc::Render()
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

HRESULT CFunction_Npc::Render_ShadowDepth()
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

HRESULT CFunction_Npc::Render_Debug()
{
	__super::Render_Debug();

	return S_OK;
}

HRESULT CFunction_Npc::Ready_Components()
{
	__super::Ready_Components();

	{
		{
			m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_NPC]->SetActive(true);
			m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_NPC]->Set_Radius(0.8f);
			m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_NPC]->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));


			COBBCollider* pChildCollider = dynamic_cast<COBBCollider*>(m_Coliders[(_uint)LAYER_COLLIDER::LAYER_BODY_NPC]->Get_Child());
			pChildCollider->Set_Scale(Vec3(0.4f, 0.6f, 0.4f));
			pChildCollider->Set_Offset(Vec3(0.0f, 0.6f, 0.0f));
			pChildCollider->SetActive(true);
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

HRESULT CFunction_Npc::Ready_Parts()
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

HRESULT CFunction_Npc::Render_Model()
{
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Render_SingleMesh(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CFunction_Npc::Render_Model_Shadow()
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

HRESULT CFunction_Npc::Render_PartModel()
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
				if(FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &m_NpcDesc.vHairColor2, sizeof(Vec4))))
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

HRESULT CFunction_Npc::Render_PartModel_Shadow()
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

HRESULT CFunction_Npc::Find_Control_Pc()
{
	if (nullptr != m_pCtrlPlayer)
		return S_OK;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pCtrlPlayer = static_cast<CPlayer*>(pGameInstance->Find_CtrlPlayer(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_PLAYER));
	if (nullptr == m_pCtrlPlayer)
	{
		RELEASE_INSTANCE(CGameInstance);
		return S_OK;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CFunction_Npc::Set_Colliders(_float fTimeDelta)
{
	for (auto& Collider : m_Coliders)
	{
		if (Collider.second->IsActive())
			Collider.second->Update_Collider();
	}
}

void CFunction_Npc::Free()
{
	__super::Free();
}
