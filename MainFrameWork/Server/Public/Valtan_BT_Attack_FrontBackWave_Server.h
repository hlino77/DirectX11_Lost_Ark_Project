#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CValtan_BT_Attack_FrontBackWave_Server :
    public CBT_Action
{
private:
    CValtan_BT_Attack_FrontBackWave_Server();
    CValtan_BT_Attack_FrontBackWave_Server(const CValtan_BT_Attack_FrontBackWave_Server& rhs) = delete;
    virtual ~CValtan_BT_Attack_FrontBackWave_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    _bool m_bShoot = true;
public:
    static	CValtan_BT_Attack_FrontBackWave_Server* Create(void* pArg);
    virtual void Free() override;
};

END