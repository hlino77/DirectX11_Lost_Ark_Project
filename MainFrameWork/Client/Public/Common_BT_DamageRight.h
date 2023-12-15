#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_DamageRight :
    public CBT_Action
{
private:
    CCommon_BT_DamageRight();
    CCommon_BT_DamageRight(const CCommon_BT_DamageRight& rhs) = delete;
    virtual ~CCommon_BT_DamageRight() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_DamageRight* Create(void* pArg);
    virtual void Free() override;
};

END