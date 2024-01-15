#include "stdafx.h"
#include "Effect_Custom_DeathFireBomb.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"
#include "Camera_Player.h"

CEffect_Custom_DeathFireBomb::CEffect_Custom_DeathFireBomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Custom_DeathFireBomb::CEffect_Custom_DeathFireBomb(const CEffect_Custom_DeathFireBomb& rhs)
	: Super(rhs)
{
}

HRESULT CEffect_Custom_DeathFireBomb::Initialize_Prototype(EFFECTDESC* pDesc)
{
	return S_OK;
}

HRESULT CEffect_Custom_DeathFireBomb::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	DeathFireBombDesc* pDesc = static_cast<DeathFireBombDesc*>(pArg);
	m_pTransformCom->Turn_Axis(m_pTransformCom->Get_State(CTransform::STATE_LOOK), XM_PI * 0.5f);
	m_pTransformCom->Turn_Axis(Vec3(0.0f, 1.0f, 0.0f), m_pGameInstance->Random_Float(0.0f, XM_PI * 2.0f));
	m_pTransformCom->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, pDesc->vPos);
	m_vStartPos = pDesc->vPos;
	m_vTargetPos = pDesc->vTargetPos;
	m_bFirst = pDesc->bFirst;

	m_bAttackStart = false;

	m_fLifeTime = 2.0f;
	m_fTimeAcc = 0.0f;

	if (pDesc->pPlayer)
	{
		m_pPlayer = pDesc->pPlayer;
		m_iSkillKey = pDesc->iSkillKey;
	}

	Init_Projectile_Motion();

	return S_OK;
}

void CEffect_Custom_DeathFireBomb::Tick(_float fTimeDelta)
{
	m_pTransformCom->Turn_Speed(m_pTransformCom->Get_State(CTransform::STATE_UP), -10.0f, fTimeDelta);


	if (m_bAttackStart)
	{
		Effect_Explosion();
		Set_Active(false);
		Set_Dead(true);
		return;
	}
	else
	{
		Update_GrenadePos(fTimeDelta);
	}
	
}

void CEffect_Custom_DeathFireBomb::LateTick(_float fTimeDelta)
{
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	}
}

HRESULT CEffect_Custom_DeathFireBomb::Render()
{
	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}


HRESULT CEffect_Custom_DeathFireBomb::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_Effect_Custom_GN_Bomb";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	Vec3 vScale;
	vScale.y = 0.3f;
	vScale.z = 0.3f;
	vScale.x = 0.3f;

	m_pTransformCom->Set_Scale(vScale);

	return S_OK;
}

void CEffect_Custom_DeathFireBomb::Init_Projectile_Motion()
{
	_float fMaxTime = 0.2f;

	/*_float fMaxTime = 2.15f;
	_float fMaxHeight = 5.0f;*/

	Vec3 vTargetPos = m_vTargetPos;
	_float fEndHeight = vTargetPos.y - m_vStartPos.y;
	_float fHeight = 0.5f;

	m_fGravity = 2.0f * fHeight / (fMaxTime * fMaxTime);

	m_fVelocityY = sqrtf(2.0f * m_fGravity * fHeight);

	_float a = m_fGravity;
	_float b = -2.0f * m_fVelocityY;
	_float c = 2.0f * fEndHeight;

	m_fEndTime = (-b + sqrtf(b * b - 4.0f * a * c)) / (2.0f * a);

	m_fVelocityX = -(m_vStartPos.x - vTargetPos.x) / m_fEndTime;
	m_fVelocityZ = -(m_vStartPos.z - vTargetPos.z) / m_fEndTime;

	m_fTimeAcc = 0.0f;
}


void CEffect_Custom_DeathFireBomb::Update_GrenadePos(_float fTimedelta)
{
	m_fTimeAcc += fTimedelta;

	m_fTimeAcc = min(m_fTimeAcc, m_fEndTime);

	Vec3 vPos;

	vPos.x = m_vStartPos.x + m_fVelocityX * m_fTimeAcc;
	vPos.y = m_vStartPos.y + (m_fVelocityY * m_fTimeAcc) - (0.5f * m_fGravity * m_fTimeAcc * m_fTimeAcc);
	vPos.z = m_vStartPos.z + m_fVelocityZ * m_fTimeAcc;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);


	_float fDistance = (m_vTargetPos - vPos).Length();
	if (fDistance < 0.1f)
	{
		m_bAttackStart = true;
		if (m_pPlayer && m_pPlayer->Is_Control())
			m_pPlayer->Get_GN_Controller()->Get_SkillAttackMessage((CPlayer_Controller::SKILL_KEY)m_iSkillKey, m_vTargetPos);
	}
}

void CEffect_Custom_DeathFireBomb::Effect_Explosion()
{
	Matrix matWorld = XMMatrixIdentity();
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	matWorld.Translation(vPos);

	CEffect_Manager::EFFECTPIVOTDESC desc;
	desc.pPivotMatrix = &matWorld;
	EFFECT_START(TEXT("DeathFireLast"), &desc)

	if(m_bFirst && m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.2f, 700.f, 0.1f, 10.f);
}

CEffect_Custom_DeathFireBomb* CEffect_Custom_DeathFireBomb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Custom_DeathFireBomb* pInstance = new CEffect_Custom_DeathFireBomb(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(nullptr)))
	{
		MSG_BOX("Failed To Created : CEffect_Custom_Grenade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Custom_DeathFireBomb::Clone(void* pArg)
{
	CEffect_Custom_DeathFireBomb* pInstance = new CEffect_Custom_DeathFireBomb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Custom_Grenade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Custom_DeathFireBomb::Free()
{
	Super::Free();
}
