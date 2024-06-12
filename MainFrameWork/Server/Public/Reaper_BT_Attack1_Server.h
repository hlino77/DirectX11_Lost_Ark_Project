#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CReaper_BT_Attack1_Server :
    public CBT_Action
{
private:
    CReaper_BT_Attack1_Server();
    CReaper_BT_Attack1_Server(const CReaper_BT_Attack1_Server& rhs) = delete;
    virtual ~CReaper_BT_Attack1_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CReaper_BT_Attack1_Server* Create(void* pArg);
    virtual void Free() override;
};

END