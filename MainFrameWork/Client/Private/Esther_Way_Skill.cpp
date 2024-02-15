#include "stdafx.h"
#include "Esther_Way_Skill.h"
#include "GameInstance.h"
#include "Pool.h"
#include "Model.h"
#include "Player.h"
#include "Esther.h"
#include "Esther_Way_Cut.h"
#include "Esther_Way_Dochul.h"

#include "Effect_Manager.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"
#include "Esther_Scene.h"

CEsther_Way_Skill::CEsther_Way_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEsther_Skill(pDevice, pContext)
{
}

CEsther_Way_Skill::CEsther_Way_Skill(const CEsther_Way_Skill& rhs)
	: CEsther_Skill(rhs)
{
}

HRESULT CEsther_Way_Skill::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther_Way_Skill::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_strObjectTag = TEXT("Esther_Way_Skill");

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	if (FAILED(Ready_ModelPart()))
		return E_FAIL;

	if (FAILED(Ready_Projectile()))
		return E_FAIL;

	m_pSkillMesh = static_cast<CEsther_Way_Dochul*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Way_Dochul")));
	if (nullptr == m_pSkillMesh)
		return E_FAIL;

	m_iAnimIndex = m_pModelCom->Initailize_FindAnimation(L"sk_dochul", 1.f);
	if (m_iAnimIndex == -1)
		return E_FAIL;

	m_pModelCom->Set_CurrAnim(m_iAnimIndex);
	m_pModelCom->Play_Animation(0.0f);

	for (size_t i = 0; i < 6; i++)
	{
		m_bActActive[i] = false;
	}

	return S_OK;
}

void CEsther_Way_Skill::Tick(_float fTimeDelta)
{
	Check_Finish();

	if (true == m_IsFinished)
		return;

	Cut_Start(fTimeDelta);
	Act1(fTimeDelta);
	Act2(fTimeDelta);
	Act3(fTimeDelta);

	Effect(fTimeDelta);


	__super::Tick(fTimeDelta);
	m_pSkillMesh->Tick(fTimeDelta);
	
	m_pSkillMesh->Set_Sound_Init(false);

}

void CEsther_Way_Skill::LateTick(_float fTimeDelta)
{
	if (true == m_IsFinished)
		return;

	__super::LateTick(fTimeDelta);
	m_pSkillMesh->LateTick(fTimeDelta);
}

void CEsther_Way_Skill::Reset()
{
	m_IsFinished = true;
}

void CEsther_Way_Skill::Ready()
{
	//m_pModelCom->Set_CurrAnim(m_iAnimIndex);
	Reserve_Animation(m_iAnimIndex, 0.1f, 0, 0);

	for (size_t i = 0; i < 6; i++)
	{
		m_bActActive[i] = false;
	}

	
	CSound_Manager::GetInstance()->PlaySoundFile(TEXT("Effect"), TEXT("WWISEDEFAULTBANK_PC_COMMON_ESTHER#105 (498590111).wav"), 0.7f);
	

	m_bCutStart = false;

	m_IsFinished = false;

	for (size_t i = 0; i < 6; i++)
	{
		m_bEffectStart[i] = false;
	}
}

void CEsther_Way_Skill::Cut_Start(_float fTimeDelta)
{
	if (0 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) && false == m_bCutStart)
	{
		Add_Sound(L"Effect", L"WWISEDEFAULTBANK_PC_COMMON_ESTHER#193 (217283684)");
		m_pOwnerEsther->Get_Esther_Scene()->Play_Frame();
		m_bCutStart = true;
	}
}

void CEsther_Way_Skill::Act1(_float fTimeDelta)
{
	/* 도철소환 */
	if (50 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) && false == m_bActActive[0])
	{
		Vec3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		vRight.Normalize();
		Vec3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();

		Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos += vRight * 1.f;
		vPos += vLook * 0.5f;

		m_pSkillMesh->Get_TransformCom()->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
		m_pSkillMesh->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
		m_pSkillMesh->Get_TransformCom()->Set_Scale(Vec3(2.5f, 2.5f, 2.5f));

		m_pSkillMesh->Call_Act1(fTimeDelta);

		m_bActActive[0] = true;
	}
	/* 충돌체 소환 */
	if (55 == m_pModelCom->Get_Anim_Frame(m_iAnimIndex)
		&& true == m_pLeaderPlayer->Is_Control() 
		&& false == m_bActActive[1])
	{
		CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
		m_vecSkillProjDesces[0].vAttackPos = Vec3();
		m_vecSkillProjDesces[0].AttackMatrix = m_pTransformCom->Get_WorldMatrix();
		pSkill->InitProjectile(&m_vecSkillProjDesces[0]);

		m_bActActive[1] = true;
	}
}

void CEsther_Way_Skill::Act2(_float fTimeDelta)
{
	/* 도철소환 */
	if (90 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex)
		&& false == m_bActActive[2])
	{
		Vec3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
		vRight.Normalize();
		Vec3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();

		Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos += vRight * -1.f;
		vPos += vLook * 0.5f;

		m_pSkillMesh->Get_TransformCom()->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
		m_pSkillMesh->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
		m_pSkillMesh->Get_TransformCom()->Set_Scale(Vec3(2.5f, 2.5f, 2.5f));

		m_pSkillMesh->Call_Act2(fTimeDelta);

		m_bActActive[2] = true;
	}
	/* 충돌체 소환 */
	if (95 == m_pModelCom->Get_Anim_Frame(m_iAnimIndex)
		&& true == m_pLeaderPlayer->Is_Control()
		&& false == m_bActActive[3])
	{
		CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
		m_vecSkillProjDesces[0].vAttackPos = Vec3();
		m_vecSkillProjDesces[0].AttackMatrix = m_pTransformCom->Get_WorldMatrix();
		pSkill->InitProjectile(&m_vecSkillProjDesces[0]);

		m_bActActive[3] = true;
	}
}

void CEsther_Way_Skill::Act3(_float fTimeDelta)
{
	/* 도철소환 */
	if (140 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) && false == m_bActActive[4])
	{
		Vec3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();

		Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos += vLook * -2.f;

		m_pSkillMesh->Get_TransformCom()->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
		m_pSkillMesh->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
		m_pSkillMesh->Get_TransformCom()->Set_Scale(Vec3(2.5f, 2.5f, 2.5f));

		m_pSkillMesh->Call_Act3(fTimeDelta);

		m_bActActive[4] = true;
	}
	/* 충돌체 소환 */
	if (145 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex)
		&& true == m_pLeaderPlayer->Is_Control()
		&& false == m_bActActive[5])
	{
		CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
		m_vecSkillProjDesces[0].vAttackPos = Vec3();
		m_vecSkillProjDesces[0].AttackMatrix = m_pTransformCom->Get_WorldMatrix();
		pSkill->InitProjectile(&m_vecSkillProjDesces[0]);

		m_bActActive[5] = true;
	}
}

void CEsther_Way_Skill::Effect(_float fTimeDelta)
{
	if (false == m_bEffectStart[0])
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		Matrix& matWayePivot = Get_TransformCom()->Get_WorldMatrix();

		tDesc.pPivotMatrix = &matWayePivot;
		EFFECT_START(TEXT("EstherSkill_Waye_Appear"), &tDesc)

		m_bEffectStart[0] = true;
	}
	
	if (false == m_bEffectStart[1] && 17 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		Matrix& matWayePivot = Get_TransformCom()->Get_WorldMatrix();

		tDesc.pPivotMatrix = &matWayePivot;
		EFFECT_START(TEXT("EstherSkill_Waye_Start"), &tDesc)

		m_bEffectStart[1] = true;
	}

	if (false == m_bEffectStart[2] && 55 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		Matrix& matPivot = Get_TransformCom()->Get_WorldMatrix();
		tDesc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("EstherSkill_Waye_Small1"), &tDesc)

		CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();

		Vec3 vDochulCenter = m_pSkillMesh->Get_TransformCom()->Get_WorldMatrix().Translation();

		pCamera->Cam_Shake(0.1f, 80.f, 0.2f, 5.0f);
		pCamera->Set_RadialBlur(0.04f, vDochulCenter, 0.15f, 0.08f);
		pCamera->Set_Chromatic(0.04f, vDochulCenter, 0.15f, 0.08f);

		m_bEffectStart[2] = true;
	}

	if (false == m_bEffectStart[3] && 95 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		Matrix& matPivot = Get_TransformCom()->Get_WorldMatrix();
		tDesc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("EstherSkill_Waye_Small1"), &tDesc)

		CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();

		Vec3 vDochulCenter = m_pSkillMesh->Get_TransformCom()->Get_WorldMatrix().Translation();

		pCamera->Cam_Shake(0.1f, 80.f, 0.2f, 5.0f);
		pCamera->Set_RadialBlur(0.04f, vDochulCenter, 0.15f, 0.08f);
		pCamera->Set_Chromatic(0.04f, vDochulCenter, 0.15f, 0.08f);

		m_bEffectStart[3] = true;
	}

	if (false == m_bEffectStart[4] && 145 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		Matrix matPivot = Get_TransformCom()->Get_WorldMatrix();
		tDesc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("EstherSkill_Waye_Small2"), &tDesc)

		CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();

		Vec3 vDochulCenter = m_pSkillMesh->Get_TransformCom()->Get_WorldMatrix().Translation();

		pCamera->Cam_Shake(0.1f, 80.f, 0.25f, 5.0f);
		pCamera->Set_RadialBlur(0.05f, vDochulCenter, 0.15f, 0.08f);
		pCamera->Set_Chromatic(0.05f, vDochulCenter, 0.15f, 0.08f);

		m_bEffectStart[4] = true;
	}
	
	if (false == m_bEffectStart[5] && 165 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		Matrix& matPivot = Get_TransformCom()->Get_WorldMatrix();
		tDesc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("EstherSkill_Waye_Big"), &tDesc)

		CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();

		Vec3 vDochulCenter = m_pSkillMesh->Get_TransformCom()->Get_WorldMatrix().Translation();

		pCamera->Cam_Shake(0.3f, 100.0f, 0.45f, 5.5f);
		pCamera->Set_RadialBlur(0.1f, vDochulCenter, 0.1f, 0.08f);
		pCamera->Set_Chromatic(0.1f, vDochulCenter, 0.13f, 0.1f);

		m_bEffectStart[5] = true;
	}
}

void CEsther_Way_Skill::Check_Finish()
{
	if (true == m_pModelCom->Is_AnimationEnd(m_iAnimIndex))
	{

		m_IsFinished = true;
	}

}

HRESULT CEsther_Way_Skill::Render()
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

HRESULT CEsther_Way_Skill::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	{
		if (nullptr == m_pModelPartCom[(_uint)MODEL_PART::FACE])
			return E_FAIL;

		_uint		iNumMeshes = m_pModelPartCom[(_uint)MODEL_PART::FACE]->Get_NumMeshes();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelPartCom[(_uint)MODEL_PART::FACE]->Render(m_pShaderCom, i, "ShadowPass")))
				return S_OK;
		}
	}

	{
		if (nullptr == m_pModelPartCom[(_uint)MODEL_PART::BODY])
			return E_FAIL;

		_uint		iNumMeshes = m_pModelPartCom[(_uint)MODEL_PART::BODY]->Get_NumMeshes();

		for (_uint i = 0; i < iNumMeshes; ++i)
		{
			if (FAILED(m_pModelPartCom[(_uint)MODEL_PART::BODY]->Render(m_pShaderCom, i, "ShadowPass")))
				return S_OK;
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CEsther_Way_Skill::Ready_Components()
{
	__super::Ready_Components();

	wstring strComName = L"Prototype_Component_Model_ESWY";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEsther_Way_Skill::Ready_ModelPart()
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

HRESULT CEsther_Way_Skill::Ready_Parts()
{
	/* 웨이는 주먹을 쓰지요 */
	return S_OK;
}

HRESULT CEsther_Way_Skill::Ready_Projectile()
{
	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = this;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_ESTHER;
	Proj_Desc.fAttackTime = 0.1f;
	Proj_Desc.fRadius = 2.5f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 2.5f);
	Proj_Desc.iDamage = 1500;
	Proj_Desc.iStagger = 600;

	m_vecSkillProjDesces.push_back(Proj_Desc);

	return S_OK;
}

CEsther_Way_Skill* CEsther_Way_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEsther_Way_Skill* pInstance = new CEsther_Way_Skill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CEsther_Way_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Way_Skill::Clone(void* pArg)
{
	CEsther_Way_Skill* pInstance = new CEsther_Way_Skill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEsther_Way_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEsther_Way_Skill::Free()
{
	__super::Free();

	for (size_t i = 0; i < (_uint)MODEL_PART::_END; i++)
	{
		Safe_Release(m_pModelPartCom[i]);
	}

	Safe_Release(m_pPart);
	Safe_Release(m_pSkillMesh);
}
