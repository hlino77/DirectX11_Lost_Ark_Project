#pragma once
#include "Client_Defines.h"
#include "Player_Controller.h"

BEGIN(Client)

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
	void			Check_Iden_State(_float fTimeDelta);
	_bool			Is_Identity();
	const _bool&	Is_In_Identity() { return m_IsIdentity; }

public:
	virtual HRESULT	Bind_Skill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);
	void			Set_Attack_Desc(_uint iIndex) { m_AttackDesc = m_Attack_Desces[iIndex]; }

public:
	virtual void	Get_AttackMessage() { Attack(); }
	void			Get_WR_IdentityMessage();
	void			Get_WR_IdenSkillMessage(_float fCoolTime) { m_fCoolTime[SKILL_KEY::Z] = fCoolTime; m_iIdentityGage -= 10; }

	void			Increase_IdenGage(_uint iGage);
	
private:
	virtual void	Input(const _float& fTimeDelta) override;
	virtual void	Attack() override;
	virtual void	SkillAttack(SKILL_KEY eKey, Vec3 vPos) override;
	virtual void	Hit(CGameObject* pHitObject) override;
	virtual void	Skill_CoolTime(const _float& fTimeDelta) override;

private:
	PROJECTILE_DESC	m_Attack_Desces[10];

	_bool	m_IsIdentity = { false };
	_uint   m_iIdentityGage = { 0 };
	_uint	m_iMaxGage = { 100 };
	_float  m_fTimdeAcc = { 0.f };

public:
	static CController_WR* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END