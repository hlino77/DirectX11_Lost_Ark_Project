#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CZombie_BT_Attack1 :
    public CBT_Action
{
private:
    CZombie_BT_Attack1();
    CZombie_BT_Attack1(const CZombie_BT_Attack1& rhs) = delete;
    virtual ~CZombie_BT_Attack1() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CZombie_BT_Attack1* Create(void* pArg);
    virtual void Free() override;
};

END