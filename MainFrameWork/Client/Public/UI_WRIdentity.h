#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_WRIdentity :
    public CUI
{
private:
    CUI_WRIdentity(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_WRIdentity(const CUI& rhs);
    virtual ~CUI_WRIdentity() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void UI_Tick(_float fTimeDelta) override;

private:
	HRESULT	UI_Set();

public:
	static  CUI_WRIdentity* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END