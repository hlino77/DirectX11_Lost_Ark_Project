#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CGolem_BT_Attack_Dash :
    public CBT_Action
{
private:
    CGolem_BT_Attack_Dash();
    CGolem_BT_Attack_Dash(const CGolem_BT_Attack_Dash& rhs) = delete;
    virtual ~CGolem_BT_Attack_Dash() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CGolem_BT_Attack_Dash* Create(void* pArg);
    virtual void Free() override;
};

END