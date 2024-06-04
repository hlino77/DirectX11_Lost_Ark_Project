#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CGhoul_BT_Attack_3 :
    public CBT_Action
{
private:
    CGhoul_BT_Attack_3();
    CGhoul_BT_Attack_3(const CGhoul_BT_Attack_3& rhs) = delete;
    virtual ~CGhoul_BT_Attack_3() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CGhoul_BT_Attack_3* Create(void* pArg);
    virtual void Free() override;
};

END