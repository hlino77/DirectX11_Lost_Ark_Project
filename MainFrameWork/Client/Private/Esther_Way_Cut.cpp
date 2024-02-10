#include "stdafx.h"
#include "Esther_Way_Cut.h"
#include "GameInstance.h"
#include "Pool.h"
#include "Model.h"
#include "Player.h"
#include "PartObject.h"
#include "Esther.h"
#include "Camera_Cut.h"

CEsther_Way_Cut::CEsther_Way_Cut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEsther_Cut(pDevice, pContext)
{
}

CEsther_Way_Cut::CEsther_Way_Cut(const CEsther_Way_Cut& rhs)
	: CEsther_Cut(rhs)
{
}

HRESULT CEsther_Way_Cut::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther_Way_Cut::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_strObjectTag = TEXT("Esther_Way_Cut");

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	if (FAILED(Ready_ModelPart()))
		return E_FAIL;

	if (FAILED(Ready_CutCamera()))
		return E_FAIL;

	m_iAnimIndex = m_pModelCom->Initailize_FindAnimation(L"evt1_sk_dochul", 1.0f);
	if (m_iAnimIndex == -1)
		return E_FAIL;

	m_pModelCom->Set_CurrAnim(m_iAnimIndex);
	m_pModelCom->Play_Animation(0.0f);

	m_iStartFrame = 0;
	m_iEndFrame = m_pModelCom->Get_Anim_MaxFrame(m_iAnimIndex);

	m_fSaveFrameTime = 0.033f;

	return S_OK;
}

void CEsther_Way_Cut::Tick(_float fTimeDelta)
{
	Check_Finish();

	if (true == m_IsFinished)
		return;

	Act1(fTimeDelta);
	Act2(fTimeDelta);
	Act3(fTimeDelta);

	m_pCutCamera->Tick(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CEsther_Way_Cut::LateTick(_float fTimeDelta)
{
	if (true == m_IsFinished)
		return;

	if (true == m_bShot)
	{
		if (true == m_bActionFrame)
		{
			m_iCurFrame = m_pModelCom->Get_Anim_Frame(m_iAnimIndex);
			if (m_iCurFrame >= m_iStartFrame && m_iCurFrame <= m_iEndFrame && m_iPreFrame != m_iCurFrame)
			{
				m_iPreFrame = m_iCurFrame;
				m_pRendererCom->Set_ScreenShot(true, TEXT("../Bin/Resources/Textures/Esther/ESWY/Way"));
			}
			else if (m_iCurFrame >= m_iStartFrame && m_iCurFrame <= m_iEndFrame && m_iPreFrame == m_iCurFrame)
			{
				m_pRendererCom->Set_ScreenShot(false, TEXT("../Bin/Resources/Textures/Esther/ESWY/Way"));
			}
		}
		else if (true == m_bTimeFrame)
		{
			m_fSaveAcc += fTimeDelta;

			m_iCurFrame = m_pModelCom->Get_Anim_Frame(m_iAnimIndex);
			if (m_iCurFrame >= m_iStartFrame && m_iCurFrame <= m_iEndFrame && m_fSaveFrameTime <= m_fSaveAcc)
			{
				m_fSaveAcc = 0.0f;
				m_pRendererCom->Set_ScreenShot(true, TEXT("../Bin/Resources/Textures/Esther/ESWY/Way"));
			}
			else
			{
				m_pRendererCom->Set_ScreenShot(false, TEXT("../Bin/Resources/Textures/Esther/ESWY/Way"));
			}
		}
	}
	
	__super::LateTick(fTimeDelta);
}

void CEsther_Way_Cut::Reset()
{
	m_bLerpActive = false;
	m_IsFinished = true;
}

void CEsther_Way_Cut::Ready()
{
	m_pTransformCom->Set_WorldMatrix(XMMatrixIdentity());
	m_pTransformCom->My_Rotation(Vec3(0.f, 180.f, 0.f));

	m_pCutCamera->Set_CutMode((_uint)CCamera_Cut::CAMERATYPE::OFFSET);
	m_pCutCamera->Set_Offset(Vec3(0.737f, -0.187f, -0.650f));
	m_pCutCamera->Set_CameraLength(2.2f);
	m_pCutCamera->Set_Direct_TargetOffset(Vec3(-1.303f, 1.224f, 0.f));

	m_pModelPartCom[(_uint)MODEL_PART::FACE] = m_pModelPartCom[(_uint)MODEL_PART::FACE_S_ANGRY];

	m_pModelCom->Set_Enforce_CurrAnimFrame(0);

	m_pModelCom->Set_IgnoreRoot(true);

	m_iCurFrame = 0;
	m_iPreFrame = -1;

	m_IsFinished = false;
}

void CEsther_Way_Cut::Act1(_float fTimeDelta)
{
	if (145 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) &&
		170 > m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		m_pModelPartCom[(_uint)MODEL_PART::FACE] = m_pModelPartCom[(_uint)MODEL_PART::FACE_ANGRY];
	}
}

void CEsther_Way_Cut::Act2(_float fTimeDelta)
{
	if (170 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		m_pModelPartCom[(_uint)MODEL_PART::FACE] = m_pModelPartCom[(_uint)MODEL_PART::FACE_DEFAULT];
	}
}

void CEsther_Way_Cut::Act3(_float fTimeDelta)
{
	if (175 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) && false == m_bLerpActive)
	{
		m_pCutCamera->Set_Lerp_TargetOffset(1.2f, Vec3(-2.f, 1.224f, 0.f), LERP_MODE::EASE_IN);
		m_bLerpActive = true;
	}
}

void CEsther_Way_Cut::Check_Finish()
{
	if (true == m_pModelCom->Is_AnimationEnd(m_iAnimIndex))
	{
		m_pRendererCom->Set_ScreenShot(false);

		m_IsFinished = true;
		m_bLerpActive = false;
	}
}

HRESULT CEsther_Way_Cut::Render()
{
	__super::Render();

	{
		if (nullptr == m_pModelPartCom[(_uint)MODEL_PART::FACE])
			return E_FAIL;

		_uint		iNumMeshes = m_pModelPartCom[(_uint)MODEL_PART::FACE]->Get_NumMeshes();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelPartCom[(_uint)MODEL_PART::FACE]->Render_SingleMesh(m_pShaderCom, i)))
				return E_FAIL;
		}
	}

	{
		if (nullptr == m_pModelPartCom[(_uint)MODEL_PART::BODY])
			return E_FAIL;

		_uint		iNumMeshes = m_pModelPartCom[(_uint)MODEL_PART::BODY]->Get_NumMeshes();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelPartCom[(_uint)MODEL_PART::BODY]->Render_SingleMesh(m_pShaderCom, i)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CEsther_Way_Cut::Ready_Components()
{
	__super::Ready_Components();

	wstring strComName = L"Prototype_Component_Model_ESWY";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEsther_Way_Cut::Ready_ModelPart()
{
	wstring strComName = TEXT("Prototype_Component_Model_ESWY_Face_Default");
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face_Default"), (CComponent**)&m_pModelPartCom[(_uint)MODEL_PART::FACE_DEFAULT])))
		return E_FAIL;

	strComName = TEXT("Prototype_Component_Model_ESWY_Face_Small_Angry");
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face_S_Angry"), (CComponent**)&m_pModelPartCom[(_uint)MODEL_PART::FACE_S_ANGRY])))
		return E_FAIL;

	strComName = TEXT("Prototype_Component_Model_ESWY_Face_Angry");
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face_Angry"), (CComponent**)&m_pModelPartCom[(_uint)MODEL_PART::FACE_ANGRY])))
		return E_FAIL;

	strComName = TEXT("Prototype_Component_Model_ESWY_Body");
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pModelPartCom[(_uint)MODEL_PART::BODY])))
		return E_FAIL;

	m_pModelPartCom[(_uint)MODEL_PART::FACE] = m_pModelPartCom[(_uint)MODEL_PART::FACE_S_ANGRY];

	return S_OK;
}

HRESULT CEsther_Way_Cut::Ready_Parts()
{

	return S_OK;
}

HRESULT CEsther_Way_Cut::Ready_CutCamera()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CCamera_Cut::CUTCAMERADESC CameraDesc;

	CameraDesc.tCameraDesc.iLayer = (_uint)LAYER_TYPE::LAYER_CAMERA;
	CameraDesc.tCameraDesc.vEye = Vec4(0.f, 10.f, -10.f, 1.f);
	CameraDesc.tCameraDesc.vAt = Vec4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.tCameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.tCameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.tCameraDesc.fNear = 0.2f;
	CameraDesc.tCameraDesc.fFar = 1200.0f;

	CameraDesc.tCameraDesc.TransformDesc.fSpeedPerSec = 30.f;
	CameraDesc.tCameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(60.0f);

	CameraDesc.pCutTarget = this;

	wstring strObject = TEXT("Prototype_GameObject_Camera_Cut");
	m_pCutCamera = static_cast<CCamera_Cut*>(pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Camera_Cut"), &CameraDesc));
	if (m_pCutCamera == nullptr)
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CEsther_Way_Cut* CEsther_Way_Cut::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEsther_Way_Cut* pInstance = new CEsther_Way_Cut(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CEsther_Way_Cut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Way_Cut::Clone(void* pArg)
{
	CEsther_Way_Cut* pInstance = new CEsther_Way_Cut(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEsther_Way_Cut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEsther_Way_Cut::Free()
{
	__super::Free();

	for (size_t i = 0; i < (_uint)MODEL_PART::_END; i++)
	{
		Safe_Release(m_pModelPartCom[i]);
	}

	Safe_Release(m_pPart);
}
