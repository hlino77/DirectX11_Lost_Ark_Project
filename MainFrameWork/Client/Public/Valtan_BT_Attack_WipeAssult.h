#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)
class CEffect;

class CValtan_BT_Attack_WipeAssult :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_WipeAssult();
    CValtan_BT_Attack_WipeAssult(const CValtan_BT_Attack_WipeAssult& rhs) = delete;
    virtual ~CValtan_BT_Attack_WipeAssult() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:

    void    Update_LightningEffect(_float fTimeDelta);
private:
    _bool     m_bShoot[2] = {};
    _bool     m_bTrailEffect[2] = {};
    _bool     m_bWarningEffect[2] = {};

    vector<CEffect*> m_Effects;

    _float m_fLightningAcc = 0.0f;
    _float m_fLightningTime = 0.0f;
public:
    static	CValtan_BT_Attack_WipeAssult* Create(void* pArg);
    virtual void Free() override;
};

END