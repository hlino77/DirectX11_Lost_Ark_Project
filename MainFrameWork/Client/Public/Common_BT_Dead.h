#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Dead :
    public CBT_Action
{
private:
    CCommon_BT_Dead();
    CCommon_BT_Dead(const CCommon_BT_Dead& rhs) = delete;
    virtual ~CCommon_BT_Dead() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Dead* Create(void* pArg);
    virtual void Free() override;
};

END