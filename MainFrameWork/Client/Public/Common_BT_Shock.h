#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Shock :
    public CBT_Action
{
private:
    CCommon_BT_Shock();
    CCommon_BT_Shock(const CCommon_BT_Shock& rhs) = delete;
    virtual ~CCommon_BT_Shock() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Shock* Create(void* pArg);
    virtual void Free() override;
};

END