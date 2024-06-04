#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_Phase2 :
    public CBT_Action
{
    enum class CameraState { Ready, Start, ZoomOut, CameraEnd };
private:
    CValtan_BT_Phase2();
    CValtan_BT_Phase2(const CValtan_BT_Phase2& rhs) = delete;
    virtual ~CValtan_BT_Phase2() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    void    Update_Camera(_float fTimeDelta);

private:
    CameraState m_eCameraState;

    _bool m_bBreak = false;
    _float m_fCameraSpeed = 0.0f;
    _bool m_bRadial = false;

    _float m_fCamShakeTimeAcc = 0.0f;
    _float m_fCamShakeTimeDelay = 0.0f;

    _bool  m_bSendServerNaviInfo = false;
public:
    static	CValtan_BT_Phase2* Create(void* pArg);
    virtual void Free() override;
};

END