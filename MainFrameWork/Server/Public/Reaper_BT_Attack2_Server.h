#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CReaper_BT_Attack2_Server :
    public CBT_Action
{
private:
    CReaper_BT_Attack2_Server();
    CReaper_BT_Attack2_Server(const CReaper_BT_Attack2_Server& rhs) = delete;
    virtual ~CReaper_BT_Attack2_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CReaper_BT_Attack2_Server* Create(void* pArg);
    virtual void Free() override;
};

END