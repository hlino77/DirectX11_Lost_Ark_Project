#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)
class CEffect;

class CValtan_BT_Attack_SecondTerrainDestruction :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_SecondTerrainDestruction();
    CValtan_BT_Attack_SecondTerrainDestruction(const CValtan_BT_Attack_SecondTerrainDestruction& rhs) = delete;
    virtual ~CValtan_BT_Attack_SecondTerrainDestruction() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
    Vec3                m_vLandPosition = {};
    _bool     m_bShoot[4] = {};

    _bool m_bTerrainWarning = false;

    CEffect* m_pWhirlWind = nullptr;
    _bool m_bWhirlWind = false;

    CEffect* m_pWarning3 = nullptr;

    _bool m_bLastAttack = false;
public:
    static	CValtan_BT_Attack_SecondTerrainDestruction* Create(void* pArg);
    virtual void Free() override;
};

END