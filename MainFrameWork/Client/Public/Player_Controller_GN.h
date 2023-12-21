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
	virtual _bool Is_Idle();
	_uint	Is_GN_Identity();

public:
	void			Get_GN_IdentityMessage(GN_IDENTITY eIndex) { GN_Identity(eIndex); }
	virtual void	Get_SkillMessage(GN_IDENTITY eIndex, SKILL_KEY eKey) { Skill(eIndex, eKey); }

public:
	GN_IDENTITY		Get_GN_Identity() { return m_eIdentity; }
	GN_IDENTITY		Get_GN_PreIdentity() { return m_ePreIdentity; }
	virtual void	Skill(GN_IDENTITY eIndex, SKILL_KEY eKey);

public:
	HRESULT			Bind_HandSkill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);
	HRESULT			Bind_ShotSkill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);
	HRESULT			Bind_LongSkill(SKILL_KEY eKey, class CPlayer_Skill* pSkill);

private:
	virtual void	Input(const _float& fTimeDelta) override;
	virtual void	Attack() override;
	virtual void	Hit(CGameObject* pHitObject) override;
	virtual void	Skill_CoolTime(const _float& fTimeDelta) override;

private:
	void			GN_Identity(GN_IDENTITY eIndex);
	void			Change_Skill_Iden(GN_IDENTITY eIndex);
	void			Change_Iden_Collider(GN_IDENTITY eIndex);

private:
	GN_IDENTITY				m_eIdentity = { GN_IDENTITY::HAND };
	GN_IDENTITY				m_ePreIdentity = { GN_IDENTITY::HAND };

	class CPlayer_Skill*	m_pHandSkills[SKILL_KEY::_END] = { nullptr };
	class CPlayer_Skill*	m_pShotSkills[SKILL_KEY::_END] = { nullptr };
	class CPlayer_Skill*	m_pLongSkills[SKILL_KEY::_END] = { nullptr };

	_float					m_fGN_CoolDownAcc[GN_IDENTITY::_END][SKILL_KEY::_END] = { 0.f };
	_float					m_fGN_CoolTime[GN_IDENTITY::_END][SKILL_KEY::_END] = { -1.f };


public:
	static CPlayer_Controller_GN* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END