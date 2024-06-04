#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)
class CEffect;

class CValtan_BT_Attack_BugSmash :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_BugSmash();
    CValtan_BT_Attack_BugSmash(const CValtan_BT_Attack_BugSmash& rhs) = delete;
    virtual ~CValtan_BT_Attack_BugSmash() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;

    Matrix Get_GrabMatrix();

    void    Update_ChargeTrail(_float fTimeDelta);

    void    Smoke_End();
private:
    _bool m_bShoot[2] = {true,true };

    vector<CEffect*> m_SmokeEffects;
    vector<CEffect*> m_ChargeEffects;

    _float m_fChargeTrailAcc = 0.0f;
    _float m_fChargeTrailTime = 0.0f;

    CEffect* m_pRushWarning = nullptr;
public:
    static	CValtan_BT_Attack_BugSmash* Create(void* pArg);
    virtual void Free() override;
};

END