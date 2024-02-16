#include "stdafx.h"
#include "Esther_Bahuntur_Skill.h"
#include "GameInstance.h"
#include "Pool.h"
#include "Model.h"
#include "Player.h"
#include "PartObject.h"
#include "Esther.h"
#include "Esther_Bahuntur_Cut.h"

#include "Esther_Bahuntur_Skill_Ceiling.h"
#include "Esther_Bahuntur_Skill_Floor.h"
#include "Esther_Scene.h"

#include "Player_Controller.h"

#include "Effect_Manager.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"

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
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther_Bahuntur_Skill::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_strObjectTag = TEXT("Esther_Bahuntur_Skill");

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	if (FAILED(Ready_ModelPart()))
		return E_FAIL;

	if (FAILED(Ready_Projectile()))
		return E_FAIL;

	m_pSkillFloor = static_cast<CEsther_Bahuntur_Skill_Floor*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Bahuntur_Skill_Floor")));
	if (nullptr == m_pSkillFloor)
		return E_FAIL;

	m_pSkillCeiling = static_cast<CEsther_Bahuntur_Skill_Ceiling*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Bahuntur_Skill_Ceiling")));
	if (nullptr == m_pSkillCeiling)
		return E_FAIL;

	m_iAnimIndex = m_pModelCom->Initailize_FindAnimation(L"sk_breathofarcturus", 1.f);
	if (m_iAnimIndex == -1)
		return E_FAIL;

	m_pModelCom->Set_CurrAnim(m_iAnimIndex);
	m_pModelCom->Set_Enforce_CurrAnimFrame(m_pModelCom->Get_Anim_MaxFrame(m_iAnimIndex));
	m_pModelCom->Play_Animation(0.0f);

	m_bCutStart = false;

	return S_OK;
}

void CEsther_Bahuntur_Skill::Tick(_float fTimeDelta)
{
	Check_Finish();

	if (true == m_IsFinished)
		return;

	Cut_Start(fTimeDelta);

	Act1(fTimeDelta);
	Act2(fTimeDelta);
	Act3(fTimeDelta);
	Act4(fTimeDelta);
	Effect(fTimeDelta);

	__super::Tick(fTimeDelta);
	m_pSkillCeiling->Tick(fTimeDelta);
	m_pSkillFloor->Tick(fTimeDelta);
}

void CEsther_Bahuntur_Skill::LateTick(_float fTimeDelta)
{
	if (true == m_IsFinished)
		return;

	if (true == m_pModelCom->Is_AnimationEnd(m_iAnimIndex))
	{
		m_fAnimationSpeed = 0.0f;
	}

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


	for (_int i = 0; i < 3; ++i)
		m_bEffectStart[i] = false;

	m_bEndStart = false;
	m_bCutStart = false;
	m_bProjStart = false;
	m_IsFinished = false;

	m_bSound = false;
	m_bSound1 = false;
	m_bSound2 = false;
	m_bSound3 = false;
}

void CEsther_Bahuntur_Skill::Cut_Start(_float fTimeDelta)
{
	if (false == m_bSound)
	{
		CSound_Manager::GetInstance()->PlaySoundFile(TEXT("Effect"), TEXT("WWISEDEFAULTBANK_PC_COMMON_ESTHER#75 (247165555).wav"), 0.7f);
		m_bSound = true;
	}

	if (25 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) && false == m_bCutStart)
	{
		Add_Sound(L"Effect", L"WWISEDEFAULTBANK_PC_COMMON_ESTHER [133]");
		m_pOwnerEsther->Get_Esther_Scene()->Play_Frame();
		m_bCutStart = true;

		if (false == m_bSound1)
		{
			CSound_Manager::GetInstance()->PlaySoundFile(TEXT("Effect"), TEXT("WWISEDEFAULTBANK_PC_COMMON_ESTHER#74 (977445373).wav"), 0.7f);
			m_bSound1 = true;
		}
	}

}


void CEsther_Bahuntur_Skill::Act1(_float fTimeDelta)
{
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

	if (50 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		if (false == m_bSound2)
		{
			CSound_Manager::GetInstance()->PlaySoundFile(TEXT("Effect"), TEXT("WWISEDEFAULTBANK_PC_COMMON_ESTHER#11 (307790669).wav"), 0.7f);
			m_bSound2 = true;
		}
	}

	if (70 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		if (false == m_bSound3)
		{
			CSound_Manager::GetInstance()->PlaySoundFile(TEXT("Effect"), TEXT("WWISEDEFAULTBANK_PC_COMMON_ESTHER#12 (364428403).wav"), 0.7f);
				m_bSound3 = true;
		}
	}

	if (73 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) 
		&& 78 > m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		m_pSkillCeiling->Call_Act2(fTimeDelta);
	}

}

void CEsther_Bahuntur_Skill::Act3(_float fTimeDelta)
{
	if (70 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) && false == m_bProjStart)
	{
		{
			CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
			m_vecSkillProjDesces[0].vAttackPos = Vec3();
			m_vecSkillProjDesces[0].AttackMatrix = m_pTransformCom->Get_WorldMatrix();
			pSkill->InitProjectile(&m_vecSkillProjDesces[0]);
		}
		
		{
			CProjectile* pSkill = CPool<CProjectile>::Get_Obj();
			m_vecSkillProjDesces[1].vAttackPos = Vec3();
			m_vecSkillProjDesces[1].AttackMatrix = m_pTransformCom->Get_WorldMatrix();
			pSkill->InitProjectile(&m_vecSkillProjDesces[1]);
		}


		m_bProjStart = true;
	}
}

void CEsther_Bahuntur_Skill::Act4(_float fTimeDelta)
{
	if (105 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex) && false == m_bEndStart)
	{
		m_pSkillCeiling->Call_Act3(fTimeDelta);
		m_pSkillFloor->Call_Act1(fTimeDelta);


		m_bEndStart = true;
	}
}

void CEsther_Bahuntur_Skill::Effect(_float fTimeDelta)
{
	if (false == m_bEffectStart[0])
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		Matrix matBahunturPivot = Get_TransformCom()->Get_WorldMatrix();
		Matrix& matFloorPivot = m_pSkillFloor->Get_TransformCom()->Get_WorldMatrix();
		matBahunturPivot.Translation(matFloorPivot.Translation());

		tDesc.pPivotMatrix = &matBahunturPivot;
		EFFECT_START(TEXT("EstherSkill_Bahuntur"), &tDesc)

		m_bEffectStart[0] = true;
	}

	if (false == m_bEffectStart[1] && 10 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		Matrix& matBahunturPivot = Get_TransformCom()->Get_WorldMatrix();

		tDesc.pPivotMatrix = &matBahunturPivot;
		EFFECT_START(TEXT("EstherSkill_Bahuntur_Appear"), &tDesc)

		m_bEffectStart[1] = true;
	}

	if (false == m_bEffectStart[2] && 73 <= m_pModelCom->Get_Anim_Frame(m_iAnimIndex))
	{
		CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();
		Vec3 vEffectCenter = m_pSkillFloor->Get_TransformCom()->Get_WorldMatrix().Translation();
		pCamera->Set_RadialBlur(0.7f, vEffectCenter, 0.1f, 0.12f);
		pCamera->Set_Chromatic(0.12f, vEffectCenter, 0.15f, 0.12f);

		m_bEffectStart[2] = true;
	}
}

void CEsther_Bahuntur_Skill::Check_Finish()
{
	if (false == m_pSkillCeiling->Is_Active() && false == m_pSkillFloor->Is_Active())
	{
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
	{
		PROJECTILE_DESC Proj_Desc;
		Proj_Desc.pAttackOwner = this;
		Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
		Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_BUFF_ESTHER;
		Proj_Desc.fAttackTime = 0.1f;
		Proj_Desc.fRadius = 20.f;
		Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.1f);
		Proj_Desc.iStatusEffect = (_uint)CPlayer_Controller::BUFFEFFECT::HALFDAMAGE;
		Proj_Desc.fRepulsion = 0.01f;
		Proj_Desc.fStatusDuration = 20.f;
		m_vecSkillProjDesces.push_back(Proj_Desc);
	}
	
	{
		PROJECTILE_DESC Proj_Desc;
		Proj_Desc.pAttackOwner = this;
		Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
		Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_BUFF_ESTHER;
		Proj_Desc.fAttackTime = 0.1f;
		Proj_Desc.fRadius = 20.f;
		Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.1f);
		Proj_Desc.iStatusEffect = (_uint)CPlayer_Controller::BUFFEFFECT::STIIFIMMUNE;
		Proj_Desc.fRepulsion = 0.f;
		Proj_Desc.fStatusDuration = 20.f;
		m_vecSkillProjDesces.push_back(Proj_Desc);
	}

	return S_OK;
}

CEsther_Bahuntur_Skill* CEsther_Bahuntur_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEsther_Bahuntur_Skill* pInstance = new CEsther_Bahuntur_Skill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CIT_GN_Body_Legend");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Bahuntur_Skill::Clone(void* pArg)
{
	CEsther_Bahuntur_Skill* pInstance = new CEsther_Bahuntur_Skill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEsther_Bahuntur_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
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
