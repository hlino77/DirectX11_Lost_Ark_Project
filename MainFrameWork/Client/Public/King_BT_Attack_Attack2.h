#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CKing_BT_Attack_Attack2 :
    public CBT_Action
{
private:
    CKing_BT_Attack_Attack2();
    CKing_BT_Attack_Attack2(const CKing_BT_Attack_Attack2& rhs) = delete;
    virtual ~CKing_BT_Attack_Attack2() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CKing_BT_Attack_Attack2* Create(void* pArg);
    virtual void Free() override;
};

END