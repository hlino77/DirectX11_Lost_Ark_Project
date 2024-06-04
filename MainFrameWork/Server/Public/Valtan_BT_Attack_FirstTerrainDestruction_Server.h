#pragma once

#include "Server_Defines.h"
#include "BT_Action.h"

BEGIN(Server)

class CValtan_BT_Attack_FirstTerrainDestruction_Server :
    public CBT_Action
{
private:
    CValtan_BT_Attack_FirstTerrainDestruction_Server();
    CValtan_BT_Attack_FirstTerrainDestruction_Server(const CValtan_BT_Attack_FirstTerrainDestruction_Server& rhs) = delete;
    virtual ~CValtan_BT_Attack_FirstTerrainDestruction_Server() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;
    virtual void On_FirstAnimStart()override;
    virtual void On_LastAnimEnd()override;
private:
    Vec3                m_vLandPosition = {};

public:
    static	CValtan_BT_Attack_FirstTerrainDestruction_Server* Create(void* pArg);
    virtual void Free() override;
};

END