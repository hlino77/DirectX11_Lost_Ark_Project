#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_Attack16 :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_Attack16();
    CValtan_BT_Attack_Attack16(const CValtan_BT_Attack_Attack16& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack16() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
    Vec3                m_vLandPosition = {};
    _bool     m_bShoot[4] = {};

public:
    static	CValtan_BT_Attack_Attack16* Create(void* pArg);
    virtual void Free() override;
};

END