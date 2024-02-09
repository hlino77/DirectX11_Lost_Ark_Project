#pragma once
#include "Esther_Skill.h"

BEGIN(Client)

class CEsther;
class CPlayer;

class CEsther_Silian_Skill final : public CEsther_Skill
{
private:
	CEsther_Silian_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEsther_Silian_Skill(const CEsther_Silian_Skill& rhs);
	virtual ~CEsther_Silian_Skill() = default;

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
	void					Cut_Start(_float fTimeDelta);
	void					Act1(_float fTimeDelta);

private:
	_uint					m_iAnimIndex;

	_bool					m_bCutStart = false;
	_bool					m_bProjShot = false;

public:
	static CEsther_Silian_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free();

};

END

