#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CValtan_BT_Spawn :
    public CBT_Action
{
private:
    CValtan_BT_Spawn();
    CValtan_BT_Spawn(const CValtan_BT_Spawn& rhs) = delete;
    virtual ~CValtan_BT_Spawn() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

    Vec3    Get_EndPos();

    void    Init_StartScene();
    void    Update_StartCamera(_float fTimeDelta);

    void    End_Scene();
private:
   _bool m_bShoot = false;

   _float m_fCameraLength;
public:
    static	CValtan_BT_Spawn* Create(void* pArg);
    virtual void Free() override;
};

END