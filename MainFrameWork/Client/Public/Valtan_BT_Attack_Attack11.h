#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_Attack11 :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_Attack11();
    CValtan_BT_Attack_Attack11(const CValtan_BT_Attack_Attack11& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack11() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CValtan_BT_Attack_Attack11* Create(void* pArg);
    virtual void Free() override;
};

END