#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CGhoul_BT_Attack_1_Server :
    public CBT_Action
{
private:
    CGhoul_BT_Attack_1_Server();
    CGhoul_BT_Attack_1_Server(const CGhoul_BT_Attack_1_Server& rhs) = delete;
    virtual ~CGhoul_BT_Attack_1_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CGhoul_BT_Attack_1_Server* Create(void* pArg);
    virtual void Free() override;
};

END