#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CValtan_BT_Attack_GroggyBall_Server :
    public CBT_Action
{
private:
    CValtan_BT_Attack_GroggyBall_Server();
    CValtan_BT_Attack_GroggyBall_Server(const CValtan_BT_Attack_GroggyBall_Server& rhs) = delete;
    virtual ~CValtan_BT_Attack_GroggyBall_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()	override;
    virtual void On_FirstAnimStart()override;
    virtual void On_LastAnimEnd()  override;
private:
   _bool m_bFirst = true;
   _bool m_bShield = false;

public:
    static	CValtan_BT_Attack_GroggyBall_Server* Create(void* pArg);
    virtual void Free() override;
};

END