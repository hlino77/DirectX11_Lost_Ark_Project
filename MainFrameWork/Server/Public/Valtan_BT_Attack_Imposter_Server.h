#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CValtan_BT_Attack_Imposter_Server :
    public CBT_Action
{
private:
    CValtan_BT_Attack_Imposter_Server();
    CValtan_BT_Attack_Imposter_Server(const CValtan_BT_Attack_Imposter_Server& rhs) = delete;
    virtual ~CValtan_BT_Attack_Imposter_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
   virtual void On_FirstAnimStart()override;
   virtual void On_LastAnimEnd()  override;
    virtual void		OnEnd()								override;


private:
    Vec3 m_vTargetPos;
public:
    static	CValtan_BT_Attack_Imposter_Server* Create(void* pArg);
    virtual void Free() override;
};

END