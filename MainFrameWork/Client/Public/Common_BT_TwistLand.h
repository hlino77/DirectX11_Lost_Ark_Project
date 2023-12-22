#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_TwistLand :
    public CBT_Action
{
private:
    CCommon_BT_TwistLand();
    CCommon_BT_TwistLand(const CCommon_BT_TwistLand& rhs) = delete;
    virtual ~CCommon_BT_TwistLand() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_TwistLand* Create(void* pArg);
    virtual void Free() override;
};

END