#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CPawn_BT_Attack2 :
    public CBT_Action
{
private:
    CPawn_BT_Attack2();
    CPawn_BT_Attack2(const CPawn_BT_Attack2& rhs) = delete;
    virtual ~CPawn_BT_Attack2() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;



public:
    static	CPawn_BT_Attack2* Create(void* pArg);
    virtual void Free() override;
};

END