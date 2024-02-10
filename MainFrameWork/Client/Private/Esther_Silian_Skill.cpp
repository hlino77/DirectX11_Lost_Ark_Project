#include "stdafx.h"
#include "Esther_Silian_Skill.h"
#include "GameInstance.h"
#include "Pool.h"
#include "Model.h"
#include "Player.h"
#include "PartObject.h"
#include "Esther.h"
#include "Esther_Silian_Cut.h"
#include "Effect_Manager.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"
#include "Esther_Scene.h"

CEsther_Silian_Skill::CEsther_Silian_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEsther_Skill(pDevice, pContext)
{
}

CEsther_Silian_Skill::CEsther_Silian_Skill(const CEsther_Silian_Skill& rhs)
	: CEsther_Skill(rhs)
{
}

HRESULT CEsther_Silian_Skill::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther_Silian_Skill::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_strObjectTag = TEXT("Esther_Silian_Skill");

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	if (FAILED(Ready_ModelPart()))
		return E_FAIL;

	if (FAILED(Ready_Projectile()))
		return E_FAIL;

	m_iAnimIndex = m_pModelCom->Initailize_FindAnimation(L"sk_swordofchampion", 1.f);
	if (m_iAnimIndex == -1)
		return E_FAIL;

	m_pModelCom->Set_CurrAnim(m_iAnimIndex);
	m_pModelCom->Play_Animation(0.0f);

	return S_OK;
}

void CEsther_Silian_Skill::Tick(_float fTimeDelta)
{
	Check_Finish();

	if (true == m_IsFinished)
		return;

	//Cut_Start(fTimeDelta);

	Act1(fTimeDelta);
	Effect(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CEsther_Silian_Skill::LateTick(_float fTimeDelta)
{
	if (true == m_IsFinished)
		return;

	__super::LateTick(fTimeDelta);
}

void CEsther_Silian_Skill::Reset()
{
	m_IsFinished = true;
}

void CEsther_Silian_Skill::Ready()
{
	Reserve_Animation(m_iAnimIndex, 0.1f, 0, 0);

	m_bCutStart = false;
	m_bProjShot = false;

	m_bEffectStart = false;
	m_IsFinished = false;
}

void CEsther_Silian_Skill::Cut_Start(_float fTimeDelta)
{
	if (0 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) && false == m_bCutStart)
	{
		m_pOwnerEsther->Get_Esther_Scene()->Play_Frame();
		m_bCutStart = true;
	}
}

void CEsther_Silian_Skill::Act1(_float fTimeDelta)
{
	if (95 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex)
		&& true == m_pLeaderPlayer->Is_Control()
		&& false == m_bProjShot)
	{
		CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
		m_vecSkillProjDesces[0].vAttackPos = Vec3();
		m_vecSkillProjDesces[0].AttackMatrix = m_pTransformCom->Get_WorldMatrix();
		pSkill->InitProjectile(&m_vecSkillProjDesces[0]);
		m_bProjShot = true;
	}
}

void CEsther_Silian_Skill::Effect(_float fTimeDelta)
{
	if (false == m_bEffectStart && 90 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		//Matrix& matPivot = Get_TransformCom()->Get_WorldMatrix();
		Matrix matPivot = Get_TransformCom()->Get_WorldMatrix();
		matPivot.Translation(matPivot.Backward());
		tDesc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("EstherSkill_Silian"), &tDesc)

		CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();

		pCamera->Cam_Shake(0.1f, 70.0f, 1.0f, 5.0f);
		pCamera->Set_Chromatic(0.3f, matPivot.Translation(), 0.15f, 0.1f);

		m_bEffectStart = true;
	}
}

void CEsther_Silian_Skill::Check_Finish()
{
	if (true == m_pModelCom->Is_AnimationEnd(m_iAnimIndex))
	{
		m_IsFinished = true;
	}
}

HRESULT CEsther_Silian_Skill::Render()
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

HRESULT CEsther_Silian_Skill::Render_ShadowDepth()
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

HRESULT CEsther_Silian_Skill::Ready_Components()
{
	__super::Ready_Components();

	wstring strComName = L"Prototype_Component_Model_ESSA";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEsther_Silian_Skill::Ready_ModelPart()
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

HRESULT CEsther_Silian_Skill::Ready_Parts()
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

	wstring strObject = TEXT("Prototype_GameObject_Esther_Part");
	m_pPart = static_cast<CPartObject*>(m_pGameInstance->Clone_GameObject(strObject, &PartDesc_Weapon));
	if (nullptr == m_pPart)
		return E_FAIL;

	return S_OK;
}

HRESULT CEsther_Silian_Skill::Ready_Projectile()
{
	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = this;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::OBB;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_ESTHER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 6.f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 4.f);
	Proj_Desc.vChildScale = Vec3(2.f, 0.6f, 4.f);
	Proj_Desc.vChildOffset = Vec3(0.0f, 0.6f, 4.f);
	Proj_Desc.iDamage = 6000;
	Proj_Desc.iStagger = 300;
	m_vecSkillProjDesces.push_back(Proj_Desc);

	return S_OK;
}

CEsther_Silian_Skill* CEsther_Silian_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEsther_Silian_Skill* pInstance = new CEsther_Silian_Skill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CEsther_Silian_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Silian_Skill::Clone(void* pArg)
{
	CEsther_Silian_Skill* pInstance = new CEsther_Silian_Skill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEsther_Silian_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEsther_Silian_Skill::Free()
{
	__super::Free();

	for (size_t i = 0; i < (_uint)MODEL_PART::_END; i++)
	{
		Safe_Release(m_pModelPartCom[i]);
	}

	Safe_Release(m_pPart);
}
