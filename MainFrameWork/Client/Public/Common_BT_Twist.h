#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Twist :
    public CBT_Action
{
private:
    CCommon_BT_Twist();
    CCommon_BT_Twist(const CCommon_BT_Twist& rhs) = delete;
    virtual ~CCommon_BT_Twist() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Twist* Create(void* pArg);
    virtual void Free() override;
};

END