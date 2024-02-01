#pragma once
#include "Client_Defines.h"
#include "Player_Controller.h"

BEGIN(Client)

class CController_SP final : public CPlayer_Controller
{
public:
	enum IDEN { MOON, SUN, IDEN_END };

private:
	CController_SP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CController_SP(const CController_SP& rhs);
	virtual ~CController_SP() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Tick(_float fTimeDelta) override;
	virtual void	LateTick(_float fTimeDelta) override;
	virtual void	DebugRender() override;

public:
	virtual void	Get_HitMessage(_uint iDamge, _float fForce, Vec3 vPos = Vec3());
	virtual void	Get_GrabMessage(CGameObject* pGrabber);
	virtual void	Get_GrabEndMessage();
	virtual void	Get_DeadMessage();

	/* 아덴 관련 함수 */
	_uint			Is_SP_Identity();
	const _int&		Get_IdenGage() { return m_iIdentityGage; }
	const _int&		Get_IdenMaxGauge() { return m_iMaxGage; }

public:
	virtual HRESULT	Bind_Skill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);
	void			Set_Attack_Desc(_uint iIndex) { m_AttackDesc = m_Attack_Desces[iIndex]; }
	CPlayer_Skill*  Get_PlayerSkill_MG(SKILL_KEY eKey);

public:
	virtual void	Get_AttackMessage(Vec3 vPos = Vec3()) { Attack(vPos); }
	void			Get_SP_IdentityMessage();
	void			Increase_IdenGage(_uint iGage);

private:
	virtual void	Input(const _float& fTimeDelta) override;
	virtual void	Attack(Vec3 vPos) override;
	virtual void	SkillAttack(SKILL_KEY eKey, Vec3 vPos) override;
	virtual void	Skill_CoolTime(const _float& fTimeDelta) override;

	virtual void	EstherSkill(_uint iIndex) override;

private:
	PROJECTILE_DESC	m_Attack_Desces[10];

	_float	m_iIdentityGage = { 0.f };
	_float	m_iMaxGage = { 100.f };

	_uint	m_iMarbleCnt = { 3 };
	_uint	m_iMaxMarbleCnt = { 3 };

	_int	m_iIdenSkill = { IDEN_END };
	
	_float  m_fTimdeAcc = { 0.f };

public:
	static CController_SP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END