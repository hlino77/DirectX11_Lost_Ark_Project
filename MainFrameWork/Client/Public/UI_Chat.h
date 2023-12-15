#pragma once
#include "UI.h"

BEGIN(Client)

class CUI_Chat :
    public CUI
{
private:
    CUI_Chat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_Chat(const CUI& rhs);
    virtual ~CUI_Chat() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	virtual void UI_Tick(_float fTimeDelta) override;

private:
	HRESULT	UI_Set();
	
private:
	vector<CUI*>	m_vecUIParts;

public:
	static  CUI_Chat* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END