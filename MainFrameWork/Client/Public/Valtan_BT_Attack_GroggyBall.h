#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_Attack_GroggyBall :
    public CBT_Action
{
private:
    CValtan_BT_Attack_GroggyBall();
    CValtan_BT_Attack_GroggyBall(const CValtan_BT_Attack_GroggyBall& rhs) = delete;
    virtual ~CValtan_BT_Attack_GroggyBall() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()	override;
    virtual void On_FirstAnimStart()override;
    virtual void On_LastAnimEnd()  override;
private:
    _bool m_bShoot = false;
    _bool m_bShield = false;
    vector<class CEffect*>    m_vecEffect;
public:
    static	CValtan_BT_Attack_GroggyBall* Create(void* pArg);
    virtual void Free() override;
};

END