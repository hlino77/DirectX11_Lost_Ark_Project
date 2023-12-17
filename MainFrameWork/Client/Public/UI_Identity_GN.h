#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_IdentityGN :
    public CUI
{
private:
	CUI_IdentityGN(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_IdentityGN(const CUI& rhs);
	virtual ~CUI_IdentityGN() = default;

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
	static  CUI_IdentityGN* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END