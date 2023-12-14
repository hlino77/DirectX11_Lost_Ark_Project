#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_Loading :
    public CUI
{
private:
	CUI_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Loading(const CUI& rhs);
	virtual ~CUI_Loading() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void UI_Tick(_float fTimeDelta) override;
	void	Change_LoadingPer();
private:
	HRESULT	UI_Set();

private:
	vector<CUI*>	m_vecServerUI;

public:
	static  CUI_Loading* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END