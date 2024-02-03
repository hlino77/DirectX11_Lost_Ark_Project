#pragma once
#include "Esther_Skill.h"

BEGIN(Client)

class CEsther_Bahuntur_Skill_Floor;
class CEsther_Bahuntur_Skill_Ceiling;

class CEsther_Bahuntur_Skill final : public CEsther_Skill
{
private:
	CEsther_Bahuntur_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Bahuntur_Skill(const CEsther_Bahuntur_Skill& rhs);
	virtual ~CEsther_Bahuntur_Skill() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_ShadowDepth();

public:
	virtual void			Reset() override;
	virtual void			Ready() override;
	virtual void			Check_Finish() override;

private:
	virtual HRESULT			Ready_Components();
	virtual HRESULT			Ready_ModelPart();
	virtual HRESULT			Ready_Parts();
	virtual HRESULT			Ready_Projectile();

private:
	void					Act1(_float fTimeDelta);
	void					Cut_Start(_float fTimeDelta);
	void					Act2(_float fTimeDelta);
	void					Act3(_float fTimeDelta);

private:
	class CEsther_Bahuntur_Skill_Floor*		m_pSkillFloor = { nullptr };
	class CEsther_Bahuntur_Skill_Ceiling*	m_pSkillCeiling = { nullptr };

	_uint					m_iAnimIndex;

	_bool					m_bCutStart = { false };
	_bool					m_bAnim = { false };

	_float					m_fTimeAcc = { 0.0f };
	_float					m_fAnimTime = { 1.5f };

public:
	static CEsther_Bahuntur_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END

