#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CPlant_BT_Attack_Shake :
    public CBT_Action
{
private:
    CPlant_BT_Attack_Shake();
    CPlant_BT_Attack_Shake(const CPlant_BT_Attack_Shake& rhs) = delete;
    virtual ~CPlant_BT_Attack_Shake() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CPlant_BT_Attack_Shake* Create(void* pArg);
    virtual void Free() override;
};

END