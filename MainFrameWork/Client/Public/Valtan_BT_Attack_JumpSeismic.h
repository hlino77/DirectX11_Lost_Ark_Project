#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_JumpSeismic :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_JumpSeismic();
    CValtan_BT_Attack_JumpSeismic(const CValtan_BT_Attack_JumpSeismic& rhs) = delete;
    virtual ~CValtan_BT_Attack_JumpSeismic() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
    _bool m_bShoot = {};

public:
    static	CValtan_BT_Attack_JumpSeismic* Create(void* pArg);
    virtual void Free() override;
};

END