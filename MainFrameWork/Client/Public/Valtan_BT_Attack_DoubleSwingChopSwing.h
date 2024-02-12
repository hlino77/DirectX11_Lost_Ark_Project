#pragma once

#include "Client_Defines.h"
#include "Valtan_BT_Attack_Attack_Base.h"

BEGIN(Client)

class CValtan_BT_Attack_DoubleSwingChopSwing :
    public CValtan_BT_Attack_Attack_Base
{
private:
    CValtan_BT_Attack_DoubleSwingChopSwing();
    CValtan_BT_Attack_DoubleSwingChopSwing(const CValtan_BT_Attack_DoubleSwingChopSwing& rhs) = delete;
    virtual ~CValtan_BT_Attack_DoubleSwingChopSwing() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

    void    Update_Effect();
private:
    _bool m_bShoot[2] = {};
    _float m_fDounutRadii[2] = { 8.f , 6.3f };
    _uint m_iEffectSequence = 0;
public:
    static	CValtan_BT_Attack_DoubleSwingChopSwing* Create(void* pArg);
    virtual void Free() override;
};

END