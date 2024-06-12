#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Damage1 :
    public CBT_Action
{
private:
    CCommon_BT_Damage1();
    CCommon_BT_Damage1(const CCommon_BT_Damage1& rhs) = delete;
    virtual ~CCommon_BT_Damage1() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Damage1* Create(void* pArg);
    virtual void Free() override;
};

END