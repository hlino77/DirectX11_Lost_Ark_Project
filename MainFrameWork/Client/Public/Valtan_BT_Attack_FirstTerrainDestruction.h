#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_FirstTerrainDestruction :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_FirstTerrainDestruction();
    CValtan_BT_Attack_FirstTerrainDestruction(const CValtan_BT_Attack_FirstTerrainDestruction& rhs) = delete;
    virtual ~CValtan_BT_Attack_FirstTerrainDestruction() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
    Vec3                m_vLandPosition = {};
    _bool     m_bShoot[4] = {};
public:
    static	CValtan_BT_Attack_FirstTerrainDestruction* Create(void* pArg);
    virtual void Free() override;
};

END