#include "Player_Controller.h"
#include "GameInstance.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Engine_Defines.h"

CPlayer_Controller::CPlayer_Controller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CPlayer_Controller::CPlayer_Controller(const CPlayer_Controller& rhs)
	: CComponent(rhs)
{
}

HRESULT CPlayer_Controller::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Controller::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		CONTROLL_DESC* pCtrlDesc = (CONTROLL_DESC*)pArg;
		m_pOwner = pCtrlDesc->pOwner;

		m_pOwnerTransform = pCtrlDesc->pOwnerTransform;

		m_pOwnerRigidBody = pCtrlDesc->pOwnerRigidBody;
	}

	return S_OK;
}

void CPlayer_Controller::Tick(_float fTimeDelta)
{
	Move(fTimeDelta);
}

void CPlayer_Controller::LateTick(_float fTimeDelta)
{
	m_vPrePos = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
}

void CPlayer_Controller::DebugRender()
{
}

_bool CPlayer_Controller::Is_Idle()
{
	if (false == Is_Run() && false == Is_Skill() && false == Is_Attack() && false == Is_Dash())
		return true;

	return false;
}

_bool CPlayer_Controller::Is_Run()
{
	if (KEY_HOLD(KEY::RBTN) || KEY_TAP(KEY::RBTN))
	{
		return true;
	}
		
	return false;
}

_bool CPlayer_Controller::Is_Skill()
{
	if (KEY_HOLD(KEY::Q) || KEY_TAP(KEY::Q) || KEY_HOLD(KEY::W) || KEY_TAP(KEY::W) ||
		KEY_HOLD(KEY::E) || KEY_TAP(KEY::E) || KEY_HOLD(KEY::R) || KEY_TAP(KEY::R) ||
		KEY_HOLD(KEY::A) || KEY_TAP(KEY::A) || KEY_HOLD(KEY::S) || KEY_TAP(KEY::S) ||															  
		KEY_HOLD(KEY::D) || KEY_TAP(KEY::D) || KEY_HOLD(KEY::F) || KEY_TAP(KEY::F))
			return true;

	return false;
}

_bool CPlayer_Controller::Is_Interect()
{
	if (KEY_TAP(KEY::G))
		return true;

	return false;
}

_bool CPlayer_Controller::Is_Dash()
{
	if (KEY_TAP(KEY::SPACE))
		return true;

	return false;
}

_bool CPlayer_Controller::Is_Attack()
{
	if (KEY_HOLD(KEY::LBTN) || KEY_TAP(KEY::LBTN))
		return true;

	return false;
}

_bool CPlayer_Controller::Pick(_uint screenX, _uint screenY, Vec3& pickPos, _float& distance)
{
	return true;
}

void CPlayer_Controller::Look(const Vec3& vPoint, const _float& fTimeDelta)
{
}

void CPlayer_Controller::Input(const _float& fTimeDelta)
{
}

void CPlayer_Controller::Move(const _float& fTimeDelta)
{
	if (Vec3(m_vNextMove - m_vPrePos).Length() <= 0.03f)
	{
		m_bMoveStop = true;
		return;
	}
	else
		m_bMoveStop = false;
		

	Vec3 vPos = m_pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	Vec3 vDir = m_vNextMove - vPos;
	m_pOwnerTransform->Move_ToPos(vDir, 10.f, 3.f, fTimeDelta);
}

void CPlayer_Controller::Attack()
{
}

void CPlayer_Controller::Skill()
{
}

void CPlayer_Controller::Hit()
{
}

CComponent* CPlayer_Controller::Clone(CGameObject* pGameObject, void* pArg)
{
	return nullptr;
}

void CPlayer_Controller::Free()
{
	__super::Free();
}
