#include "stdafx.h"
#include "Esther_Silian_Cut.h"
#include "GameInstance.h"
#include "Pool.h"
#include "Model.h"
#include "Player.h"
#include "PartObject.h"
#include "Esther.h"
#include "Camera_Cut.h"

CEsther_Silian_Cut::CEsther_Silian_Cut(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEsther_Cut(pDevice, pContext)
{
}

CEsther_Silian_Cut::CEsther_Silian_Cut(const CEsther_Silian_Cut& rhs)
	: CEsther_Cut(rhs)
{
}

HRESULT CEsther_Silian_Cut::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther_Silian_Cut::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_strObjectTag = TEXT("Esther_Silian_Cut");

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	if (FAILED(Ready_ModelPart()))
		return E_FAIL;

	if (FAILED(Ready_CutCamera()))
		return E_FAIL;

	m_iAnimIndex = m_pModelCom->Initailize_FindAnimation(L"evt1_sk_swordofchampion", 1.f);
	if (m_iAnimIndex == -1)
		return E_FAIL;

	m_pModelCom->Set_CurrAnim(m_iAnimIndex);
	m_pModelCom->Play_Animation(0.0f);

	for (size_t i = 0; i < 4; i++)
	{
		m_bCut[i] = false;
	}


	m_iStartFrame = 0;
	m_iEndFrame = m_pModelCom->Get_Anim_MaxFrame(m_iAnimIndex);

	return S_OK;
}

void CEsther_Silian_Cut::Tick(_float fTimeDelta)
{
	Check_Finish();

	if (true == m_IsFinished)
		return;

	Cut1(fTimeDelta);
	Cut2(fTimeDelta);
	Cut3(fTimeDelta);
	Cut4(fTimeDelta);

	Act1(fTimeDelta);
	Act2(fTimeDelta);
	Act3(fTimeDelta);

	m_pCutCamera->Tick(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CEsther_Silian_Cut::LateTick(_float fTimeDelta)
{
	if (true == m_IsFinished)
		return;

	if (true == m_bShot)
	{
		m_iCurFrame = m_pModelCom->Get_Anim_Frame(m_iAnimIndex);
		if (m_iCurFrame >= m_iStartFrame && m_iCurFrame <= m_iEndFrame && m_iPreFrame != m_iCurFrame)
		{
			m_iPreFrame = m_iCurFrame;
			m_pRendererCom->Set_ScreenShot(true, TEXT("../Bin/Resources/Textures/Esther/ESSA/Silian"));
		}
		else if (m_iCurFrame >= m_iStartFrame && m_iCurFrame <= m_iEndFrame && m_iPreFrame == m_iCurFrame)
		{
			m_pRendererCom->Set_ScreenShot(false, TEXT("../Bin/Resources/Textures/Esther/ESSA/Silian"));
		}
	}

	__super::LateTick(fTimeDelta);
}

void CEsther_Silian_Cut::Reset()
{
	m_IsFinished = true;
}

void CEsther_Silian_Cut::Ready()
{
	// 모델 초기화
	m_pTransformCom->Set_WorldMatrix(XMMatrixIdentity());
	m_pTransformCom->My_Rotation(Vec3(0.f, 180.f, 0.f));

	// 카메라 초기화
	m_pCutCamera->Set_CutMode((_uint)CCamera_Cut::CAMERATYPE::OFFSET);
	m_pCutCamera->Set_Offset(Vec3(0.729f, -0.186f, -0.659f));
	m_pCutCamera->Set_CameraLength(2.68f);
	m_pCutCamera->Set_Direct_TargetOffset(Vec3(-1.533f, 1.288f, 0.f));

	// 얼굴 초기화
	m_pModelPartCom[(_uint)MODEL_PART::FACE] = m_pModelPartCom[(_uint)MODEL_PART::FACE_DEFAULT];

	// 파츠 초기화
	m_pPart->Set_Render(false);

	// 애님 초기화
	Reserve_Animation(m_iAnimIndex, 0.1f, 0, 0, 1.f, false, false, false);
	m_pModelCom->Set_IgnoreRoot(false);

	// 시퀸스 초기화
	for (size_t i = 0; i < 4; i++)
	{
		m_bCut[i] = false;
	}

	m_iCurFrame = 0;
	m_iPreFrame = -1;

	m_IsFinished = false;
}

void CEsther_Silian_Cut::Cut1(_float fTimeDelta)
{
	if (30 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) && false == m_bCut[0])
	{
		m_pCutCamera->Set_Lerp_Offset(0.9f, Vec3(0.891f, -0.229f, -0.392f), LERP_MODE::EASE_OUT);
		m_pCutCamera->ZoomInOut(2.74f, 1.2f);
		m_pCutCamera->Set_Lerp_TargetOffset(0.9f, Vec3(-1.917f, 1.206f, 0.f), LERP_MODE::EASE_OUT);

		m_bCut[0] = true;
	}
}

void CEsther_Silian_Cut::Cut2(_float fTimeDelta)
{
	if (55 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) && false == m_bCut[1])
	{
		m_pCutCamera->Set_Lerp_Offset(0.9f, Vec3(0.636f, -0.642f, -0.429f), LERP_MODE::EASE_IN);
		m_pCutCamera->ZoomInOut(2.29f, 2.f);
		m_pCutCamera->Set_Lerp_TargetOffset(0.9f, Vec3(-0.915, 1.889f, -0.193f), LERP_MODE::EASE_IN);

		m_bCut[1] = true;
	}
}

void CEsther_Silian_Cut::Cut3(_float fTimeDelta)
{
	if (90 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) && false == m_bCut[2])
	{
		m_pCutCamera->Set_Lerp_Offset(0.9f, Vec3(0.767, -0.591f, -0.249f), LERP_MODE::EASE_OUT);
		m_pCutCamera->ZoomInOut(1.990, 6.f);
		m_pCutCamera->Set_Lerp_TargetOffset(0.9f, Vec3(-0.995f, 1.892f, -0.060f), LERP_MODE::EASE_OUT);

		m_bCut[2] = true;
	}
}

void CEsther_Silian_Cut::Cut4(_float fTimeDelta)
{
	if (115 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) && false == m_bCut[3])
	{
		m_pCutCamera->Set_Lerp_TargetOffset(0.5f, Vec3(-1.230f, 2.450f, -0.110f), LERP_MODE::EASE_IN);

		m_bCut[3] = true;
	}
}

void CEsther_Silian_Cut::Act1(_float fTimeDelta)
{
	if (10 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		m_pPart->Set_Render(true);
	}
}

void CEsther_Silian_Cut::Act2(_float fTimeDelta)
{
	if (50 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		m_pModelCom->Set_IgnoreRoot(true);
	}
}

void CEsther_Silian_Cut::Act3(_float fTimeDelta)
{
	if (85 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		m_pModelPartCom[(_uint)MODEL_PART::FACE] = m_pModelPartCom[(_uint)MODEL_PART::FACE_S_ANGRY];
	}
}

void CEsther_Silian_Cut::Check_Finish()
{
	if (true == m_pModelCom->Is_AnimationEnd(m_iAnimIndex))
	{
		m_pRendererCom->Set_ScreenShot(false);
		m_IsFinished = true;
	}
}

HRESULT CEsther_Silian_Cut::Render()
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

HRESULT CEsther_Silian_Cut::Ready_Components()
{
	__super::Ready_Components();

	wstring strComName = L"Prototype_Component_Model_ESSA";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEsther_Silian_Cut::Ready_ModelPart()
{
	wstring strComName = TEXT("Prototype_Component_Model_ESSA_Face_Default");
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face_Default"), (CComponent**)&m_pModelPartCom[(_uint)MODEL_PART::FACE_DEFAULT])))
		return E_FAIL;

	strComName = TEXT("Prototype_Component_Model_ESSA_Face_Small_Angry");
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face_S_Angry"), (CComponent**)&m_pModelPartCom[(_uint)MODEL_PART::FACE_S_ANGRY])))
		return E_FAIL;

	strComName = TEXT("Prototype_Component_Model_ESSA_Face_Angry");
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face_Angry"), (CComponent**)&m_pModelPartCom[(_uint)MODEL_PART::FACE_ANGRY])))
		return E_FAIL;

	strComName = TEXT("Prototype_Component_Model_ESSA_Body");
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pModelPartCom[(_uint)MODEL_PART::BODY])))
		return E_FAIL;

	m_pModelPartCom[(_uint)MODEL_PART::FACE] = m_pModelPartCom[(_uint)MODEL_PART::FACE_S_ANGRY];

	return S_OK;
}

HRESULT CEsther_Silian_Cut::Ready_Parts()
{
	CPartObject::PART_DESC			PartDesc_Weapon;
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_1;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_weapon_00"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	PartDesc_Weapon.strModel = TEXT("ESSA_WP");

	m_pTransformCom->Set_WorldMatrix(XMMatrixIdentity());
	m_pTransformCom->My_Rotation(Vec3(30.f, 110.f, -20.f));
	PartDesc_Weapon.OffsetMatrix = m_pTransformCom->Get_WorldMatrix();
	m_pTransformCom->Set_WorldMatrix(XMMatrixIdentity());

	wstring strObject = TEXT("Prototype_GameObject_Esther_Part_forCut");
	m_pPart = static_cast<CPartObject*>(m_pGameInstance->Clone_GameObject(strObject, &PartDesc_Weapon));
	if (nullptr == m_pPart)
		return E_FAIL;

	return S_OK;
}

HRESULT CEsther_Silian_Cut::Ready_CutCamera()
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

CEsther_Silian_Cut* CEsther_Silian_Cut::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEsther_Silian_Cut* pInstance = new CEsther_Silian_Cut(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CEsther_Silian_Cut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Silian_Cut::Clone(void* pArg)
{
	CEsther_Silian_Cut* pInstance = new CEsther_Silian_Cut(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEsther_Silian_Cut");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEsther_Silian_Cut::Free()
{
	__super::Free();

	for (size_t i = 0; i < (_uint)MODEL_PART::_END; i++)
	{
		Safe_Release(m_pModelPartCom[i]);
	}

	Safe_Release(m_pPart);
}
