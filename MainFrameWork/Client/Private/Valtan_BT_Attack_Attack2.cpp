#include "stdafx.h"
#include "Valtan_BT_Attack_Attack2.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>

CValtan_BT_Attack_Attack2::CValtan_BT_Attack_Attack2()
{
}

void CValtan_BT_Attack_Attack2::OnStart()
{
	__super::OnStart(0);
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(m_vecAnimDesc[0].strAnimName, m_vecAnimDesc[0].fChangeTime, m_vecAnimDesc[0].iStartFrame, m_vecAnimDesc[0].iChangeFrame);

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack2::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() ==m_vecAnimDesc[0].iAnimIndex)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (m_vecAnimDesc[m_iCurrAnimation].bIsLoop && m_vecAnimDesc[m_iCurrAnimation].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
	{
		if (m_fLoopTime > m_vecAnimDesc[m_iCurrAnimation].fMaxLoopTime && m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_pGameObject->Get_ModelCom()->Get_CurrAnim()))
		{
			m_iCurrAnimation++;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
			m_fLoopTime = 0;
		}
		m_fLoopTime += fTimeDelta;
	}
	else if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) - 3)
	{
		if (m_iCurrAnimation == m_iMaxAnimation - 1)
			return BT_SUCCESS;
		else
		{
			m_iCurrAnimation++;
			static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(m_vecAnimDesc[m_iCurrAnimation].strAnimName, m_vecAnimDesc[m_iCurrAnimation].fChangeTime, m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
		}
	}
	return  BT_RUNNING;
}

void CValtan_BT_Attack_Attack2::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack2* CValtan_BT_Attack_Attack2::Create(void* pArg)
{
	CValtan_BT_Attack_Attack2* pInstance = new CValtan_BT_Attack_Attack2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack2::Free()
{
	__super::Free();
}
