#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CZombie_BT_DamageLeft :
    public CBT_Action
{
private:
    CZombie_BT_DamageLeft();
    CZombie_BT_DamageLeft(const CZombie_BT_DamageLeft& rhs) = delete;
    virtual ~CZombie_BT_DamageLeft() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CZombie_BT_DamageLeft* Create(void* pArg);
    virtual void Free() override;
};

END