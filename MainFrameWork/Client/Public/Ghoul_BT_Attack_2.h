#pragma once

#include "Client_Defines.h"
#include "BT_MultiAnimationAction.h"

BEGIN(Client)

class CGhoul_BT_Attack_2 :
    public CBT_Action
{
private:
    CGhoul_BT_Attack_2();
    CGhoul_BT_Attack_2(const CGhoul_BT_Attack_2& rhs) = delete;
    virtual ~CGhoul_BT_Attack_2() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CGhoul_BT_Attack_2* Create(void* pArg);
    virtual void Free() override;
};

END