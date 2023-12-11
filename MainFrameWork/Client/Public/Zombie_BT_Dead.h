#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CZombie_BT_Dead :
    public CBT_Action
{
private:
    CZombie_BT_Dead();
    CZombie_BT_Dead(const CZombie_BT_Dead& rhs) = delete;
    virtual ~CZombie_BT_Dead() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CZombie_BT_Dead* Create(void* pArg);
    virtual void Free() override;
};

END