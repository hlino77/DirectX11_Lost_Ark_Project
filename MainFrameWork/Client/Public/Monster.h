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
class CUI_Monster_Hp;
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
		_uint	iLevel;
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

	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther);
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

	void	 Update_StatusEffect(_float fTimeDelta);

	virtual	void Hit_Collision(_uint iDamage, Vec3 vHitPos, _uint iStatusEffect, _float fForce, _float fDuration, _uint iGroggy);


	virtual void Set_SlowMotion(_bool bSlow) override;



public:
	CShader*				Get_ShaderCom() { return m_pShaderCom; }
	void					Set_MoveSpeed(_float fSpeed) { m_fMoveSpeed = fSpeed; }
	_float					Get_MoveSpeed() { return m_fMoveSpeed; }
	void					Add_MoveSpeed(_float fSpeed, _float fMaxSpeed) { m_fMoveSpeed += fSpeed; m_fMoveSpeed = min(m_fMoveSpeed, fMaxSpeed); }
	_bool					Is_Control() { return m_bControl; }
public:
	void					Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _uint iStartFrame, _uint iChangeFrame);
	void					Follow_ServerPos(_float fDistance, _float fLerpSpeed);
	void					Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta);
	Vec3					Get_NearTarget_Position();
	_float					Get_Target_Distance();
	void					LookAt_Target_Direction_Lerp(_float fTimeDelta);
	void					LookAt_Target_Direction();
	Vec3					Get_Target_Direction();

	Vec3 Get_Target_RandomDirection();

	_bool Is_Close_To_TargetRandomPosition();

	void					Move_to_RandomPosition(_float fTimeDelta);
	_bool					Is_Close_To_RandomPosition();
	virtual void			Set_Die(_float fTime = 1.f);

	virtual	void			Disable_HpUI();
public:
	_bool						Is_Hit() { return m_IsHit; }
	void						Set_Hit(_bool bHit) { m_IsHit = bHit; }

	_bool						Is_Left() { return m_IsLeft; }
	void						Set_Left(_bool IsLeft) { m_IsLeft = IsLeft; }

	_bool						Is_Culled() { return m_IsCulled; }

	_bool						Is_Spawn() { return m_IsSpawn; }
	void						Set_Spawn(_bool IsSpawn) { m_IsSpawn = IsSpawn; }

	void					Set_AnimationSpeed(_float fAnimationSpeed) { m_fAnimationSpeed = fAnimationSpeed; }
	_float					Get_AnimationSpeed() { return m_fAnimationSpeed; }

	void					Set_Action(wstring strAction);
	wstring					Get_Action() { return m_strAction; }

	void					Set_AttackRange(_int iRangeIndex);
	_float					Get_AttackRange() { return m_fAttackRange; }

	void					Set_NoticeRange(_float fNoticeRange) { m_fNoticeRange = fNoticeRange; }
	_float					Get_NoticeRange() { return m_fNoticeRange; }

	void					Set_Colliders(_float fTimeDelta);
	void					Set_Collider_Active(_uint eColliderType,_bool IsActive);
	void					Set_Collider_Info(_uint eColliderType, Vec3 _vCenter, _float fRadius);
	_bool					Get_Collider_Center(_uint eColliderType, Vec3* pCenter);

	_uint					Get_BasicAttackStartFrame() { return m_iBasicAttackStartFrame; }
	_uint					Get_BasicAttackEndFrame() { return m_iBasicAttackEndFrame; }
	_bool					Is_SetuponCell() { return m_IsSetuponCell; }
	void					Set_SetuponCell(_bool IsSetuponCell) { m_IsSetuponCell = IsSetuponCell; }


	Vec3 Get_RandomPosition() { return m_vRandomPosition; }
	void Set_RandomPosition(Vec3 vPos) {m_vTargetPos=  m_vRandomPosition = vPos; }

	void Set_RootTargetDistance(_float fDistance) { m_fRootTargetDistance = fDistance; }
	Vec3 Get_BonePos(wstring strBoneName);

	void					Set_RimLight(_float fTime, _float fColor) { m_bRimLight = true; m_fRimLightTime = fTime; m_fRimLightColor = fColor; }
	_bool					Get_RimLight() {	return m_bRimLight;	}

	void					Show_Damage(_uint iDamage, _bool IsCritical);
	void					Deactivate_AllColliders();

	_int					Get_Atk() { return m_iAtk; }
	void					Set_Atk(_int iAtk) { m_iAtk = iAtk; }

	void					Set_DissolveOut(_float fTime) { m_bDissolveOut = true; m_bDissolveIn = false;  m_fDissolvetime = 0; m_fMaxDissolvetime = fTime;	}
	_bool					Get_DissolveOut() { return m_bDissolveOut; }


	void					Set_DissolveIn(_float fTime) { m_bDissolveIn = true; m_bDissolveOut = false; m_fDissolvetime = m_fMaxDissolvetime = fTime;	}
	_bool					Get_DissolveIn() { return m_bDissolveIn; }

	_float					Get_Dissolvetime() { return m_fDissolvetime; }
	_float					Get_fMaxDissolvetime() { return m_fMaxDissolvetime; }

	CTexture*				Get_DissolveTexture() {	return m_pDissolveTexture;	}

protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_DissolveTexture();
	virtual HRESULT Ready_BehaviourTree();
	virtual HRESULT	Ready_Proto_InstanceBuffer() override;
	virtual HRESULT	Ready_Instance_For_Render(_uint iSize) override;
	HRESULT	Ready_AnimInstance_For_Render(_uint iSize);

protected:
	virtual	void					Update_Dissolve(_float fTimeDelta);
	void					CullingObject();
	void					Set_to_RootPosition(_float fTimeDelta, _float _TargetDistance= 0.f);
	virtual void			Send_Collision(_uint iDamage, Vec3 vHitPos, STATUSEFFECT eEffect, _float fForce, _float fDuration, _uint iGroggy);
	void					Send_CollidingInfo(const _uint iColLayer, CCollider* pOther);
	virtual void			Set_EffectPos()override;
	virtual HRESULT					Ready_HpUI();


protected:
	_int							m_iAtk = 0;
	_float							m_fMoveSpeed = 1.f;
	_float							m_fAttackMoveSpeed = 0.0f;
	_float							m_fAnimationSpeed = 1.0f;
	vector<_float>					m_vecAttackRanges;
	_float							m_fAttackRange = 1.f;
	_float							m_fNoticeRange = 20.f;
	_uint							m_iBasicAttackStartFrame = 0;
	_uint							m_iBasicAttackEndFrame = 0;
	_bool							m_IsHit = false;
	_bool							m_IsLeft = false;
	_bool							m_IsCulled = false;
	_bool							m_bDissolveIn = false;
	_bool							m_bDissolveOut = false;
	_float							m_fDissolvetime = 0.f;
	_float							m_fMaxDissolvetime = 0.f;
	_bool							m_IsSpawn = true;
	_bool							m_IsSuperArmor = false;
	Vec3							m_vRandomPosition = {};
	wstring							m_strAction = L"";
	unordered_map<wstring, _uint>	m_BoneIndex;
	_float							m_fRootTargetDistance =0.f;
	_float							m_fFontScale=0.4f;

	_bool							m_bRimLight = false;
	_float							m_fRimLightColor = 1.f;
	_float							m_fRimLightTime = 0.0f;

	_bool							m_IsSetuponCell = true;

	_bool							m_IsAlphaBlend = false;

protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CRenderer* m_pRendererCom = nullptr;
	CBehaviorTree* m_pBehaviorTree = nullptr;
	CTexture* m_pDissolveTexture = nullptr;
	
	std::future<HRESULT>			m_PlayAnimation;
	//상태이상
	_float							m_fStatusEffects[(_uint)STATUSEFFECT::EFFECTEND] = {};
	//Culling
	BoundingSphere m_tCullingSphere;

	atomic<_int>					m_iSlowMotionCount = 0;

protected:

	CUI_Monster_Hp* m_pHpUI = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();


};

END

