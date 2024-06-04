#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_Dead :
    public CBT_Action
{
private:
    CValtan_BT_Dead();
    CValtan_BT_Dead(const CValtan_BT_Dead& rhs) = delete;
    virtual ~CValtan_BT_Dead() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;


    void    Update_Camera(_float fTimeDelta);
private:

    _bool		m_bDeadEffect = false;

    _uint       m_iCameraSequence = 0;

    Vec3        m_vCameraPos, m_vCamTargetPos;

    _float      m_fSlowMotionSpeed = 0.0f;

    _float      m_fAnimSpeed;
public:
    static	CValtan_BT_Dead* Create(void* pArg);
    virtual void Free() override;
};

END