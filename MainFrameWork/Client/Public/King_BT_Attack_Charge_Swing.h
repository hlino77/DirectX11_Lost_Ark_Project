#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CKing_BT_Attack_Charge_Swing :
    public CBT_Action
{
private:
    CKing_BT_Attack_Charge_Swing();
    CKing_BT_Attack_Charge_Swing(const CKing_BT_Attack_Charge_Swing& rhs) = delete;
    virtual ~CKing_BT_Attack_Charge_Swing() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CKing_BT_Attack_Charge_Swing* Create(void* pArg);
    virtual void Free() override;
};

END