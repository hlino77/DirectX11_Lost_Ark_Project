#pragma once
#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Client)

class CWeapon_Mn_Reaper final : public CPartObject
{
protected:
	CWeapon_Mn_Reaper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Mn_Reaper(const CWeapon_Mn_Reaper& rhs);
	virtual ~CWeapon_Mn_Reaper() = default;

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
	static CWeapon_Mn_Reaper* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END