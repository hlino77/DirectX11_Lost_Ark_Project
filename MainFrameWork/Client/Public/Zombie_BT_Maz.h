#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CZombie_BT_Maz :
    public CBT_Action
{
private:
    CZombie_BT_Maz();
    CZombie_BT_Maz(const CZombie_BT_Maz& rhs) = delete;
    virtual ~CZombie_BT_Maz() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CZombie_BT_Maz* Create(void* pArg);
    virtual void Free() override;
};

END