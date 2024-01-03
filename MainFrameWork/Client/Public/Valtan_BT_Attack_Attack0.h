#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_Attack_Attack0 :
    public CBT_Action
{
private:
    CValtan_BT_Attack_Attack0();
    CValtan_BT_Attack_Attack0(const CValtan_BT_Attack_Attack0& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack0() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
    Vec3                m_vLandPosition = {};

public:
    static	CValtan_BT_Attack_Attack0* Create(void* pArg);
    virtual void Free() override;
};

END