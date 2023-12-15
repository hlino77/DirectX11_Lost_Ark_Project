#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CPlant_BT_Attack_Root_Server :
    public CBT_Action
{
private:
    CPlant_BT_Attack_Root_Server();
    CPlant_BT_Attack_Root_Server(const CPlant_BT_Attack_Root_Server& rhs) = delete;
    virtual ~CPlant_BT_Attack_Root_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CPlant_BT_Attack_Root_Server* Create(void* pArg);
    virtual void Free() override;
};

END