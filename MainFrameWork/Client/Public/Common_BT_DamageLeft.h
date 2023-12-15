#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_DamageLeft :
    public CBT_Action
{
private:
    CCommon_BT_DamageLeft();
    CCommon_BT_DamageLeft(const CCommon_BT_DamageLeft& rhs) = delete;
    virtual ~CCommon_BT_DamageLeft() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_DamageLeft* Create(void* pArg);
    virtual void Free() override;
};

END