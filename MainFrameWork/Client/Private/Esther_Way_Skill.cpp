#include "stdafx.h"
#include "Esther_Way_Skill.h"
#include "GameInstance.h"
#include "Pool.h"
#include "Model.h"
#include "Esther_Way_Dochul.h"
#include "Player.h"

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
	return S_OK;
}

HRESULT CEsther_Way_Skill::Initialize(CPlayer* pPlayer, void* pArg)
{
	__super::Initialize(pPlayer, pArg);

	m_strObjectTag = TEXT("Esther_Way_Skill");

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	if (FAILED(Ready_ModelPart()))
		return E_FAIL;

	if (FAILED(Ready_Projectile()))
		return E_FAIL;

	m_pSkillMesh = CEsther_Way_Dochul::Create(m_pDevice, m_pContext, nullptr);
	if (nullptr == m_pSkillMesh)
		return E_FAIL;

	m_iAnimIndex = m_pModelCom->Initailize_FindAnimation(L"sk_dochul", 1.0f);
	if (m_iAnimIndex == -1)
		return E_FAIL;

	return S_OK;
}

void CEsther_Way_Skill::Tick(_float fTimeDelta)
{
	Check_Finish();

	if (true == m_IsFinished)
		return;

	Act1(fTimeDelta);
	Act2(fTimeDelta);
	Act3(fTimeDelta);

	__super::Tick(fTimeDelta);
	m_pSkillMesh->Tick(fTimeDelta);
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
	m_pModelCom->Set_CurrAnim(m_iAnimIndex);

	m_IsFinished = false;
}

void CEsther_Way_Skill::Act1(_float fTimeDelta)
{
	/* 도철소환 */
	if (50 == m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
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
		m_pSkillMesh->Get_TransformCom()->Set_Scale(Vec3(0.025f, 0.025f, 0.025f));

		m_pSkillMesh->Call_Act1(fTimeDelta);
	}
	/* 충돌체 소환 */
	if (55 == m_pModelCom->Get_Anim_Frame(m_iAnimIndex)
		&& true == m_pLeaderPlayer->Is_Control())
	{
		CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
		m_vecSkillProjDesces[0].vAttackPos = Vec3();
		m_vecSkillProjDesces[0].AttackMatrix = m_pTransformCom->Get_WorldMatrix();
		pSkill->InitProjectile(&m_vecSkillProjDesces[0]);
	}
}

void CEsther_Way_Skill::Act2(_float fTimeDelta)
{
	/* 도철소환 */
	if (90 == m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
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
		m_pSkillMesh->Get_TransformCom()->Set_Scale(Vec3(0.025f, 0.025f, 0.025f));

		m_pSkillMesh->Call_Act2(fTimeDelta);
	}
	/* 충돌체 소환 */
	if (95 == m_pModelCom->Get_Anim_Frame(m_iAnimIndex)
		&& true == m_pLeaderPlayer->Is_Control())
	{
		CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
		m_vecSkillProjDesces[0].vAttackPos = Vec3();
		m_vecSkillProjDesces[0].AttackMatrix = m_pTransformCom->Get_WorldMatrix();
		pSkill->InitProjectile(&m_vecSkillProjDesces[0]);
	}
}

void CEsther_Way_Skill::Act3(_float fTimeDelta)
{
	/* 도철소환 */
	if (140 == m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		Vec3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();

		Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos += vLook * -2.f;

		m_pSkillMesh->Get_TransformCom()->Set_WorldMatrix(m_pTransformCom->Get_WorldMatrix());
		m_pSkillMesh->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
		m_pSkillMesh->Get_TransformCom()->Set_Scale(Vec3(0.025f, 0.025f, 0.025f));

		m_pSkillMesh->Call_Act3(fTimeDelta);
	}
	/* 충돌체 소환 */
	if (145 == m_pModelCom->Get_Anim_Frame(m_iAnimIndex)
		&& true == m_pLeaderPlayer->Is_Control())
	{
		CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
		m_vecSkillProjDesces[0].vAttackPos = Vec3();
		m_vecSkillProjDesces[0].AttackMatrix = m_pTransformCom->Get_WorldMatrix();
		pSkill->InitProjectile(&m_vecSkillProjDesces[0]);
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

CEsther_Way_Skill* CEsther_Way_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer* pPlayer, void* pArg)
{
	CEsther_Way_Skill* pInstance = new CEsther_Way_Skill(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPlayer, pArg)))
	{
		MSG_BOX("Failed To Created : CEsther_Way_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Way_Skill::Clone(void* pArg)
{
	return nullptr;
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
