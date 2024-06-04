#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CValtan_BT_Attack_Imprisonment_Server :
    public CBT_Action
{
private:
    CValtan_BT_Attack_Imprisonment_Server();
    CValtan_BT_Attack_Imprisonment_Server(const CValtan_BT_Attack_Imprisonment_Server& rhs) = delete;
    virtual ~CValtan_BT_Attack_Imprisonment_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;
    void Add_Prison();
private:
    _int  m_iTargetID = {};

public:
    static	CValtan_BT_Attack_Imprisonment_Server* Create(void* pArg);
    virtual void Free() override;
};

END