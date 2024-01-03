#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_Attack_Attack18 :
    public CBT_Action
{
private:
    CValtan_BT_Attack_Attack18();
    CValtan_BT_Attack_Attack18(const CValtan_BT_Attack_Attack18& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack18() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    _int m_iLoop = 0;

public:
    static	CValtan_BT_Attack_Attack18* Create(void* pArg);
    virtual void Free() override;
};

END