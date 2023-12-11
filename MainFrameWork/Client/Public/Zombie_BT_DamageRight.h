#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CZombie_BT_DamageRight :
    public CBT_Action
{
private:
    CZombie_BT_DamageRight();
    CZombie_BT_DamageRight(const CZombie_BT_DamageRight& rhs) = delete;
    virtual ~CZombie_BT_DamageRight() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CZombie_BT_DamageRight* Create(void* pArg);
    virtual void Free() override;
};

END