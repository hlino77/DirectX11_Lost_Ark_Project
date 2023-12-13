#pragma once

#include "Server_Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include <atomic>

BEGIN(Engine)
class CTransform;
class CSphereCollider;
class CCollider;
class CRenderer;
class CModel;
class CBehaviorTree;
END


BEGIN(Server)
class CMonster_Server : public CGameObject
{
public:
	typedef struct ModelDesc
	{
		wstring strFileName;
		_int	iObjectID;
		_uint	iLayer;
	}MODELDESC;

public:


protected:
	CMonster_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Server(const CMonster_Server& rhs);
	virtual ~CMonster_Server() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void Set_SlowMotion(_bool bSlow) override;

	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;
public:


	void					Set_MoveSpeed(_float fSpeed) { m_fMoveSpeed = fSpeed; }
	_float					Get_MoveSpeed() { return m_fMoveSpeed; }
	void					Add_MoveSpeed(_float fSpeed, _float fMaxSpeed) { m_fMoveSpeed += fSpeed; m_fMoveSpeed = min(m_fMoveSpeed, fMaxSpeed); }

	void					Set_FollowDistance(_float fDistance) { m_fFollowDistance = fDistance; }
	_float					Get_FollowDistance() { return m_fFollowDistance; }
	
	void					Send_Collision(const _uint iColLayer, CCollider* pOther, _bool bEnter);

	
public:


	void					Find_NearTarget();

	void					Send_NearTarget();

	_float					Get_NearTargetDistance();

	Vec3 Get_Target_Direction();

	void Set_RandomPosition();

	void Move_to_RandomPosition(_float fTimeDelta);

	_bool Is_Close_To_RandomPosition();

	void LookAt_Target_Direction(_float fTimeDelta);

	//Send Packet
	void				Send_State(const wstring& szName);
	void				Set_State(const wstring& szName);
	void				Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame);
	void				Send_ColliderState(const _uint& iLayer);
	void				Send_SlowMotion(_bool bSlow);
	void				Send_Hp();
	void				Send_Monster_Action();

	void				Set_Colliders(_float fTimeDelta);
	HRESULT				Ready_Coliders();


	void				Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta);

	void				Body_Collision(CGameObject* pObject);
	void				Hit_Attack(CCollider* pCollider);

	virtual void				Set_Die();
public:
	_bool						Is_Hit() { return m_IsHit; }
	void						Set_Hit(_bool bHit) { m_IsHit = bHit; }

	_bool						Is_Left() { return m_IsLeft; }
	void						Set_Left(_bool IsLeft) { m_IsLeft = IsLeft; }

	_bool						Is_Spawn() { return m_IsSpawn; }
	void						Set_Spawn(_bool IsSpawn) { m_IsSpawn = IsSpawn; }

	void					Set_AnimationSpeed(_float fAnimationSpeed) { m_fAnimationSpeed = fAnimationSpeed; }
	_float					Get_AnimationSpeed() { return m_fAnimationSpeed; }
	void					Set_Action(wstring strAction) { m_strAction = strAction; }
	wstring					Get_Action() { return m_strAction; }
protected:
	virtual HRESULT		Ready_Components();



protected:
	wstring							m_strAction = L"";
	CRenderer*						m_pRendererCom = nullptr;
	CBehaviorTree*					m_pBehaviorTree = nullptr;
	_float							m_fScanCoolDown = 0.f;
	std::future<HRESULT>			m_PlayAnimation;
	_float							m_fMoveSpeed = 0.0f;
	_float							m_fAttackMoveSpeed = 0.0f;
	_float							m_fAnimationSpeed = 1.0f;

	unordered_map<wstring, _uint>	m_BoneIndex;


	_float							m_fFollowDistance = 0.0f;


	atomic<_int>					m_iSlowMotionCount = 0;

	_bool							m_IsHit = false;
	_bool							m_IsLeft = false;
	_bool							m_IsSpawn = true;
	Vec3							m_vRandomPosition = {};
protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */


public:
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

