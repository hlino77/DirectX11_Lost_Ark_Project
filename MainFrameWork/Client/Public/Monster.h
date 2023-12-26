#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "StateMachine.h"
#include <atomic>

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CBehaviorTree;
class CRenderer;
class CTransform;
class CPipeLine;
class CSphereCollider;
class CHierarchyNode;
class CCollider;
END


BEGIN(Client)
class CMonster : public CGameObject
{
public:
	typedef struct MODELDESC
	{
		wstring strFileName;
		_int	iObjectID;
		_uint	iLayer;
		Vec3	vPos;
		_bool	bInstance = false;
	}MODELDESC;

public:


protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Tick(_float fTimeDelta) override;
	virtual void			LateTick(_float fTimeDelta) override;
	virtual HRESULT			Render() override;
	virtual HRESULT			Render_ShadowDepth() override;
	virtual HRESULT			Render_ShadowDepth_Instance(_uint iSize) override;
	virtual HRESULT			Render_Debug() override;

	virtual HRESULT	Render_Instance(_uint iSize) override;
	virtual void	Add_InstanceData(_uint iSize, _uint& iIndex) override;

	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

	void	 Update_StatusEffect(_float fTimeDelta);

	void Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration);


	virtual void Set_SlowMotion(_bool bSlow) override;


	void					Send_Collision(_uint iDamage, Vec3 vHitPos, STATUSEFFECT eEffect, _float fForce, _float fDuration);
public:
	void					Find_NearTarget();
	CShader*				Get_ShaderCom() { return m_pShaderCom; }
	void					Set_MoveSpeed(_float fSpeed) { m_fMoveSpeed = fSpeed; }
	_float					Get_MoveSpeed() { return m_fMoveSpeed; }
	void					Add_MoveSpeed(_float fSpeed, _float fMaxSpeed) { m_fMoveSpeed += fSpeed; m_fMoveSpeed = min(m_fMoveSpeed, fMaxSpeed); }
	_bool					Is_Control() { return m_bControl; }
public:
	void					Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame);
	void					Follow_ServerPos(_float fDistance, _float fLerpSpeed);
	void					Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta);
	_float					Get_Target_Distance();
	void					LookAt_Target_Direction_Lerp(_float fTimeDelta);
	void					LookAt_Target_Direction();
	Vec3					Get_Target_Direction();

	void					Move_to_RandomPosition(_float fTimeDelta);
	_bool					Is_Close_To_RandomPosition();
	virtual void			Set_Die();
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

	void					Set_AttackRange(_int iRangeIndex);
	_float					Get_AttackRange() { return m_fAttackRange; }

	void					Set_NoticeRange(_float fNoticeRange) { m_fNoticeRange = fNoticeRange; }
	_float					Get_NoticeRange() { return m_fNoticeRange; }

	void					Set_Colliders(_float fTimeDelta);
	void					Set_Collider_Active(_uint eColliderType,_bool IsActive);
	void					Set_Collider_Info(_uint eColliderType, Vec3 _vCenter, _float fRadius);

	_uint					Get_BasicAttackStartFrame() { return m_iBasicAttackStartFrame; }
	_uint					Get_BasicAttackEndFrame() { return m_iBasicAttackEndFrame; }

	Vec3 Get_RandomPosition() { return m_vRandomPosition; }
	void Set_RandomPosition(Vec3 vPos) {m_vRandomPosition = vPos; }

	void Set_RootTargetDistance(_float fDistance) { m_fRootTargetDistance = fDistance; }


	void					Set_RimLight(_float fTime) { m_bRimLight = true; m_fRimLightTime = fTime; }

	void					Show_Damage(_uint iDamage);
protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_BehaviourTree();
	HRESULT Ready_HP_UI();

	virtual HRESULT	Ready_Proto_InstanceBuffer() override;
	virtual HRESULT	Ready_Instance_For_Render(_uint iSize) override;
	HRESULT	Ready_AnimInstance_For_Render(_uint iSize);

protected:
	void					CullingObject();
	void					Set_to_RootPosition(_float fTimeDelta, _float _TargetDistance= 0.f);

protected:
	_float							m_fMoveSpeed = 0.0f;
	_float							m_fAttackMoveSpeed = 0.0f;
	_float							m_fAnimationSpeed = 1.0f;
	_float							m_fScanCoolDown = 0.f;
	vector<_float>					m_vecAttackRanges;
	_float							m_fAttackRange = 1.f;
	_float							m_fNoticeRange = 20.f;
	_float							m_fTimeCount = 0.f;
	_uint							m_iBasicAttackStartFrame = 0;
	_uint							m_iBasicAttackEndFrame = 0;
	_bool							m_IsHit = false;
	_bool							m_IsLeft = false;
	_bool							m_IsSpawn = true;
	_bool							m_IsSuperArmor = false;
	Vec3							m_vRandomPosition = {};
	wstring							m_strAction = L"";
	unordered_map<wstring, _uint>	m_BoneIndex;
	_float							m_fRootTargetDistance =0.f;
	_float							m_fFontScale=0.4f;
	_bool							m_bRimLight = false;
	_float							m_fRimLightTime = 0.0f;

protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CRenderer* m_pRendererCom = nullptr;
	CBehaviorTree* m_pBehaviorTree = nullptr;

	
	std::future<HRESULT>			m_PlayAnimation;
	//상태이상
	_float							m_fStatusEffects[(_uint)STATUSEFFECT::EFFECTEND] = {};
	//Culling
	BoundingSphere m_tCullingSphere;

	atomic<_int>					m_iSlowMotionCount = 0;
public:
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

