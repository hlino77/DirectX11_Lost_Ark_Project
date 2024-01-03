#pragma once
#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Client)

class CMannequinPart final : public CPartObject
{
protected:
	CMannequinPart(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMannequinPart(const CMannequinPart& rhs);
	virtual ~CMannequinPart() = default;

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
	static CMannequinPart* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END