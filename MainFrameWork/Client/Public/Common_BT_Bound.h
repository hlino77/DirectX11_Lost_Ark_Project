#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Bound :
    public CBT_Action
{
private:
    CCommon_BT_Bound();
    CCommon_BT_Bound(const CCommon_BT_Bound& rhs) = delete;
    virtual ~CCommon_BT_Bound() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Bound* Create(void* pArg);
    virtual void Free() override;
};

END