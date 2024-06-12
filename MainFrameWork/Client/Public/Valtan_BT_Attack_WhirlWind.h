#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)
class CEffect;

class CValtan_BT_Attack_WhirlWind :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_WhirlWind();
    CValtan_BT_Attack_WhirlWind(const CValtan_BT_Attack_WhirlWind& rhs) = delete;
    virtual ~CValtan_BT_Attack_WhirlWind() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;


    CEffect* m_pEffect = nullptr;

    _bool m_bAttack = false;
public:
    static	CValtan_BT_Attack_WhirlWind* Create(void* pArg);
    virtual void Free() override;
};

END