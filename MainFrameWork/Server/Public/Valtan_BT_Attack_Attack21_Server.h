#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CValtan_BT_Attack_Attack21_Server :
    public CBT_Action
{
private:
    CValtan_BT_Attack_Attack21_Server();
    CValtan_BT_Attack_Attack21_Server(const CValtan_BT_Attack_Attack21_Server& rhs) = delete;
    virtual ~CValtan_BT_Attack_Attack21_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    void Add_Stone(Vec3 vPosition);
   virtual void On_FirstAnimStart()override;
   virtual void On_LastAnimEnd()  override;
    virtual void		OnEnd()								override;

private:
    _bool   m_bShoot = true;

public:
    static	CValtan_BT_Attack_Attack21_Server* Create(void* pArg);
    virtual void Free() override;
};

END