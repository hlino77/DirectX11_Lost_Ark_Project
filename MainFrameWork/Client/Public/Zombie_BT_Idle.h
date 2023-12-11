#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CZombie_BT_Idle :
    public CBT_Action
{
private:
    CZombie_BT_Idle();
    CZombie_BT_Idle(const CZombie_BT_Idle& rhs) = delete;
    virtual ~CZombie_BT_Idle() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CZombie_BT_Idle* Create(void* pArg);
    virtual void Free() override;
};

END