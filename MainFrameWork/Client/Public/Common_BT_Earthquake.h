#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Earthquake :
    public CBT_Action
{
private:
    CCommon_BT_Earthquake();
    CCommon_BT_Earthquake(const CCommon_BT_Earthquake& rhs) = delete;
    virtual ~CCommon_BT_Earthquake() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Earthquake* Create(void* pArg);
    virtual void Free() override;
};

END