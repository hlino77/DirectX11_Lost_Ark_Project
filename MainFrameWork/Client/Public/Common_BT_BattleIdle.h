#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CCommon_BT_BattleIdle :
    public CBT_Action
{
private:
    CCommon_BT_BattleIdle();
    CCommon_BT_BattleIdle(const CCommon_BT_BattleIdle& rhs) = delete;
    virtual ~CCommon_BT_BattleIdle() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CCommon_BT_BattleIdle* Create(void* pArg);
    virtual void Free() override;
};

END