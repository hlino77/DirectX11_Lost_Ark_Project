#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_Attack_Attack_Base :
    public CBT_Action
{
protected:
    CValtan_BT_Attack_Attack_Base();
    CValtan_BT_Attack_Attack_Base(const CValtan_BT_Attack_Attack_Base& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack_Base() =  default;

    virtual void		OnStart();
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta);
    virtual void		OnEnd();

public:
    static	CValtan_BT_Attack_Attack_Base* Create(void* pArg);
    virtual void Free() override;
};

END