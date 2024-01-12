#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CGolem_BT_Attack_Jump :
    public CBT_Action
{
private:
    CGolem_BT_Attack_Jump();
    CGolem_BT_Attack_Jump(const CGolem_BT_Attack_Jump& rhs) = delete;
    virtual ~CGolem_BT_Attack_Jump() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
    _bool 	m_Shoot = true;

public:
    static	CGolem_BT_Attack_Jump* Create(void* pArg);
    virtual void Free() override;
};

END