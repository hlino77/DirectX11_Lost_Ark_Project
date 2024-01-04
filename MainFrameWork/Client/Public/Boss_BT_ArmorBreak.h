#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CBoss_BT_ArmorBreak :
    public CBT_Action
{
private:
    CBoss_BT_ArmorBreak();
    CBoss_BT_ArmorBreak(const CBoss_BT_ArmorBreak& rhs) = delete;
    virtual ~CBoss_BT_ArmorBreak() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CBoss_BT_ArmorBreak* Create(void* pArg);
    virtual void Free() override;
};

END