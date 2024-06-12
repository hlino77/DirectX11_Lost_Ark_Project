#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_TrippleCounterChop_3 :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_TrippleCounterChop_3();
    CValtan_BT_Attack_TrippleCounterChop_3(const CValtan_BT_Attack_TrippleCounterChop_3& rhs) = delete;
    virtual ~CValtan_BT_Attack_TrippleCounterChop_3() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;

public:
    virtual void        On_FirstAnimStart()                 override;

private:
    void    Update_Effect();

private:
    _bool m_bShoot = false;
    _bool m_bEffectStart = false;

public:
    static	CValtan_BT_Attack_TrippleCounterChop_3* Create(void* pArg);
    virtual void Free() override;
};

END