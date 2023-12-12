#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CZombie_BT_Maz_Server :
    public CBT_Action
{
private:
    CZombie_BT_Maz_Server();
    CZombie_BT_Maz_Server(const CZombie_BT_Maz_Server& rhs) = delete;
    virtual ~CZombie_BT_Maz_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CZombie_BT_Maz_Server* Create(void* pArg);
    virtual void Free() override;
};

END