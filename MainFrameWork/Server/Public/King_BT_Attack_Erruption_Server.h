#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CKing_BT_Attack_Erruption_Server :
    public CBT_Action
{
private:
    CKing_BT_Attack_Erruption_Server();
    CKing_BT_Attack_Erruption_Server(const CKing_BT_Attack_Erruption_Server& rhs) = delete;
    virtual ~CKing_BT_Attack_Erruption_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;


public:
    static	CKing_BT_Attack_Erruption_Server* Create(void* pArg);
    virtual void Free() override;
};

END