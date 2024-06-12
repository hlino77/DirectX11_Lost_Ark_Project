#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_FistSmashExplosion :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_FistSmashExplosion();
    CValtan_BT_Attack_FistSmashExplosion(const CValtan_BT_Attack_FistSmashExplosion& rhs) = delete;
    virtual ~CValtan_BT_Attack_FistSmashExplosion() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
    _bool   m_bShoot = {};

    _bool m_bWarning = false;
    _bool m_bLastAttack = false;
    _float m_fLastAttackDelay = 0.0f;
public:
    static	CValtan_BT_Attack_FistSmashExplosion* Create(void* pArg);
    virtual void Free() override;
};

END