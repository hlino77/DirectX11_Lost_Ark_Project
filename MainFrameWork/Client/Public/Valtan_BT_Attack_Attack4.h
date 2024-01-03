#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_Attack_Attack4 :
    public CBT_Action
{
private:
    CValtan_BT_Attack_Attack4();
    CValtan_BT_Attack_Attack4(const CValtan_BT_Attack_Attack4& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack4() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CValtan_BT_Attack_Attack4* Create(void* pArg);
    virtual void Free() override;
};

END