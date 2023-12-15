#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Maz :
    public CBT_Action
{
private:
    CCommon_BT_Maz();
    CCommon_BT_Maz(const CCommon_BT_Maz& rhs) = delete;
    virtual ~CCommon_BT_Maz() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Maz* Create(void* pArg);
    virtual void Free() override;
};

END