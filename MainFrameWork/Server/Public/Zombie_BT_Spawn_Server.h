#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CZombie_BT_Spawn_Server :
    public CBT_Action
{
private:
    CZombie_BT_Spawn_Server();
    CZombie_BT_Spawn_Server(const CZombie_BT_Spawn_Server& rhs) = delete;
    virtual ~CZombie_BT_Spawn_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    _bool IsOut();

public:
    static	CZombie_BT_Spawn_Server* Create(void* pArg);
    virtual void Free() override;
};

END