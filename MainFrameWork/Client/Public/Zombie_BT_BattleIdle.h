#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CZombie_BT_BattleIdle :
    public CBT_Action
{
private:
    CZombie_BT_BattleIdle();
    CZombie_BT_BattleIdle(const CZombie_BT_BattleIdle& rhs) = delete;
    virtual ~CZombie_BT_BattleIdle() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CZombie_BT_BattleIdle* Create(void* pArg);
    virtual void Free() override;
};

END