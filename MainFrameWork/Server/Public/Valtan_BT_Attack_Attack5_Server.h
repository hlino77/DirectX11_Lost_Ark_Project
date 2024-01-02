#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CValtan_BT_Attack_Attack5_Server :
    public CBT_Action
{
private:
    CValtan_BT_Attack_Attack5_Server();
    CValtan_BT_Attack_Attack5_Server(const CValtan_BT_Attack_Attack5_Server& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack5_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    Vec3 m_vDirection;
    _int m_iStack=0;
public:
    static	CValtan_BT_Attack_Attack5_Server* Create(void* pArg);
    virtual void Free() override;
};

END