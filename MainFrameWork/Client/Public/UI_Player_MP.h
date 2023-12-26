#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_Player_MP :
    public CUI
{
private:
	CUI_Player_MP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Player_MP(const CUI& rhs);
	virtual ~CUI_Player_MP() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(const _float& fTimeDelta);
	virtual void LateTick(const _float& fTimeDelta);
	virtual HRESULT Render();

	virtual void UI_Tick(_float fTimeDelta) override;

private:
	HRESULT	UI_Set();

public:
	static  CUI_Player_MP* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END