#include "stdafx.h"
#include "GameInstance.h"
#include "Deco_Npc.h"
#include "Npc.h"
#include "UI_SpeechBubble.h"

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
	__super::Tick(fTimeDelta);

	if (true == m_IsMove)
	{
		Move(fTimeDelta);
	}

	if (true == m_IsTalk)
	{
		Talk(fTimeDelta);
	}
}

void CDeco_Npc::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CDeco_Npc::Render()
{
	__super::Render();

	if (NPCSHAPE::SOL != m_eNpcShape && NPCSHAPE::SP != m_eNpcShape)
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

	if (NPCSHAPE::SOL != m_eNpcShape && NPCSHAPE::SP != m_eNpcShape)
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

	return S_OK;
}

HRESULT CDeco_Npc::Ready_Parts()
{
	__super::Ready_Parts();

	if (NPCSHAPE::SOL != m_eNpcShape && NPCSHAPE::SP != m_eNpcShape)
	{
		/* 초기 장비 및 얼굴 설정 */
		wstring strComName = TEXT("Prototype_Component_Model_") + m_strNpcHead;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face"), (CComponent**)&m_pNpcPartCom[(_uint)PART::FACE])))
			return E_FAIL;

		m_IsHair = m_pNpcPartCom[(_uint)PART::FACE]->Is_HairTexture();

		strComName = TEXT("Prototype_Component_Model_") + m_strNpcBody;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pNpcPartCom[(_uint)PART::BODY])))
			return E_FAIL;
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
				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &m_vHairColor_1, sizeof(Vec4)) ||
					FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &m_vHairColor_2, sizeof(Vec4)))))
					return E_FAIL;

				if (FAILED(m_pNpcPartCom[i]->Render_SingleMesh(m_pShaderCom, j)))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &Vec4(), sizeof(Vec4)) ||
					FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &Vec4(), sizeof(Vec4)))))
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

void CDeco_Npc::Move(const _float& fTimeDelta)
{
	if (false == m_IsReach)
	{
		Vec3 vDir = m_vMovePos - m_vStartPos;
		vDir.Normalize();
		m_pTransformCom->Move_ToPos(vDir, 20.f, 2.5f, fTimeDelta);

		if (Vec3(m_vMovePos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)).Length() <= 0.05f)
		{
			m_IsReach = true;
		}
	}
	else if (true == m_IsReach)
	{
		Vec3 vDir = m_vStartPos - m_vMovePos;
		vDir.Normalize();
		m_pTransformCom->Move_ToPos(vDir, 20.f, 2.5f, fTimeDelta);

		if (Vec3(m_vStartPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)).Length() <= 0.05f)
		{
			m_IsReach = false;
		}
	}


}

void CDeco_Npc::Talk(const _float& fTimeDelta)
{
	if (true == m_pSpeechBuble->Is_Active())
		return;

	m_fTalkStartAcc += fTimeDelta;
	if (m_fTalkStartTime <= m_fTalkStartAcc)
	{
		m_fTalkStartAcc = 0.f;

		Show_SpeechBuble(m_vecTalks[m_iCurrTalk]);

		m_iCurrTalk++;
		if (m_vecTalks.size() <= m_iCurrTalk)
			m_iCurrTalk = 0;
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
