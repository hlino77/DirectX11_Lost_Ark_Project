#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_Imprisonment :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_Imprisonment();
    CValtan_BT_Attack_Imprisonment(const CValtan_BT_Attack_Imprisonment& rhs) = delete;
    virtual ~CValtan_BT_Attack_Imprisonment() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

public:
    virtual void On_FirstAnimStart()    override;

private:
    void    Update_Effect();

private:
    _bool               m_bShoot = true;
    _bool                m_bEffectStart[2] = { false, false };

public:
    static	CValtan_BT_Attack_Imprisonment* Create(void* pArg);
    virtual void Free() override;
};

END