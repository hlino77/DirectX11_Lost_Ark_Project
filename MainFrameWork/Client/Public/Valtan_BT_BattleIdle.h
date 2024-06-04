#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_BattleIdle :
    public CBT_Action
{
private:
    CValtan_BT_BattleIdle();
    CValtan_BT_BattleIdle(const CValtan_BT_BattleIdle& rhs) = delete;
    virtual ~CValtan_BT_BattleIdle() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    _float                 m_fIdletime   = 0.f;

public:
    static	CValtan_BT_BattleIdle* Create(void* pArg);
    virtual void Free() override;
};

END