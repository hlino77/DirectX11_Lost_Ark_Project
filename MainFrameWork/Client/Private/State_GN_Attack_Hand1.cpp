#include "stdafx.h"
#include "..\Public\State_GN_Attack_Hand1.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Pool.h"
#include "Projectile.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "Sound_Manager.h"


CState_GN_Attack_Hand1::CState_GN_Attack_Hand1(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Attack_Hand1::Initialize()
{
	m_Attack_Hand1 = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"att_battle_1_01", 1.4f);
	if (m_Attack_Hand1 == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Attack_Hand1::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Attack_Hand1::Tick_State_NoneControl;


	/* 일반공격 프레임 */
	m_AttackFrames.push_back(4);
	m_AttackFrames.push_back(8);
	m_AttackFrames.push_back(-1);

	m_EffectFrames.push_back(EFFECTFRAMEDESC(4, (_uint)CPartObject::PARTS::WEAPON_2));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(8, (_uint)CPartObject::PARTS::WEAPON_1));
	m_EffectFrames.push_back(EFFECTFRAMEDESC());

	return S_OK;
}

void CState_GN_Attack_Hand1::Enter_State()
{
	if (true == m_pPlayer->Is_Control())
	{
		CSound_Manager::GetInstance()->PlaySoundFile(TEXT("Effect"), TEXT("GN_Laugh_605"), 1.f);
	}

	m_iAttackCnt = 0;
	m_iEffectCnt = 0;

	m_pPlayer->Reserve_Animation(m_Attack_Hand1, 0.1f, 0, 0);
	m_pController->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	m_IsAttackContinue = false;
}

void CState_GN_Attack_Hand1::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Attack_Hand1::Exit_State()
{
}

void CState_GN_Attack_Hand1::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand1);

	if (-1 != m_AttackFrames[m_iAttackCnt] && m_AttackFrames[m_iAttackCnt] <= iAnimFrame)
	{
		m_iAttackCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_AttackMessage();
	}

	if (-1 != m_EffectFrames[m_iEffectCnt].iFrame && m_EffectFrames[m_iEffectCnt].iFrame <= iAnimFrame)
	{
		Effect_Shot();
		m_iEffectCnt++;
	}

	if (true == m_pController->Is_Attack() &&
		15 > iAnimFrame &&
		5 <= iAnimFrame)
	{
		m_IsAttackContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_Attack_Hand1))
		m_pPlayer->Set_State(TEXT("Idle"));

	_uint iIdentity = static_cast<CPlayer_Controller_GN*>(m_pController)->Is_GN_Identity();

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (0 != iIdentity)
	{
		if (1 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_GN"));
		else if (2 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_GN_Back"));
	}
	else if (true == m_pController->Is_Skill())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		CPlayer_Controller::SKILL_KEY eKey = m_pController->Get_Selected_Skill();
		m_pPlayer->Set_State(m_pController->Get_SkillStartName(eKey));
	}
	else if (true == m_IsAttackContinue && 15 <= iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Attack_Hand_2"));
	}
	else if (true == m_pController->Is_Run() && 15 < iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			m_pPlayer->Set_TargetPos(vClickPos);
			m_pPlayer->Set_State(TEXT("Run"));
		}
	}
}

void CState_GN_Attack_Hand1::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_Attack_Hand1);

	if (-1 != m_EffectFrames[m_iEffectCnt].iFrame && m_EffectFrames[m_iEffectCnt].iFrame <= (_int)iAnimFrame)
	{
		Effect_Shot();
		m_iEffectCnt++;
	}
}

void CState_GN_Attack_Hand1::Attack_Hand(_float fAttackTime)
{
	CProjectile* pAttack = CPool<CProjectile>::Get_Obj();
	pAttack->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION));
	CSphereCollider* pCollider = pAttack->Get_Colider(CProjectile::ATTACKCOLLIDER::OBB);
	pCollider->Set_ColLayer((_uint)LAYER_COLLIDER::LAYER_ATTACK_PLAYER);
	pCollider->Set_Radius(2.f);
	pCollider->Set_Offset(Vec3(0.0f, 0.2f, 1.7f));
	COBBCollider* pChildCollider = static_cast<COBBCollider*>(pCollider->Get_Child());
	pChildCollider->Set_Scale(Vec3(0.3f, 0.6f, 1.5f));
	pChildCollider->Set_Offset(Vec3(0.0f, 0.6f, 1.7f));


}

void CState_GN_Attack_Hand1::Effect_Shot()
{

	CEffect_Manager::EFFECTPIVOTDESC desc;
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	Vec3 vPos = static_cast<CPartObject*>(m_pPlayer->Get_Parts((CPartObject::PARTS)m_EffectFrames[m_iEffectCnt].iWeapon))->Get_Part_WorldMatrix().Translation();

	Vec3 vOriginLook = matWorld.Backward();
	vOriginLook.Normalize();

	Vec3 vOriginUp = matWorld.Up();
	vOriginUp.Normalize();

	Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
	vOriginRight.Normalize();

	matWorld.Translation(vPos);
	desc.pPivotMatrix = &matWorld;
	EFFECT_START(TEXT("HandBullet"), &desc)

	_uint iCount = rand() % 2 + 1;
	for(_uint i = 0; i < iCount; ++i)
	{
		_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);
		_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);


		Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

		Matrix matEffectWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matEffectWorld;

		EFFECT_START(TEXT("HandBulletParticle"), &desc)
	}
}

CState_GN_Attack_Hand1* CState_GN_Attack_Hand1::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Attack_Hand1* pInstance = new CState_GN_Attack_Hand1(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Attack_Hand1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Attack_Hand1::Free()
{
	__super::Free();
}
