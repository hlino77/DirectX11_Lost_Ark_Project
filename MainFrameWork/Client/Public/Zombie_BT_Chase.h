#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CZombie_BT_Chase :
    public CBT_Action
{
private:
    CZombie_BT_Chase();
    CZombie_BT_Chase(const CZombie_BT_Chase& rhs) = delete;
    virtual ~CZombie_BT_Chase() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CZombie_BT_Chase* Create(void* pArg);
    virtual void Free() override;
};

END