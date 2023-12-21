#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CPawn_BT_Attack2_Server :
    public CBT_Action
{
private:
    CPawn_BT_Attack2_Server();
    CPawn_BT_Attack2_Server(const CPawn_BT_Attack2_Server& rhs) = delete;
    virtual ~CPawn_BT_Attack2_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CPawn_BT_Attack2_Server* Create(void* pArg);
    virtual void Free() override;
};

END