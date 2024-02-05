#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_TeleportRush :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_TeleportRush();
    CValtan_BT_Attack_TeleportRush(const CValtan_BT_Attack_TeleportRush& rhs) = delete;
    virtual ~CValtan_BT_Attack_TeleportRush() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    Vec3 m_vDirection;
    _int m_iStack=0;
public:
    static	CValtan_BT_Attack_TeleportRush* Create(void* pArg);
    virtual void Free() override;
};

END