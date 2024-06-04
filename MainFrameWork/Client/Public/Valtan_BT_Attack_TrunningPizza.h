#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_TrunningPizza :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_TrunningPizza();
    CValtan_BT_Attack_TrunningPizza(const CValtan_BT_Attack_TrunningPizza& rhs) = delete;
    virtual ~CValtan_BT_Attack_TrunningPizza() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
     _bool  m_bShoot = false;

public:
    static	CValtan_BT_Attack_TrunningPizza* Create(void* pArg);
    virtual void Free() override;
};

END