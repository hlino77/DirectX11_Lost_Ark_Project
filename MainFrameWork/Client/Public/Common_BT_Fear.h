#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Fear :
    public CBT_Action
{
private:
    CCommon_BT_Fear();
    CCommon_BT_Fear(const CCommon_BT_Fear& rhs) = delete;
    virtual ~CCommon_BT_Fear() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Fear* Create(void* pArg);
    virtual void Free() override;
};

END