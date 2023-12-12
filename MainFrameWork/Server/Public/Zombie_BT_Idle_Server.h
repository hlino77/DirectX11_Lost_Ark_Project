#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CZombie_BT_Idle_Server :
    public CBT_Action
{
private:
    CZombie_BT_Idle_Server();
    CZombie_BT_Idle_Server(const CZombie_BT_Idle_Server& rhs) = delete;
    virtual ~CZombie_BT_Idle_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CZombie_BT_Idle_Server* Create(void* pArg);
    virtual void Free() override;
};

END