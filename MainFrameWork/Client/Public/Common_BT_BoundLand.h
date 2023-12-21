#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_BoundLand :
    public CBT_Action
{
private:
    CCommon_BT_BoundLand();
    CCommon_BT_BoundLand(const CCommon_BT_BoundLand& rhs) = delete;
    virtual ~CCommon_BT_BoundLand() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_BoundLand* Create(void* pArg);
    virtual void Free() override;
};

END