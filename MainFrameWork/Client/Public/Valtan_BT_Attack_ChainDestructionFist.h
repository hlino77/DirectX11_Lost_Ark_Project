#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)
class CEffect;

class CValtan_BT_Attack_ChainDestructionFist :
    public CValtan_BT_Attack_Attack_Base
{
public:
    void    Load_TrailEffectMatrix(Matrix& matWorld);

private:
    CValtan_BT_Attack_ChainDestructionFist();
    CValtan_BT_Attack_ChainDestructionFist(const CValtan_BT_Attack_ChainDestructionFist& rhs) = delete;
    virtual ~CValtan_BT_Attack_ChainDestructionFist() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

    void    Update_Trail(_float fTimeDelta);
private:
    _bool     m_bShoot[11] = {};

    vector<CEffect*> m_Door;
    vector<CEffect*> m_DoorParticle;

    _bool m_bLandWarning[2];

    Vec3 m_vTargetPos;

    _bool m_bPizzaWarning[4];

    _bool m_bLastChopWarning = false;
    _bool m_bLastRing = false;

    _bool m_bLastShoutWarning = false;

    vector<CEffect*> m_Trails;

    _float m_fChargeTrailAcc = 0.0f;
    _float m_fChargeTrailTime = 0.0f;
public:
    static	CValtan_BT_Attack_ChainDestructionFist* Create(void* pArg);
    virtual void Free() override;
};

END