#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CGolem_BT_Attack_Swipe_Server :
    public CBT_Action
{
private:
    CGolem_BT_Attack_Swipe_Server();
    CGolem_BT_Attack_Swipe_Server(const CGolem_BT_Attack_Swipe_Server& rhs) = delete;
    virtual ~CGolem_BT_Attack_Swipe_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CGolem_BT_Attack_Swipe_Server* Create(void* pArg);
    virtual void Free() override;
};

END