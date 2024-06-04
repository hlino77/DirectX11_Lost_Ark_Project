#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_SilenceChop :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_SilenceChop();
    CValtan_BT_Attack_SilenceChop(const CValtan_BT_Attack_SilenceChop& rhs) = delete;
    virtual ~CValtan_BT_Attack_SilenceChop() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    _bool   m_bShoot[2] = {};
    _bool   m_bOutSide;

    _bool   m_bWarning = false;
public:
    static	CValtan_BT_Attack_SilenceChop* Create(void* pArg);
    virtual void Free() override;
};

END