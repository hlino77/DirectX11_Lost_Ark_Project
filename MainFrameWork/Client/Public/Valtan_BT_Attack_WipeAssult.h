#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_WipeAssult :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_WipeAssult();
    CValtan_BT_Attack_WipeAssult(const CValtan_BT_Attack_WipeAssult& rhs) = delete;
    virtual ~CValtan_BT_Attack_WipeAssult() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
    _bool     m_bShoot[2] = {};

public:
    static	CValtan_BT_Attack_WipeAssult* Create(void* pArg);
    virtual void Free() override;
};

END