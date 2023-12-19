#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CReaper_BT_Attack2 :
    public CBT_Action
{
private:
    CReaper_BT_Attack2();
    CReaper_BT_Attack2(const CReaper_BT_Attack2& rhs) = delete;
    virtual ~CReaper_BT_Attack2() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CReaper_BT_Attack2* Create(void* pArg);
    virtual void Free() override;
};

END