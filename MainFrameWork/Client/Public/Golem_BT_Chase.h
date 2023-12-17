#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CGolem_BT_Chase :
    public CBT_Action
{
private:
    CGolem_BT_Chase();
    CGolem_BT_Chase(const CGolem_BT_Chase& rhs) = delete;
    virtual ~CGolem_BT_Chase() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CGolem_BT_Chase* Create(void* pArg);
    virtual void Free() override;
};

END