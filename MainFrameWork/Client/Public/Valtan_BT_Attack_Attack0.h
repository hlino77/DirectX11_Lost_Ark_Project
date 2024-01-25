#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_Attack0 :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_Attack0();
    CValtan_BT_Attack_Attack0(const CValtan_BT_Attack_Attack0& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack0() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
    void Add_Stone(Vec3 vPosition);
private:
    Vec3                m_vLandPosition = {};
    _bool     m_bShoot[4] = {};
public:
    static	CValtan_BT_Attack_Attack0* Create(void* pArg);
    virtual void Free() override;
};

END