#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include <atomic>

BEGIN(Engine)
class CTransform;
class CSphereCollider;
class CCollider;
class CRenderer;
class CModel;
END


BEGIN(Client)
class CSkill : public CGameObject
{
public:
	typedef struct ModelDesc
	{
		wstring strFileName;
		_int	iObjectID;
		_uint	iLayer;
		CGameObject* pOwner;
	}MODELDESC;

	typedef struct SkillDesc
	{
		_uint				iAtk = 0;
		_float				fForce = 0.0f;
		STATUSEFFECT		eStatusEffect = STATUSEFFECT::EFFECTEND;
		_float				fStatusEffectDuration = 0.0f;
	}SKILLDESC;

protected:
	CSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill(const CSkill& rhs);
	virtual ~CSkill() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render() = 0;

	virtual HRESULT Render_Debug() override;


	virtual	void	OnCollisionEnter(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionStay(const _uint iColLayer, class CCollider* pOther) override;
	virtual	void	OnCollisionExit(const _uint iColLayer, class CCollider* pOther) override;

	void Set_DoughnutRadii(_float OutsideRadius, _float InsideRadius);

	void Set_PizzaRadii(_float OutsideRadius, _float InsideRadius);

	void Set_PizzaSlope(_float fLeftSlope, _float fRightSlope);

	_bool Get_Collider_Center(_uint iID, Vec3* pCenter);

	virtual void	Explosion() PURE;
	virtual void	Buff_Player(_uint iPlayerID) {};


	virtual HRESULT					Ready_Coliders() = 0;
public:


	void					Set_MoveSpeed(_float fSpeed) { m_fMoveSpeed = fSpeed; }
	_float					Get_MoveSpeed() { return m_fMoveSpeed; }
	void					Add_MoveSpeed(_float fSpeed, _float fMaxSpeed) { m_fMoveSpeed += fSpeed; m_fMoveSpeed = min(m_fMoveSpeed, fMaxSpeed); }

	CGameObject*			Get_SkillOwner() { return m_pSkillOwner; }

public:


	void					Find_NearTarget();

	void					Send_NearTarget();

	_float					Get_NearTargetDistance();

	void					Send_ColliderState(const _uint& iLayer);

	void					Set_Colliders(_float fTimeDelta);


	void					Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta);

	void					Set_Die();

	_uint					Get_Atk() { return m_SkillDesc.iAtk; }
	void					Set_Atk(_uint iAtk) { m_SkillDesc.iAtk = iAtk; }

	_float					Get_Force() { return m_SkillDesc.fForce; }
	void					Set_Force(_float fForce) { m_SkillDesc.fForce = fForce; }

	STATUSEFFECT			Get_StatusEffect() { return m_SkillDesc.eStatusEffect; }
	void					Set_StatusEffect(STATUSEFFECT eStatusEffect) { m_SkillDesc.eStatusEffect = eStatusEffect; }

	_float					Get_StatusEffectDuration() { return m_SkillDesc.fStatusEffectDuration; }
	void					Set_StatusEffectDuration(_float fStatusEffectDuration) { m_SkillDesc.fStatusEffectDuration = fStatusEffectDuration; }

	SKILLDESC					Get_SkillDesc() { return m_SkillDesc; }
	void					Set_SkillDesc(SKILLDESC SkillDesc) { m_SkillDesc = SkillDesc; }

	_float					Get_LastTime() { return m_fLastTime; }
	void					Set_LastTime(_float fLastTime) { m_fLastTime = fLastTime; }

	_float					Get_BlinkTime() { return m_fBlinkTime; }
	void					Set_BlinkTime(_float fBlinkTime) { m_fBlinkTime = fBlinkTime; }

	_bool					Is_Destructive() { return m_bDestructive; }
	void					Set_Destructive(_bool bDestructive) { m_bDestructive = bDestructive; }


	_bool					Is_SafeZonePierce() { return m_bSafeZonePierce; }
	void					Set_SafeZonePierce(_bool bSafeZonePierce) { m_bSafeZonePierce = bSafeZonePierce; }
	
	void					Set_SoundTag(wstring strSoundTag) { m_strSoundTag = strSoundTag; }
protected:
	virtual HRESULT			Ready_Components();



protected:
	CRenderer*						m_pRendererCom = nullptr;
	

	_float							m_fBlinkTime = 0.f;
	_float							m_fLastTime = 0.f;
	_float							m_fMoveSpeed = 0.0f;

	SKILLDESC						m_SkillDesc;
	_float							m_fFollowDistance = 0.0f;
	_bool							m_bDestructive = false;
	_bool							m_bSafeZonePierce = false;
	CGameObject*					m_pSkillOwner = nullptr;
protected: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	wstring							m_strSoundTag = {};
	_bool							m_bSoundOn = false;
public:
	virtual void Free();


};

END

