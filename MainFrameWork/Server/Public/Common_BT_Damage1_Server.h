#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CCommon_BT_Damage1_Server :
    public CBT_Action
{
private:
    CCommon_BT_Damage1_Server();
    CCommon_BT_Damage1_Server(const CCommon_BT_Damage1_Server& rhs) = delete;
    virtual ~CCommon_BT_Damage1_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    _float m_fDuration = 0.f;

public:
    static	CCommon_BT_Damage1_Server* Create(void* pArg);
    virtual void Free() override;
};

END