#pragma once
#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Client)

class CWeapon_Boss_Valtan final : public CPartObject
{
protected:
	CWeapon_Boss_Valtan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Boss_Valtan(const CWeapon_Boss_Valtan& rhs);
	virtual ~CWeapon_Boss_Valtan() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
public:
	void					Set_AnimationSpeed(_float fAnimationSpeed) { m_fAnimationSpeed = fAnimationSpeed; }
	_float					Get_AnimationSpeed() { return m_fAnimationSpeed; }

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	_float							m_fAnimationSpeed = 1.0f;
	Vec3							m_vRotationms;
	std::future<HRESULT>			m_PlayAnimation;

public:
	static CWeapon_Boss_Valtan* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END