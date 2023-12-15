#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CGolem_BT_Attack_Swipe :
    public CBT_Action
{
private:
    CGolem_BT_Attack_Swipe();
    CGolem_BT_Attack_Swipe(const CGolem_BT_Attack_Swipe& rhs) = delete;
    virtual ~CGolem_BT_Attack_Swipe() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CGolem_BT_Attack_Swipe* Create(void* pArg);
    virtual void Free() override;
};

END