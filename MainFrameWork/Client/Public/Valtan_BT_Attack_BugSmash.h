#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_BugSmash :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_BugSmash();
    CValtan_BT_Attack_BugSmash(const CValtan_BT_Attack_BugSmash& rhs) = delete;
    virtual ~CValtan_BT_Attack_BugSmash() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    _bool m_bShoot[2] = {true,true };

public:
    static	CValtan_BT_Attack_BugSmash* Create(void* pArg);
    virtual void Free() override;
};

END