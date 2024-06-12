#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Bug :
    public CBT_Action
{
private:
    CCommon_BT_Bug();
    CCommon_BT_Bug(const CCommon_BT_Bug& rhs) = delete;
    virtual ~CCommon_BT_Bug() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Bug* Create(void* pArg);
    virtual void Free() override;
};

END