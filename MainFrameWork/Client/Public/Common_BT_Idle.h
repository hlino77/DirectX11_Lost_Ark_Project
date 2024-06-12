#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Idle :
    public CBT_Action
{
private:
    CCommon_BT_Idle();
    CCommon_BT_Idle(const CCommon_BT_Idle& rhs) = delete;
    virtual ~CCommon_BT_Idle() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Idle* Create(void* pArg);
    virtual void Free() override;
};

END