#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_ChainDestructionFist :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_ChainDestructionFist();
    CValtan_BT_Attack_ChainDestructionFist(const CValtan_BT_Attack_ChainDestructionFist& rhs) = delete;
    virtual ~CValtan_BT_Attack_ChainDestructionFist() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
    _bool     m_bShoot[11] = {};

public:
    static	CValtan_BT_Attack_ChainDestructionFist* Create(void* pArg);
    virtual void Free() override;
};

END