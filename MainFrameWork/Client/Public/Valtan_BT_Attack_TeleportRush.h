#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)
class CEffect;


class CValtan_BT_Attack_TeleportRush :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_TeleportRush();
    CValtan_BT_Attack_TeleportRush(const CValtan_BT_Attack_TeleportRush& rhs) = delete;
    virtual ~CValtan_BT_Attack_TeleportRush() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;

    void        Open_Start_Door();
    void        Update_Loop_Door(_bool bOpen);
    void        Open_End_Door();
   
private:
    Vec3 m_vDirection;
    _int m_iStack=0;

    vector<CEffect*> m_Door;
    vector<CEffect*> m_DoorParticle;

    vector<CEffect*> m_RushEffect;
public:
    static	CValtan_BT_Attack_TeleportRush* Create(void* pArg);
    virtual void Free() override;
};

END