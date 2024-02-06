#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_Spawn :
    public CBT_Action
{
private:
    CValtan_BT_Spawn();
    CValtan_BT_Spawn(const CValtan_BT_Spawn& rhs) = delete;
    virtual ~CValtan_BT_Spawn() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
   _bool m_bShoot = false;
public:
    static	CValtan_BT_Spawn* Create(void* pArg);
    virtual void Free() override;
};

END