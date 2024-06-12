#pragma once
#include "Client_Defines.h"
#include "Player_Controller.h"

BEGIN(Client)

class CPlayer_Controller_GN final : public CPlayer_Controller
{
public:
	enum GN_IDENTITY { HAND, SHOT, LONG, _END };

private:
	CPlayer_Controller_GN(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Controller_GN(const CPlayer_Controller_GN& rhs);
	virtual ~CPlayer_Controller_GN() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	LateTick(_float fTimeDelta) override;
	virtual void	DebugRender() override;

public:
	virtual _bool		Is_EstherSkill() override;

	virtual _bool Is_Idle();
	_uint	Is_GN_Identity();

public:
	virtual void	Get_AttackMessage(Vec3 vPos = Vec3()) { Attack(vPos); }
	void			Get_GN_IdentityMessage(GN_IDENTITY eIndex) { GN_Identity(eIndex); }
	virtual void	Get_SkillMessage(GN_IDENTITY eIndex, SKILL_KEY eKey) { Skill(eIndex, eKey); }
	virtual void	Get_HitMessage(_uint iDamge, _float fForce, Vec3 vPos = Vec3());
	virtual void	Get_GrabMessage(CGameObject* pGrabber);
	virtual void	Get_GrabEndMessage();
	virtual void	Get_EshterGageUseMessage() override;

public:
	GN_IDENTITY		Get_GN_Identity() { return m_eIdentity; }
	GN_IDENTITY		Get_GN_PreIdentity() { return m_ePreIdentity; }
	virtual void	Skill(GN_IDENTITY eIndex, SKILL_KEY eKey);

public:
	HRESULT			Bind_HandSkill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);
	HRESULT			Bind_ShotSkill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);
	HRESULT			Bind_LongSkill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);

	CPlayer_Skill* Get_PlayerSkill_Weapon(GN_IDENTITY eStance, SKILL_KEY eKey);
	void			Change_Skill_Iden(GN_IDENTITY eIndex);
private:
	virtual void	Input(_float fTimeDelta) override;
	virtual void	Attack(Vec3 vPos) override;
	virtual void	SkillAttack(SKILL_KEY eKey, Vec3 vPos) override;
	virtual void	Skill_CoolTime(_float fTimeDelta) override;

private:
	void			GN_Identity(GN_IDENTITY eIndex);
	void			Change_Iden_Collider(GN_IDENTITY eIndex);

	virtual void	EstherSkill(_uint iIndex) override;
	virtual void	Esther_Refill(_float fTimeDelta) override;


private:
	GN_IDENTITY				m_eIdentity = { GN_IDENTITY::HAND };
	GN_IDENTITY				m_ePreIdentity = { GN_IDENTITY::HAND };

	class CPlayer_Skill*	m_pHandSkills[SKILL_KEY::_END] = { nullptr };
	class CPlayer_Skill*	m_pShotSkills[SKILL_KEY::_END] = { nullptr };
	class CPlayer_Skill*	m_pLongSkills[SKILL_KEY::_END] = { nullptr };

	PROJECTILE_DESC			m_HandAttackDesc;
	PROJECTILE_DESC			m_ShotAttackDesc;
	PROJECTILE_DESC			m_LongAttackDesc;


	_float					m_fGN_CoolDownAcc[GN_IDENTITY::_END][SKILL_KEY::_END] = { 0.f };
	_float					m_fGN_CoolTime[GN_IDENTITY::_END][SKILL_KEY::_END] = { -1.f };


public:
	static CPlayer_Controller_GN* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END