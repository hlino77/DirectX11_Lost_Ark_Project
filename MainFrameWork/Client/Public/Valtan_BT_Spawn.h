#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CEffect;

class CValtan_BT_Spawn :
    public CBT_Action
{

private:
    CValtan_BT_Spawn();
    CValtan_BT_Spawn(const CValtan_BT_Spawn& rhs) = delete;
    virtual ~CValtan_BT_Spawn() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(_float fTimeDelta)	override;
    virtual void		OnEnd()								override;

    Vec3    Get_EndPos();

    void    Init_StartScene();
    void    Update_StartCamera(_float fTimeDelta);

    void    End_Scene();

    void    Update_LightningParticle(_float fTimeDelta);
    void    Update_LightningParticle_AfterSpawn(_float fTimeDelta);
    void    Effect_SpawnStart();

    void    Effect_Appear();
private:
   _bool m_bShoot = false;

   _float m_fCameraLength;

   _uint m_iCameraSequence = 0;

   _float m_fCameraTurnSpeed = 0.0f;
   _float m_fCamShakeTimeAcc = 0.0f;
   _float m_fCamShakeTimeDelay = 0.0f;


   //Effect
   _float m_fLightningParticleAcc = 0.0f;
   _float m_fLightningParticleTime = 0.0f;

   vector<CEffect*> m_SpawnFloor;
   vector<CEffect*> m_BlackSmoke;
   vector<CEffect*> m_Trails;

   _bool m_bFirstInit = false;
public:
    static	CValtan_BT_Spawn* Create(void* pArg);
    virtual void Free() override;
};

END