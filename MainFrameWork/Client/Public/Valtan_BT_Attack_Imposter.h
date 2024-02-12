#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_Imposter :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_Imposter();
    CValtan_BT_Attack_Imposter(const CValtan_BT_Attack_Imposter& rhs) = delete;
    virtual ~CValtan_BT_Attack_Imposter() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;
private:
    void    Update_Camera(_float fTimeDelta);
private:
    _bool     m_bShoot[4] = {};

    _bool       m_bWarning[1];
    _bool       m_bShoutEffect = false;

    _uint       m_iCameraSequence = 0;

    Vec3        m_vCamPos, m_vCamTargetPos;

    _bool       m_bJumpEffect = false;
public:
    static	CValtan_BT_Attack_Imposter* Create(void* pArg);
    virtual void Free() override;
};

END