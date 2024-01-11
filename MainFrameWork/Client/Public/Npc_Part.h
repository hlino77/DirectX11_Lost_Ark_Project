#pragma once
#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Client)

class CNpc_Part final : public CPartObject
{
protected:
	CNpc_Part(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNpc_Part(const CNpc_Part& rhs);
	virtual ~CNpc_Part() = default;

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
	virtual void	Set_EffectPos() override;

public:
	static CNpc_Part* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END