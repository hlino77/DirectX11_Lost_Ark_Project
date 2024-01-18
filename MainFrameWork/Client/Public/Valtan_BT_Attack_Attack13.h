#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_Attack13 :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_Attack13();
    CValtan_BT_Attack_Attack13(const CValtan_BT_Attack_Attack13& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack13() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:

    _bool   m_bOutSide;
public:
    static	CValtan_BT_Attack_Attack13* Create(void* pArg);
    virtual void Free() override;
};

END