#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_Move :
    public CBT_Action
{
private:
    CCommon_BT_Move();
    CCommon_BT_Move(const CCommon_BT_Move& rhs) = delete;
    virtual ~CCommon_BT_Move() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_Move* Create(void* pArg);
    virtual void Free() override;
};

END