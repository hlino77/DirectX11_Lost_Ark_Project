#pragma once
#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Client)

class CWeapon_Shot_2 final : public CPartObject
{
protected:
	CWeapon_Shot_2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Shot_2(const CWeapon_Shot_2& rhs);
	virtual ~CWeapon_Shot_2() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:

public:
	static CWeapon_Shot_2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END