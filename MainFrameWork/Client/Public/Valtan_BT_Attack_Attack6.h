#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_Attack_Attack6 :
    public CBT_Action
{
private:
    CValtan_BT_Attack_Attack6();
    CValtan_BT_Attack_Attack6(const CValtan_BT_Attack_Attack6& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack6() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CValtan_BT_Attack_Attack6* Create(void* pArg);
    virtual void Free() override;
};

END