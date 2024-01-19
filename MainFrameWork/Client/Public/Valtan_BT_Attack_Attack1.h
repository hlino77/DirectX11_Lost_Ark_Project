#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_Attack1 :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_Attack1();
    CValtan_BT_Attack_Attack1(const CValtan_BT_Attack_Attack1& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack1() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
    _int   m_iLoop = 0;

public:
    static	CValtan_BT_Attack_Attack1* Create(void* pArg);
    virtual void Free() override;
};

END