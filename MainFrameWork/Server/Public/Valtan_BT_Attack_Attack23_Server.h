#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CValtan_BT_Attack_Attack23_Server :
    public CBT_Action
{
private:
    CValtan_BT_Attack_Attack23_Server();
    CValtan_BT_Attack_Attack23_Server(const CValtan_BT_Attack_Attack23_Server& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack23_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
    virtual void On_FirstAnimStart()override;
    virtual void On_LastAnimEnd()  override;

public:
    static	CValtan_BT_Attack_Attack23_Server* Create(void* pArg);
    virtual void Free() override;
};

END