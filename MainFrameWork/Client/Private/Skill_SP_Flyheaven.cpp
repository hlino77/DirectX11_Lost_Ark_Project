#include "stdafx.h"
#include "..\Public\Skill_SP_Flyheaven.h"
#include "GameInstance.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Projectile.h"
#include "Renderer.h"
#include "Shader.h"
#include "Model.h"
#include "AsUtils.h"

CSkill_SP_Flyheaven::CSkill_SP_Flyheaven(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Doaga* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_SP_Flyheaven"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_SP_Flyheaven::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 두루미나래 */
	m_strSkill_Name = TEXT("Skill_SP_Flyheaven");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NEUTRAL;;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 1.5f;
	Proj_Desc.fRadius = 1.f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.6f, 0.4f);
	Proj_Desc.IsMove = true;
	Proj_Desc.fMoveSpeed = 5.2f;
	Proj_Desc.iDamage = 300;
	Proj_Desc.iStagger = 70;
	Proj_Desc.iStatusEffect = (_uint)STATUSEFFECT::STUN;
	Proj_Desc.fStatusDuration = 2.f;
	Proj_Desc.fRepulsion = 0.f;

	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	m_iAnimIndex = m_pModelCom->Initailize_FindAnimation(L"sk_flyinheaven", 1.f);
	if (m_iAnimIndex == -1)
		return E_FAIL;

	m_pModelCom->Set_CurrAnim(m_iAnimIndex);
	m_pModelCom->Play_Animation(0.0f);

	return S_OK;
}

void CSkill_SP_Flyheaven::Tick(_float fTimeDelta)
{
	if (true == m_pModelCom->Is_AnimationEnd(m_iAnimIndex))
	{
		Exit();
	}


	m_fSpeed = m_tLerp.Update_Lerp(fTimeDelta);

	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta);
}

void CSkill_SP_Flyheaven::LateTick(_float fTimeDelta)
{
	if (m_PlayAnimation.valid())
		m_PlayAnimation.get();

	m_pTransformCom->Go_Straight(m_fSpeed, fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ALPHABLEND, this);
}

HRESULT CSkill_SP_Flyheaven::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return S_OK;

	_float fRimLight = (_float)m_bRimLight;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLight, sizeof(_float))))
		return E_FAIL;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	m_pModelCom->Render_Outline(m_pShaderCom);

	return S_OK;
}

void CSkill_SP_Flyheaven::Enter()
{
	__super::Enter();

	Vec3 vLook = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
	vLook.Normalize();

	Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	vPos += vLook * 0.5f;
	m_pTransformCom->Set_WorldMatrix(m_pPlayer->Get_TransformCom()->Get_WorldMatrix());
	m_pTransformCom->Get_TransformCom()->Set_Scale(Vec3(0.7f, 0.7f, 0.7f));
	m_pTransformCom->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);

	m_pModelCom->Set_CurrAnim(m_iAnimIndex);

	m_tLerp.Init_Lerp(1.f, m_fStartSpeed, m_fTargetSpeed, LERP_MODE::EASE_IN);
}

void CSkill_SP_Flyheaven::Exit()
{
	__super::Exit();
}

void CSkill_SP_Flyheaven::Check_ColliderState()
{
}

HRESULT CSkill_SP_Flyheaven::Ready_Components()
{
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
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
	wstring strComName = L"Prototype_Component_Model_SK_SDM_FH";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Yinyanshi_Crane_Wing"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

CSkill_SP_Flyheaven* CSkill_SP_Flyheaven::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Doaga* pPlayer, void* pArg)
{
	CSkill_SP_Flyheaven* pInstance = new CSkill_SP_Flyheaven(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_SP_Flyheaven");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_SP_Flyheaven::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_SP_Flyheaven::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}

