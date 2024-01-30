#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_Dead :
    public CBT_Action
{
private:
    CValtan_BT_Dead();
    CValtan_BT_Dead(const CValtan_BT_Dead& rhs) = delete;
    virtual ~CValtan_BT_Dead() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CValtan_BT_Dead* Create(void* pArg);
    virtual void Free() override;
};

END