#include "stdafx.h"
#include "Esther_Bahuntur_Skill.h"
#include "GameInstance.h"
#include "Pool.h"
#include "Model.h"
#include "Player.h"
#include "PartObject.h"
#include "Esther.h"

#include "Esther_Bahuntur_Skill_Ceiling.h"
#include "Esther_Bahuntur_Skill_Floor.h"

CEsther_Bahuntur_Skill::CEsther_Bahuntur_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEsther_Skill(pDevice, pContext)
{
}

CEsther_Bahuntur_Skill::CEsther_Bahuntur_Skill(const CEsther_Bahuntur_Skill& rhs)
	: CEsther_Skill(rhs)
{
}

HRESULT CEsther_Bahuntur_Skill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEsther_Bahuntur_Skill::Initialize(CPlayer* pPlayer, void* pArg)
{
	__super::Initialize(pPlayer, pArg);

	m_strObjectTag = TEXT("Esther_Bahuntur_Skill");

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	if (FAILED(Ready_ModelPart()))
		return E_FAIL;

	if (FAILED(Ready_Projectile()))
		return E_FAIL;

	m_pSkillFloor = CEsther_Bahuntur_Skill_Floor::Create(m_pDevice, m_pContext, nullptr);
	if (nullptr == m_pSkillFloor)
		return E_FAIL;

	m_pSkillCeiling = CEsther_Bahuntur_Skill_Ceiling::Create(m_pDevice, m_pContext, nullptr);
	if (nullptr == m_pSkillCeiling)
		return E_FAIL;

	m_iAnimIndex = m_pModelCom->Initailize_FindAnimation(L"sk_breathofarcturus", 1.f);
	if (m_iAnimIndex == -1)
		return E_FAIL;

	m_pModelCom->Set_CurrAnim(m_iAnimIndex);
	m_pModelCom->Set_Enforce_CurrAnimFrame(m_pModelCom->Get_Anim_MaxFrame(m_iAnimIndex));
	m_pModelCom->Play_Animation(0.0f);

	return S_OK;
}

void CEsther_Bahuntur_Skill::Tick(_float fTimeDelta)
{
	Check_Finish();

	if (true == m_IsFinished)
		return;

	Act1(fTimeDelta);
	Act2(fTimeDelta);
	Act3(fTimeDelta);

	__super::Tick(fTimeDelta);
	m_pSkillCeiling->Tick(fTimeDelta);
	m_pSkillFloor->Tick(fTimeDelta);
}

void CEsther_Bahuntur_Skill::LateTick(_float fTimeDelta)
{
	if (true == m_IsFinished)
		return;

	__super::LateTick(fTimeDelta);
	m_pSkillCeiling->LateTick(fTimeDelta);
	m_pSkillFloor->LateTick(fTimeDelta);
}

void CEsther_Bahuntur_Skill::Reset()
{
	m_IsFinished = true;
}

void CEsther_Bahuntur_Skill::Ready()
{
	Vec3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	vLook.Normalize();

	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos += vLook * 3.4f;
	vPos.y -= 0.4f;

	m_pSkillCeiling->Get_TransformCom()->Set_WorldMatrix(XMMatrixIdentity());
	m_pSkillCeiling->Get_TransformCom()->Set_Scale(Vec3(2.f, 2.f, 2.f));
	m_pSkillCeiling->Get_TransformCom()->My_Rotation(Vec3(0.f, 0.f, 90.f));
	m_pSkillCeiling->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);

	m_pSkillFloor->Get_TransformCom()->Set_WorldMatrix(XMMatrixIdentity());
	m_pSkillFloor->Get_TransformCom()->Set_Scale(Vec3(2.f, 2.f, 2.f));
	m_pSkillFloor->Get_TransformCom()->My_Rotation(Vec3(0.f, 0, 90.f)); 
	m_pSkillFloor->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);

	m_pSkillCeiling->Ready();
	m_pSkillFloor->Ready();

	m_bAnim = false;
	m_fTimeAcc = 0.0f;

	m_fAnimationSpeed = 0.f;
	Reserve_Animation(m_iAnimIndex, 0.1f, 10, 0);
	

	m_IsFinished = false;
}

void CEsther_Bahuntur_Skill::Act1(_float fTimeDelta)
{
	m_pSkillFloor->Set_Active(true);
	m_pSkillCeiling->Set_Active(true);

	m_fTimeAcc += fTimeDelta;
	if (m_fAnimTime <= m_fTimeAcc)
	{
		if (false == m_bAnim)
			m_fAnimationSpeed = 1.f;

		m_bAnim = true;
	}
}

void CEsther_Bahuntur_Skill::Act2(_float fTimeDelta)
{
	if (30 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex)
		&& 35 > m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		m_pSkillCeiling->Call_Act1(fTimeDelta);
	}
	if (73 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) 
		&& 78 > m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		m_pSkillCeiling->Call_Act2(fTimeDelta);
	}
}

void CEsther_Bahuntur_Skill::Act3(_float fTimeDelta)
{
	if (79 == m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
		m_vecSkillProjDesces[0].vAttackPos = Vec3();
		m_vecSkillProjDesces[0].AttackMatrix = m_pTransformCom->Get_WorldMatrix();
		pSkill->InitProjectile(&m_vecSkillProjDesces[0]);
	}
}

void CEsther_Bahuntur_Skill::Check_Finish()
{
	if (true == m_bAnim && true == m_pModelCom->Is_AnimationEnd(m_iAnimIndex))
	{
		m_pSkillFloor->Set_Active(false);
		m_pSkillCeiling->Set_Active(false);

		m_IsFinished = true;
	}
}

HRESULT CEsther_Bahuntur_Skill::Render()
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

HRESULT CEsther_Bahuntur_Skill::Render_ShadowDepth()
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

HRESULT CEsther_Bahuntur_Skill::Ready_Components()
{
	__super::Ready_Components();

	wstring strComName = L"Prototype_Component_Model_ESBT";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEsther_Bahuntur_Skill::Ready_ModelPart()
{
	wstring strComName = TEXT("Prototype_Component_Model_ESBT_Face_Default");
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face_Default"), (CComponent**)&m_pModelPartCom[(_uint)MODEL_PART::FACE_DEFAULT])))
		return E_FAIL;

	strComName = TEXT("Prototype_Component_Model_ESBT_Face_Small_Angry");
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face_S_Angry"), (CComponent**)&m_pModelPartCom[(_uint)MODEL_PART::FACE_S_ANGRY])))
		return E_FAIL;

	strComName = TEXT("Prototype_Component_Model_ESBT_Face_Angry");
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Face_Angry"), (CComponent**)&m_pModelPartCom[(_uint)MODEL_PART::FACE_ANGRY])))
		return E_FAIL;

	strComName = TEXT("Prototype_Component_Model_ESBT_Body");
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pModelPartCom[(_uint)MODEL_PART::BODY])))
		return E_FAIL;

	m_pModelPartCom[(_uint)MODEL_PART::FACE] = m_pModelPartCom[(_uint)MODEL_PART::FACE_S_ANGRY];

	return S_OK;
}

HRESULT CEsther_Bahuntur_Skill::Ready_Parts()
{
	CPartObject::PART_DESC			PartDesc_Weapon;
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_1;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_1"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	PartDesc_Weapon.strModel = TEXT("ESBT_WP");

	m_pTransformCom->Set_WorldMatrix(XMMatrixIdentity());
	m_pTransformCom->My_Rotation(Vec3(-25.f, 120.f, -90.f));
	PartDesc_Weapon.OffsetMatrix = m_pTransformCom->Get_WorldMatrix();
	m_pTransformCom->Set_WorldMatrix(XMMatrixIdentity());

	wstring strObject = TEXT("Prototype_GameObject_Esther_Part");
	m_pPart = static_cast<CPartObject*>(m_pGameInstance->Clone_GameObject(strObject, &PartDesc_Weapon));
	if (nullptr == m_pPart)
		return E_FAIL;

	return S_OK;
}

HRESULT CEsther_Bahuntur_Skill::Ready_Projectile()
{
	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = this;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_BUFF_ESTHER;
	Proj_Desc.fAttackTime = 0.05f;
	Proj_Desc.fRadius = 10.f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.1f);
	Proj_Desc.iDamage = 1000;
	Proj_Desc.iStagger = 0;
	m_vecSkillProjDesces.push_back(Proj_Desc);

	return S_OK;
}

CEsther_Bahuntur_Skill* CEsther_Bahuntur_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer* pPlayer, void* pArg)
{
	CEsther_Bahuntur_Skill* pInstance = new CEsther_Bahuntur_Skill(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pPlayer, pArg)))
	{
		MSG_BOX("Failed To Created : CEsther_Bahuntur_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Bahuntur_Skill::Clone(void* pArg)
{
	return nullptr;
}

void CEsther_Bahuntur_Skill::Free()
{
	__super::Free();

	for (size_t i = 0; i < (_uint)MODEL_PART::_END; i++)
	{
		Safe_Release(m_pModelPartCom[i]);
	}

	Safe_Release(m_pPart);
}
