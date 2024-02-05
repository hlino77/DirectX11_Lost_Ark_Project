#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_GhostGrab :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_GhostGrab();
    CValtan_BT_Attack_GhostGrab(const CValtan_BT_Attack_GhostGrab& rhs) = delete;
    virtual ~CValtan_BT_Attack_GhostGrab() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
    _bool        m_bShoot = false;

public:
    static	CValtan_BT_Attack_GhostGrab* Create(void* pArg);
    virtual void Free() override;
};

END