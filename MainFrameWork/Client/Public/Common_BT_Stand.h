#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class Common_BT_Stand :
    public CBT_Action
{
private:
    Common_BT_Stand();
    Common_BT_Stand(const Common_BT_Stand& rhs) = delete;
    virtual ~Common_BT_Stand() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	Common_BT_Stand* Create(void* pArg);
    virtual void Free() override;
};

END