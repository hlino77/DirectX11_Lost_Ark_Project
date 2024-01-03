#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CBoss_BT_Groggy :
    public CBT_Action
{
private:
    CBoss_BT_Groggy();
    CBoss_BT_Groggy(const CBoss_BT_Groggy& rhs) = delete;
    virtual ~CBoss_BT_Groggy() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CBoss_BT_Groggy* Create(void* pArg);
    virtual void Free() override;
};

END