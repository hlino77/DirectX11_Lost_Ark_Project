#pragma once

#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CKing_BT_Attack_Erruption :
    public CBT_Action
{
private:
    CKing_BT_Attack_Erruption();
    CKing_BT_Attack_Erruption(const CKing_BT_Attack_Erruption& rhs) = delete;
    virtual ~CKing_BT_Attack_Erruption() =  default;

    virtual void		OnStart()							override;
    virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
    virtual void		OnEnd()								override;

private:
    _bool m_Shoot = false;
    _bool m_bLookatRight;
public:
    static	CKing_BT_Attack_Erruption* Create(void* pArg);
    virtual void Free() override;
};

END