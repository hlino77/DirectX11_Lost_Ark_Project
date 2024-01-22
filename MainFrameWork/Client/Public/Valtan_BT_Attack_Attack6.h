#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_Attack6 :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_Attack6();
    CValtan_BT_Attack_Attack6(const CValtan_BT_Attack_Attack6& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack6() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
     _bool  m_bShoot = false;

public:
    static	CValtan_BT_Attack_Attack6* Create(void* pArg);
    virtual void Free() override;
};

END