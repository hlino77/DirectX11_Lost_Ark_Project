#pragma once
#include "UI.h"

BEGIN(Client)
class CUI_DeadWnd;
class CUI_WatchingMode;
class CPlayer;
class CCamera_Player;
class CUI_DeadScene :
    public CUI
{
private:
    CUI_DeadScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI_DeadScene(const CUI& rhs);
    virtual ~CUI_DeadScene() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void UI_Tick(_float fTimeDelta) override {}

private:
    HRESULT UI_Set();
    void    Update_UIs(_float fTimeDelta);
    void    Late_Update_UIs(_float fTimeDelta);

private:
    virtual HRESULT Ready_Components();
    virtual HRESULT Bind_ShaderResources();

private:
    _uint   m_DeadMode = { 0 };

    CUI_DeadWnd* m_pDeadWnd = { nullptr };
    CUI_WatchingMode* m_pWatchingMode = { nullptr };

public:
    static  CUI_DeadScene* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void    Free() override;
};

END