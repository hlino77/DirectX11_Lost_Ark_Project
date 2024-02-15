#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)
class CEffect;

class CValtan_BT_Attack_SwingSeismic :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_SwingSeismic();
    CValtan_BT_Attack_SwingSeismic(const CValtan_BT_Attack_SwingSeismic& rhs) = delete;
    virtual ~CValtan_BT_Attack_SwingSeismic() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    _bool m_bShoot = {};

    _bool m_bSwing = false;
public:
    static	CValtan_BT_Attack_SwingSeismic* Create(void* pArg);
    virtual void Free() override;
};

END