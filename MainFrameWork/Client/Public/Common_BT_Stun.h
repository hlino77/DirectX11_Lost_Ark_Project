#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Stun :
    public CBT_Action
{
private:
    CCommon_BT_Stun();
    CCommon_BT_Stun(const CCommon_BT_Stun& rhs) = delete;
    virtual ~CCommon_BT_Stun() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Stun* Create(void* pArg);
    virtual void Free() override;
};

END