#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CPlant_BT_Attack_Root :
    public CBT_Action
{
private:
    CPlant_BT_Attack_Root();
    CPlant_BT_Attack_Root(const CPlant_BT_Attack_Root& rhs) = delete;
    virtual ~CPlant_BT_Attack_Root() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CPlant_BT_Attack_Root* Create(void* pArg);
    virtual void Free() override;
};

END