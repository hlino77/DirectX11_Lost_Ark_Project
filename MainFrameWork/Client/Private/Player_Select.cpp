#include "stdafx.h"
#include "GameInstance.h"
#include "Player_Select.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "Camera_Player.h"
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
#include "Player_Controller.h"

#include "Chat_Manager.h"
#include "BindShaderDesc.h"


CPlayer_Select::CPlayer_Select(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Player_Select", OBJ_TYPE::PLAYER)
{
}

CPlayer_Select::CPlayer_Select(const CPlayer_Select& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer_Select::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CPlayer_Select::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	Reset_Triangle();

	return S_OK;
}

void CPlayer_Select::Tick(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (true == Intersect_Mouse())
		{
			Clicked();
		}
		else
		{
			Unclicked();
		}
	}

	if (m_iSelectAnim != m_iSelectAnim_Normal && m_pModelCom->Is_AnimationEnd(m_iSelectAnim))
	{
		if (m_iSelectAnim == m_iSelectAnim_Start)
		{
			m_iSelectAnim = m_iSelectAnim_Loop;
			Reserve_Animation(m_iSelectAnim, 0.2f, 0, 0);
		}
		else if (m_iSelectAnim == m_iSelectAnim_End)
		{
			m_iSelectAnim = m_iSelectAnim_Normal;
			Reserve_Animation(m_iSelectAnim, 0.2f, 0, 0);
		}
	}


	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta);
}

void CPlayer_Select::LateTick(_float fTimeDelta)
{
	if (m_PlayAnimation.valid())
		m_PlayAnimation.get();

	if (nullptr == m_pRendererCom)
		return;
	{
		READ_LOCK
			for (auto& CollisionStay : m_CollisionList)
				OnCollisionStay(CollisionStay.iColLayer, CollisionStay.pCollider);
	}

	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
		m_pRendererCom->Add_DebugObject(this);
	}

	if (m_bRimLight)
	{
		m_fRimLightTime -= fTimeDelta;
		if (m_fRimLightTime <= 0.0f)
		{
			m_fRimLightTime = 0.0f;
			m_bRimLight = false;
		}
	}
}

HRESULT CPlayer_Select::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return S_OK;

	_float fRimLight = (_float)m_bRimLight;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLight, sizeof(_float))))
		return E_FAIL;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	return S_OK;
}

HRESULT CPlayer_Select::Render_ShadowDepth()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_ShadowWVP()))
		return S_OK;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	return S_OK;
}



HRESULT CPlayer_Select::Render_Debug()
{
	for (auto& Colider : m_Coliders)
	{
		if (Colider.second->IsActive())
		{
			Colider.second->DebugRender();
		}
	}

	return S_OK;
}

_bool CPlayer_Select::Intersect_Mouse()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_float viewX = (+2.0f * pt.x / g_iWinSizeX - 1.0f) / m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ)(0, 0);
	_float viewY = (-2.0f * pt.y / g_iWinSizeY + 1.0f) / m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ)(1, 1);

	Vec4 vRayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Vec4 vRayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

	Vec3 vWorldRayOrigin = XMVector3TransformCoord(vRayOrigin, m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW));
	Vec3 vWorldRayDir = XMVector3TransformNormal(vRayDir, m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW));
	vWorldRayDir.Normalize();

	Ray MouseRay = Ray(vWorldRayOrigin, vWorldRayDir);

	_float fDist;
	for (auto& Colider : m_Coliders)
	{
		if (Colider.second->IsActive())
		{
			return Colider.second->Intersects(MouseRay, fDist);
		}

	}

	return false;
}

void CPlayer_Select::Clicked()
{
	if (true == m_bSelected)
		return;

	m_bSelected = true;

	m_iSelectAnim = m_iSelectAnim_Start;
	Reserve_Animation(m_iSelectAnim, 0.2f, 0, 0);
}

void CPlayer_Select::Unclicked()
{
	m_bSelected = false;

	if (m_iSelectAnim == m_iSelectAnim_Loop || m_iSelectAnim == m_iSelectAnim_Start)
	{
		m_iSelectAnim = m_iSelectAnim_End;
		Reserve_Animation(m_iSelectAnim, 0.2f, 0, 0);
	}
}

HRESULT CPlayer_Select::Ready_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_" + m_strObjectTag;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CCollider::ColliderInfo tColliderInfo;
	tColliderInfo.m_bActive = true;
	tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER;
	CSphereCollider* pCollider = nullptr;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderBody"), (CComponent**)&pCollider, &tColliderInfo)))
		return E_FAIL;

	if (pCollider)
	{
		{
			CCollider::ColliderInfo tChildColliderInfo;
			tChildColliderInfo.m_bActive = true;
			tChildColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;
			COBBCollider* pChildCollider = nullptr;

			if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"), TEXT("Com_ColliderBodyChild"), (CComponent**)&pChildCollider, &tChildColliderInfo)))
				return E_FAIL;

			pCollider->Set_Child(pChildCollider);
		}

		m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_BODY_PLAYER, pCollider);
	}


	RELEASE_INSTANCE(CGameInstance);

	m_vOriginScale.x = 0.005f;
	m_vOriginScale.y = 0.005f;
	m_vOriginScale.z = 0.005f;

	m_pTransformCom->Set_Scale(m_vOriginScale);

	return S_OK;
}

void CPlayer_Select::Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist, _bool bReverse)
{
	m_pModelCom->Reserve_NextAnimation(iAnimIndex, fChangeTime, iStartFrame, iChangeFrame, fRootDist, bReverse);
}

void CPlayer_Select::Free()
{
	__super::Free();

	for (size_t i = 0; i < (_uint)PART::_END; i++)
	{
		Safe_Release(m_pModelPartCom[i]);
	}

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
