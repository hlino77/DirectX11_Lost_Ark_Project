#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CZombie_BT_Spawn :
    public CBT_Action
{
private:
    CZombie_BT_Spawn();
    CZombie_BT_Spawn(const CZombie_BT_Spawn& rhs) = delete;
    virtual ~CZombie_BT_Spawn() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    _bool IsOut();

public:
    static	CZombie_BT_Spawn* Create(void* pArg);
    virtual void Free() override;
};

END