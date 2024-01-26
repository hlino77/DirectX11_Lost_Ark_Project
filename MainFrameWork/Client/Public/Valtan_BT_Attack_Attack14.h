#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_Attack14 :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_Attack14();
    CValtan_BT_Attack_Attack14(const CValtan_BT_Attack_Attack14& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack14() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
    _uint               m_iAttackCount = 0;
    _float              m_fAttackTerm = 0.f;
    _bool               m_bShoot =true;
public:
    static	CValtan_BT_Attack_Attack14* Create(void* pArg);
    virtual void Free() override;
};

END