#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CReaper_BT_Attack3_Server :
    public CBT_Action
{
private:
    CReaper_BT_Attack3_Server();
    CReaper_BT_Attack3_Server(const CReaper_BT_Attack3_Server& rhs) = delete;
    virtual ~CReaper_BT_Attack3_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CReaper_BT_Attack3_Server* Create(void* pArg);
    virtual void Free() override;
};

END