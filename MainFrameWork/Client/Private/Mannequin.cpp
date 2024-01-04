#include "stdafx.h"
#include "GameInstance.h"
#include "Mannequin.h"
#include "Model.h"
#include "Key_Manager.h"
#include "MannequinPart.h"

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

	m_pModelCom = nullptr;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec3(0.f, -1.f, 0.f));

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

	if (KEY_HOLD(KEY::DOWN_ARROW))
	{
		m_pTransformCom->Go_Down(fTimeDelta);
	}

	if (KEY_HOLD(KEY::LEFT_ARROW))
	{
		m_pTransformCom->Turn(Vec3(0.f, 1.f, 0.f), fTimeDelta * -1.f);
	}

	if (KEY_HOLD(KEY::RIGHT_ARROW))
	{
		m_pTransformCom->Turn(Vec3(0.f, 1.f, 0.f), fTimeDelta);
	}
	if (KEY_HOLD(KEY::UP_ARROW))
	{
		m_pTransformCom->Go_Up(fTimeDelta);
	}

	if (KEY_TAP(KEY::O))
	{
		_uint iCurrAnimIdx = m_pModelCom->Get_CurrAnim();
		if (iCurrAnimIdx <= 0)
			m_pModelCom->Set_CurrAnim(0);
		else
			m_pModelCom->Set_CurrAnim(--iCurrAnimIdx);
	}

	if (KEY_TAP(KEY::P))
	{
		_uint iCurrAnimIdx = m_pModelCom->Get_CurrAnim();

		if (iCurrAnimIdx >= m_pModelCom->Get_MaxAnimIndex())
			m_pModelCom->Set_CurrAnim(m_pModelCom->Get_MaxAnimIndex());
		else
			m_pModelCom->Set_CurrAnim(++iCurrAnimIdx);
	}

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

	
	m_pModelCom->Play_Animation(fTimeDelta);

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

 	for (_uint i = 0; i < m_pModelCom->Get_NumMeshes(); ++i)
	{
		if (FAILED(m_pModelCom->Render_SingleMesh(m_pShaderCom, i)))
			return E_FAIL;
	}
}

void CMannequin::Set_ModelCom(CModel* pModel)
{
	m_pModelCom = pModel;
	for (size_t i = 0; i < PART_END; i++)
	{
		static_cast<CPartObject*>(m_pPart[i])->Change_ParentModelCom(m_pModelCom);
		static_cast<CPartObject*>(m_pPart[i])->Change_ModelCom(nullptr);
	}
	if(nullptr != m_pModelCom)
		m_pModelCom->Set_CurrAnim(0);
}

void CMannequin::Set_Part(_uint PartType, CModel* pModel, Matrix LocalMatrix)
{
	if (nullptr == pModel)
	{
		static_cast<CPartObject*>(m_pPart[PartType])->Change_ModelCom(nullptr);
		return;
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
		static_cast<CPartObject*>(m_pPart[PART_R])->Change_BoneIndex(iBoneIndex);
		static_cast<CPartObject*>(m_pPart[PART_R])->Change_Pivot(m_pModelCom->Get_PivotMatrix());
		static_cast<CPartObject*>(m_pPart[PART_R])->Get_TransformCom()->Set_WorldMatrix(LocalMatrix);
	}
	else
	{
		static_cast<CPartObject*>(m_pPart[PART_L])->Change_ModelCom(pModel);
		
		iBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_weapon_lhand"));
		if (-1 == iBoneIndex)
		{
			iBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_2"));
		}
		static_cast<CPartObject*>(m_pPart[PART_L])->Change_BoneIndex(iBoneIndex);
		static_cast<CPartObject*>(m_pPart[PART_L])->Change_Pivot(m_pModelCom->Get_PivotMatrix());
		static_cast<CPartObject*>(m_pPart[PART_L])->Get_TransformCom()->Set_WorldMatrix(LocalMatrix);
	}
}

HRESULT CMannequin::Ready_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
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
	vScale.x = 0.009f;
	vScale.y = 0.009f;
	vScale.z = 0.009f;

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
