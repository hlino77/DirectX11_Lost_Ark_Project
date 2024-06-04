#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_FrontBackWave :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_FrontBackWave();
    CValtan_BT_Attack_FrontBackWave(const CValtan_BT_Attack_FrontBackWave& rhs) = delete;
    virtual ~CValtan_BT_Attack_FrontBackWave() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    _bool   m_bShoot[3] = {};
public:
    static	CValtan_BT_Attack_FrontBackWave* Create(void* pArg);
    virtual void Free() override;
};

END