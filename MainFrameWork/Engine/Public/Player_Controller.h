#pragma once
#include "Component.h"
#include "Transform.h"
#include "RigidBody.h"

BEGIN(Engine)

class ENGINE_DLL CPlayer_Controller abstract : public CComponent
{
public:
	typedef struct tagControllerDesc
	{
		CGameObject*	pOwner = { nullptr };
		CTransform*		pOwnerTransform = { nullptr };
		CRigidBody*		pOwnerRigidBody = { nullptr };
	}CONTROLL_DESC;

protected:
	CPlayer_Controller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Controller(const CPlayer_Controller& rhs);
	virtual ~CPlayer_Controller() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual void	DebugRender();

public:
	_bool	Is_Idle();
	_bool	Is_Run();
	_bool	Is_Skill();
	_bool	Is_Interect();
	_bool	Is_Dash();
	_bool	Is_Attack();

	void	Get_MoveMessage(Vec3 vPos) { m_vNextMove = vPos; }
	void	Get_AttackMessage() { Attack(); }
	void	Get_SkillMessage() { Skill(); }
	void	Get_HitMessage() { Hit(); }

public:
	_bool	Is_Stop() { return m_bMoveStop; }

public:
	/* 언젠가는 쓰겠지 */
	_bool	Pick(_uint screenX, _uint screenY, Vec3 & pickPos, _float & distance);
	void	Look(const Vec3 & vPoint, const _float & fTimeDelta);

protected:
	void			Move(const _float& fTimeDelta);
	virtual void	Input(const _float & fTimeDelta);
	virtual void	Attack();
	virtual void	Skill();
	virtual void	Hit();

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	CGameObject*			m_pOwner = nullptr;
	CTransform*				m_pOwnerTransform = nullptr;
	CRigidBody*				m_pOwnerRigidBody = nullptr;

	Vec3					m_vPrePos;
	Vec3					m_vNextMove;
	_bool					m_bMoveStop = { false };

	Vec3					m_vMaxLinearSpeed;
	Vec3					m_vLinearSpeed;
	Vec3					m_vMaxAngularSpeed;
	Vec3					m_vAngularSpeed;

public:
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END