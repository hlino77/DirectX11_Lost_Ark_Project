#include "stdafx.h"
#include "GameInstance.h"
#include "Mannequin.h"
#include "Model.h"
#include "Key_Manager.h"
#include "MannequinPart.h"
#include "NavigationMgr.h"
#include "Pool.h"
#include "UI_SpeechBubble.h"

CMannequin::CMannequin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Mannequin", OBJ_TYPE::PLAYER)
{
}

CMannequin::CMannequin(const CMannequin& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMannequin::Initialize_Prototype()
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PlayerPart()))
		return E_FAIL;

	if (FAILED(Ready_SpeechBuble()))
		return E_FAIL;

	m_pModelCom = nullptr;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(0.f, 0.f, 0.f));



	return S_OK;
}

HRESULT CMannequin::Initialize(void* pArg)
{

	return S_OK;
}

void CMannequin::Tick(_float fTimeDelta)
{
	if (nullptr == m_pModelCom)
		return;

	if((_uint)LEVEL_TOOL_NPC == m_iCurrLevel)
		CNavigationMgr::GetInstance()->SetUp_OnCell(LEVEL_TOOL_NPC, this);

	if (true == m_IsMove)
	{
		Move(fTimeDelta);
	}
	if (true == m_IsTalk)
	{
		Talk(fTimeDelta);
	}

	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta);

	for (size_t i = 0; i < PART_END; i++)
	{
		if (nullptr == m_pPart[i]) continue;

		m_pPart[i]->Tick(fTimeDelta);
	}
}

void CMannequin::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom || nullptr == m_pModelCom)
		return;
	
	m_PlayAnimation.get();

	Set_EffectPos();

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	for (size_t i = 0; i < PART_END; i++)
	{
		if (nullptr == m_pPart[i]) continue;

		m_pPart[i]->LateTick(fTimeDelta);
	}
}

HRESULT CMannequin::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return S_OK;

	_float fRimLight = (_float)m_bRimLight;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLight, sizeof(_float))))
		return E_FAIL;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	if (nullptr != m_pModelPart[HEAD] && nullptr != m_pModelPart[BODY])
	{
		for (_uint i = 0; i < m_pModelPart[HEAD]->Get_NumMeshes(); ++i)
		{
			if (i == m_IsHair)
			{
				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &m_vHairColor_1, sizeof(Vec4)) ||
					FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &m_vHairColor_2, sizeof(Vec4)))))
					return E_FAIL;

				if (FAILED(m_pModelPart[HEAD]->Render_SingleMesh(m_pShaderCom, i)))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &Vec4(), sizeof(Vec4)) ||
					FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &Vec4(), sizeof(Vec4)))))
					return E_FAIL;
			}
			else
			{
				if (FAILED(m_pModelPart[HEAD]->Render_SingleMesh(m_pShaderCom, i)))
					return E_FAIL;
			}
		}

		for (_uint i = 0; i < m_pModelPart[BODY]->Get_NumMeshes(); ++i)
		{
			if (FAILED(m_pModelPart[BODY]->Render_SingleMesh(m_pShaderCom, i)))
				return E_FAIL;
		}
	}
	else
	{
		for (_uint i = 0; i < m_pModelCom->Get_NumMeshes(); ++i)
		{
			if (FAILED(m_pModelCom->Render_SingleMesh(m_pShaderCom, i)))
				return E_FAIL;
		}

		if (nullptr != m_pModelPart[HEAD])
		{
			for (_uint i = 0; i < m_pModelPart[HEAD]->Get_NumMeshes(); ++i)
			{
				if (i == m_IsHair)
				{
					if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &m_vHairColor_1, sizeof(Vec4)) ||
						FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &m_vHairColor_2, sizeof(Vec4)))))
						return E_FAIL;

					if (FAILED(m_pModelPart[HEAD]->Render_SingleMesh(m_pShaderCom, i)))
						return E_FAIL;

					if (FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_1", &Vec4(), sizeof(Vec4)) ||
						FAILED(m_pShaderCom->Bind_RawValue("g_vHairColor_2", &Vec4(), sizeof(Vec4)))))
						return E_FAIL;
				}
				else
				{
					if (FAILED(m_pModelPart[HEAD]->Render_SingleMesh(m_pShaderCom, i)))
						return E_FAIL;
				}
			}
		}

		if (nullptr != m_pModelPart[BODY])
		{
			for (_uint i = 0; i < m_pModelPart[BODY]->Get_NumMeshes(); ++i)
			{
				if (FAILED(m_pModelPart[BODY]->Render_SingleMesh(m_pShaderCom, i)))
					return E_FAIL;
			}
		}
	}
}

void CMannequin::Clear_MQ()
{
	m_pModelCom = nullptr;
	for (size_t i = 0; i < PART_END; i++)
	{
		static_cast<CPartObject*>(m_pPart[i])->Change_ParentModelCom(nullptr);
		static_cast<CPartObject*>(m_pPart[i])->Change_ModelCom(nullptr);
		m_pModelPart[i] = nullptr;
	}

	if (nullptr != m_pModelCom)
		m_pModelCom->Set_CurrAnim(0);

	m_IsMove = false;
	m_iMoveCnt = 0;
	m_vecMovePos.clear();
	m_vStartPos = Vec3();

	m_IsTalk = false;
	m_vecTalkScript.clear();
	m_fTalkStartAcc = 0.f;
	m_fTalkTime = 0.f;
	m_iCurrTalk = 0.f;
}

void CMannequin::Set_ModelCom(CModel* pModel)
{
	if (nullptr != m_pModelCom)
		Safe_Release(m_pModelCom);

	m_pModelCom = pModel;
	for (size_t i = 0; i < PART_END; i++)
	{
		static_cast<CPartObject*>(m_pPart[i])->Change_ParentModelCom(m_pModelCom);
		static_cast<CPartObject*>(m_pPart[i])->Change_ModelCom(nullptr);
		m_pModelPart[i] = nullptr;
	}

	if(nullptr != m_pModelCom)
		m_pModelCom->Set_CurrAnim(0);
}

CPartObject* CMannequin::Set_Part(_uint PartType, CModel* pModel, Matrix LocalMatrix)
{
	if (nullptr == pModel)
	{
		static_cast<CPartObject*>(m_pPart[PartType])->Change_ModelCom(nullptr);
		return nullptr;
	}

	_int iBoneIndex = -1;

	if (PartType == PARTTYPE::PART_R)
	{
		static_cast<CPartObject*>(m_pPart[PART_R])->Change_ModelCom(pModel);

		iBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_weapon_rhand"));
		if (-1 == iBoneIndex)
		{
			iBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_1"));
		}
		if (-1 == iBoneIndex)
		{
			return nullptr;
		}


		static_cast<CPartObject*>(m_pPart[PART_R])->Change_BoneIndex(iBoneIndex);
		static_cast<CPartObject*>(m_pPart[PART_R])->Change_Pivot(m_pModelCom->Get_PivotMatrix());
		static_cast<CPartObject*>(m_pPart[PART_R])->Get_TransformCom()->Set_WorldMatrix(LocalMatrix);

		return static_cast<CPartObject*>(m_pPart[PART_R]);
	}
	else
	{
		static_cast<CPartObject*>(m_pPart[PART_L])->Change_ModelCom(pModel);
		
		iBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_weapon_lhand"));
		if (-1 == iBoneIndex)
		{
			iBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_2"));
		}
		if (-1 == iBoneIndex)
		{
			return nullptr;
		}

		static_cast<CPartObject*>(m_pPart[PART_L])->Change_BoneIndex(iBoneIndex);
		static_cast<CPartObject*>(m_pPart[PART_L])->Change_Pivot(m_pModelCom->Get_PivotMatrix());
		static_cast<CPartObject*>(m_pPart[PART_L])->Get_TransformCom()->Set_WorldMatrix(LocalMatrix);

		return static_cast<CPartObject*>(m_pPart[PART_L]);
	}
}

void CMannequin::Set_ModelPart(_uint iIndex, CModel* pModel)
{
	switch (iIndex)
	{
	case MODELTYPE::HEAD:
		m_pModelPart[HEAD] = pModel;
		if(nullptr != m_pModelPart[HEAD])
			m_IsHair = m_pModelPart[HEAD]->Is_HairTexture();
		break;
	case MODELTYPE::BODY:
		m_pModelPart[BODY] = pModel;
		break;
	}
}

void CMannequin::Set_HairColor(Vec3 rgb1, Vec3 rgb2)
{
	m_vHairColor_1 = Vec4(rgb1.x, rgb1.y, rgb1.z, 1.f);
	m_vHairColor_2 = Vec4(rgb2.x, rgb2.y, rgb2.z, 1.f);
}

void CMannequin::Set_Move_State(_bool IsMove)
{
	m_IsMove = IsMove;
	if (false == m_IsMove)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vStartPos);
	}
}

void CMannequin::Set_Talk_State(_bool IsTalk, vector<wstring> TalkScript, _float fTalkTime)
{
	m_IsTalk = IsTalk;
	if (true == m_IsTalk)
	{
		m_vecTalkScript = TalkScript;
		m_fTalkTime = fTalkTime;
		m_fTalkStartAcc = 0.f;
	}
	else if (false == m_IsTalk)
	{
		m_vecTalkScript.clear();
		m_fTalkTime = 0.f;
		m_fTalkStartAcc = 0.f;
	}
}

HRESULT CMannequin::Ready_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.5f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	Vec3 vScale;
	vScale.x = 0.01f;
	vScale.y = 0.01f;
	vScale.z = 0.01f;

	m_pTransformCom->Set_Scale(vScale);

	return S_OK;
}

HRESULT CMannequin::Ready_PlayerPart()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pPart[PART_R] = CMannequinPart::Create(m_pDevice, m_pContext);
	if (nullptr == m_pPart[PART_R])
		return E_FAIL;

	CPartObject::PART_DESC			PartDesc;
	PartDesc.pOwner = this;
	PartDesc.ePart = CPartObject::PARTS::WEAPON_1;
	PartDesc.pParentTransform = m_pTransformCom;
	m_pPart[PART_R]->Initialize(&PartDesc);

	m_pPart[PART_L] = CMannequinPart::Create(m_pDevice, m_pContext);
	if (nullptr == m_pPart[PART_L])
		return E_FAIL;

	PartDesc.pOwner = this;
	PartDesc.ePart = CPartObject::PARTS::WEAPON_2;
	PartDesc.pParentTransform = m_pTransformCom;
	m_pPart[PART_L]->Initialize(&PartDesc);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMannequin::Ready_SpeechBuble()
{
	m_pSpeechBuble = CPool<CUI_SpeechBubble>::Get_Obj();

	if (m_pSpeechBuble == nullptr)
		return E_FAIL;

	m_pSpeechBuble->Set_Host(this);

	return S_OK;
}

void CMannequin::Set_EffectPos()
{
	_uint iBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_effectname"));
	Matrix matEffect = m_pModelCom->Get_CombinedMatrix(iBoneIndex);
	matEffect *= m_pTransformCom->Get_WorldMatrix();
	memcpy(&m_vEffectPos, matEffect.m[3], sizeof(Vec3));
	Matrix ViewMatrix = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW);
	Matrix ProjMatrix = CGameInstance::GetInstance()->Get_TransformMatrix(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ);
	m_vEffectPos = XMVector3TransformCoord(m_vEffectPos, ViewMatrix);
	m_vEffectPos = XMVector3TransformCoord(m_vEffectPos, ProjMatrix);
}

void CMannequin::Move(const _float& fTimeDelta)
{
	Vec3 vMove;
	if (m_vecMovePos.size() == m_iMoveCnt)
	{
		vMove = m_vStartPos;
	}
	else
	{
		vMove = m_vecMovePos[m_iMoveCnt];
	}

	Vec3 vDir = vMove - m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Move_ToPos(vDir, 10.f, 1.f, fTimeDelta);

	if (Vec3(m_pTransformCom->Get_State(CTransform::STATE_POSITION) - vMove).Length() <= 0.05f)
	{
		m_iMoveCnt++;
		if (m_vecMovePos.size() < m_iMoveCnt)
		{
			m_iMoveCnt = 0;
		}
	}
}

void CMannequin::Talk(const _float& fTimeDelta)
{
	if (true == m_pSpeechBuble->Is_Active())
		return;

	if (false == m_IsTalkStart)
	{
		m_fTalkStartAcc += fTimeDelta;

		if (m_fTalkTime <= m_fTalkStartAcc)
		{
			m_IsTalkStart = true;
		}

		return;
	}

	m_pSpeechBuble->Active_SpeechBuble(m_vecTalkScript[m_iCurrTalk]);

	m_iCurrTalk++;
	if (m_vecTalkScript.size() <= m_iCurrTalk)
	{
		m_iCurrTalk = 0;
		m_fTalkStartAcc = -5.f;
		m_IsTalkStart = false;
	}

}

CMannequin* CMannequin::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMannequin* pInstance = new CMannequin(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMannequin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMannequin::Clone(void* pArg)
{
	CMannequin* pInstance = new CMannequin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMannequin");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMannequin::Free()
{
	__super::Free();

	for (size_t i = 0; i < PART_END; i++)
	{
		Safe_Release(m_pPart[i]);
	}
	
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
