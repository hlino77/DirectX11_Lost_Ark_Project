#pragma once
#include "Esther_Skill.h"

BEGIN(Client)

class CEsther;
class CPlayer;

class CEsther_Way_Dochul;

class CEsther_Way_Skill final : public CEsther_Skill
{
private:
	CEsther_Way_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Way_Skill(const CEsther_Way_Skill& rhs);
	virtual ~CEsther_Way_Skill() = default;

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
	void					Act2(_float fTimeDelta);
	void					Act3(_float fTimeDelta);

private:
	CEsther_Way_Dochul*		m_pSkillMesh = { nullptr };

	_uint					m_iAnimIndex;

public:
	static CEsther_Way_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END

