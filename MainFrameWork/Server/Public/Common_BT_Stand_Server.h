#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class Common_BT_Stand_Server :
    public CBT_Action
{
private:
    Common_BT_Stand_Server();
    Common_BT_Stand_Server(const Common_BT_Stand_Server& rhs) = delete;
    virtual ~Common_BT_Stand_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	Common_BT_Stand_Server* Create(void* pArg);
    virtual void Free() override;
};

END