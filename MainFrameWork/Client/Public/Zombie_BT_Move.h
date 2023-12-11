#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CZombie_BT_Move :
    public CBT_Action
{
private:
    CZombie_BT_Move();
    CZombie_BT_Move(const CZombie_BT_Move& rhs) = delete;
    virtual ~CZombie_BT_Move() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CZombie_BT_Move* Create(void* pArg);
    virtual void Free() override;
};

END