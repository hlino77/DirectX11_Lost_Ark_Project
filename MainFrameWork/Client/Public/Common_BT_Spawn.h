#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Spawn :
    public CBT_Action
{
private:
    CCommon_BT_Spawn();
    CCommon_BT_Spawn(const CCommon_BT_Spawn& rhs) = delete;
    virtual ~CCommon_BT_Spawn() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

public:
    static	CCommon_BT_Spawn* Create(void* pArg);
    virtual void Free() override;
};

END