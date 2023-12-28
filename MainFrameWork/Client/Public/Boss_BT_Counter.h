#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CBoss_BT_Counter :
    public CBT_Action
{
private:
    CBoss_BT_Counter();
    CBoss_BT_Counter(const CBoss_BT_Counter& rhs) = delete;
    virtual ~CBoss_BT_Counter() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CBoss_BT_Counter* Create(void* pArg);
    virtual void Free() override;
};

END