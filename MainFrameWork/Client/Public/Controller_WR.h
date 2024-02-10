#pragma once
#include "Client_Defines.h"
#include "Player_Controller.h"
#include "Delegate.h"

BEGIN(Client)

class CEsther;

class CController_WR final : public CPlayer_Controller
{
public:

private:
	CController_WR(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CController_WR(const CController_WR& rhs);
	virtual ~CController_WR() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	LateTick(_float fTimeDelta) override;
	virtual void	DebugRender() override;

public:
	virtual _bool		Is_EstherSkill() override;

	/* 아덴 관련 함수 */
	void			Check_Iden_State(_float fTimeDelta);
	_bool			Is_Identity();
	const _bool&	Is_In_Identity() { return m_IsIdentity; }
	const _float&	Get_IdenGage() { return m_fIdentityGage; }
	const _float&	Get_IdenMaxGauge() { return m_fMaxGage; }
	const _float&	Get_IdenCoolTime() { return m_fIdenCoolTime; }
	const _float	Get_IdenCoolAcc() { return m_fIdenTimeAcc; }
	const	_float& Get_IdenSkillCool() { return m_fIdenSkillCool; }

public:
	virtual HRESULT	Bind_Skill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);
	void			Set_Attack_Desc(_uint iIndex) { m_AttackDesc = m_Attack_Desces[iIndex]; }
	CPlayer_Skill*  Get_PlayerSkill_WR(SKILL_KEY eKey);

public:
	virtual void	Get_AttackMessage(Vec3 vPos = Vec3()) { Attack(vPos); }
	virtual void	Get_HitMessage(_uint iDamge, _float fForce, Vec3 vPos = Vec3());
	virtual void	Get_GrabMessage(CGameObject* pGrabber);
	virtual void	Get_GrabEndMessage();
	virtual void	Get_DeadMessage();

	void			Get_WR_IdentityMessage();
	void			Get_WR_IdenSkillMessage(_float fCoolTime) { m_fCoolTime[SKILL_KEY::Z] = m_fIdenSkillCool; m_fIdentityGage -= 5.f; }

	void			Increase_IdenGage(_float iGage);

public:
	FDelegate<Matrix&>	CB_UpdateIdentityAuraPivot;

private:
	virtual void	Input(const _float& fTimeDelta) override;
	virtual void	Attack(Vec3 vPos) override;
	virtual void	SkillAttack(SKILL_KEY eKey, Vec3 vPos) override;
	virtual void	Skill_CoolTime(const _float& fTimeDelta) override;


	void			UpdateIdentityAuraPivot();
	virtual void	EstherSkill(_uint iIndex) override;
private:
	PROJECTILE_DESC	m_Attack_Desces[10];

	_bool	m_IsIdentity = { false };
	_float	m_fIdentityGage = { 0.f };
	_float	m_fMaxGage = { 30.f };
	_float  m_fTimeAcc = { 0.f };

	_float	m_fIdenTimeAcc = { 0.f };
	_float	m_fIdenCoolTime = { 30.f };

	_float	m_fIdenSkillCool = { 5.f };

public:
	static CController_WR* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END