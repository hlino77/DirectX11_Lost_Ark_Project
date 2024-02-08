#include "stdafx.h"
#include "..\Public\Skill_WDR_EndurePain.h"
#include "Player_Destroyer.h"
#include "Projectile.h"
#include "Effect_Manager.h"
#include "Camera_Player.h"
#include "Effect.h"

CSkill_WDR_EndurePain::CSkill_WDR_EndurePain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Destroyer* pPlayer)
	: CPlayer_Skill(pDevice, pContext, TEXT("Skill_WDR_EndurePain"), OBJ_TYPE::SKILL), m_pPlayer(pPlayer)
{
}

HRESULT CSkill_WDR_EndurePain::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strSkill_Name = TEXT("Skill_WDR_EndurePain");

	m_fSkillDamage = 0.f;
	m_fSkillCoolTime = 3.f;

	m_eAttackType = SKILL_ATTACKTYPE::NORMAL;;
	m_eCtrlType = SKILL_CTRLTYPE::NORMAL;
	m_IsSuperArmor = true;

	PROJECTILE_DESC Proj_Desc;
	Proj_Desc.pAttackOwner = m_pOwner;
	Proj_Desc.eUseCollider = (_uint)CProjectile::ATTACKCOLLIDER::SPHERE;
	Proj_Desc.eLayer_Collider = (_uint)LAYER_COLLIDER::LAYER_SKILL_PLAYER;
	Proj_Desc.fAttackTime = 0.5f;
	Proj_Desc.fRadius = 1.3f;
	Proj_Desc.vOffset = Vec3(0.0f, 0.2f, 0.0f);
	Proj_Desc.iDamage = 1200;
	Proj_Desc.iStagger = 10;
	Proj_Desc.fRepulsion = 1.f;
	Proj_Desc.bUseProjPos = true;
	m_vecSkillProjDesces.push_back(Proj_Desc);
	m_SkillProjDesc = Proj_Desc;

	m_iUseMana = 114;

	return S_OK;
}

void CSkill_WDR_EndurePain::Tick(_float fTimeDelta)
{
}

void CSkill_WDR_EndurePain::LateTick(_float fTimeDelta)
{
}

HRESULT CSkill_WDR_EndurePain::Render()
{
	return S_OK;
}

HRESULT CSkill_WDR_EndurePain::Ready_Components()
{
	if (false == m_pPlayer->Is_Control())
		return S_OK;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Destroyer_Endure_Pain"),
		TEXT("Com_Texture"), (CComponent**)&m_pSkillTextureCom)))
		return E_FAIL;

	return S_OK;
}

_float CSkill_WDR_EndurePain::Change_Player_Status()
{
	m_pPlayer->Set_SuperiorArmorState(true);
	
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &matWorld;

	vector<CEffect*> Effects;

	EFFECT_START(L"EndurePain", &tDesc);
	EFFECT_START_OUTLIST(L"EndurePain1", &tDesc, Effects);
	
	CEffect* pEffect = Effects.front();

	m_pPlayer->Add_Effect(L"EndurePain", pEffect);
	if (m_pPlayer->Is_Control())
	{
		m_pPlayer->Get_Camera()->Set_RadialBlur(0.5f, matWorld.Translation(), 0.1f, 0.1f);
		m_pPlayer->Get_Camera()->Cam_Shake(0.15f, 80.0f, 1.0f, 5.0f);
		m_pPlayer->Get_Camera()->Set_Chromatic(0.5f, matWorld.Translation(), 0.1f, 0.1f);
	}
		

	pEffect->CB_UpdatePivot += bind(&CPlayer::Load_WorldMatrix, m_pPlayer, placeholders::_1);
	pEffect->Set_Trace(true);

	return 4.f;
}

_float CSkill_WDR_EndurePain::Restore_Player_Status()
{
	m_pPlayer->Set_SuperiorArmorState(false);
	m_pPlayer->Delete_Effect(L"EndurePain");

	return -1;
}

void CSkill_WDR_EndurePain::Check_ColliderState()
{
}

CSkill_WDR_EndurePain* CSkill_WDR_EndurePain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CPlayer_Destroyer* pPlayer, void* pArg)
{
	CSkill_WDR_EndurePain* pInstance = new CSkill_WDR_EndurePain(pDevice, pContext, pPlayer);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSkill_WDR_EndurePain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkill_WDR_EndurePain::Clone(void* pArg)
{
	return nullptr;
}

void CSkill_WDR_EndurePain::Free()
{
	__super::Free();
}

