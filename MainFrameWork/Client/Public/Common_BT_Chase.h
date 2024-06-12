#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Chase :
    public CBT_Action
{
private:
    CCommon_BT_Chase();
    CCommon_BT_Chase(const CCommon_BT_Chase& rhs) = delete;
    virtual ~CCommon_BT_Chase() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Chase* Create(void* pArg);
    virtual void Free() override;
};

END