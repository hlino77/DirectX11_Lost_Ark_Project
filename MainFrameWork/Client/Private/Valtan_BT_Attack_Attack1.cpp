#include "stdafx.h"
#include "Valtan_BT_Attack_Attack1.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include "NavigationMgr.h"
#include <Boss.h>
CValtan_BT_Attack_Attack1::CValtan_BT_Attack_Attack1()
{
}

void CValtan_BT_Attack_Attack1::OnStart()
{
	__super::OnStart(0);
	m_iLoop = 0;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack1::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_iLoop < 2)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 48 && m_iLoop < 2)
	{
		m_iLoop++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 48 && m_iLoop < 3)
	{
		m_iLoop++;
		m_iCurrAnimation = 1;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[1].iAnimIndex, m_vecAnimDesc[1].fChangeTime, m_vecAnimDesc[1].iStartFrame, m_vecAnimDesc[1].iChangeFrame);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex&& m_iCurrAnimation==1)
	{
		m_pGameObject->Get_TransformCom()->Go_Straight(static_cast<CMonster*>(m_pGameObject)->Get_MoveSpeed() * 3.f, fTimeDelta);
		if (CNavigationMgr::GetInstance()->Is_Outside(m_pGameObject->Get_CurrLevel(), m_pGameObject, 2.f))
		{
			m_iCurrAnimation = 2;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[2].iAnimIndex, m_vecAnimDesc[2].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[2].iChangeFrame);
			//_int iArmor = static_cast<CBoss*>(m_pGameObject)->Get_Armor();
			//if (iArmor != 0)
			//	static_cast<CBoss*>(m_pGameObject)->Set_Armor(iArmor - 1);
			//else
			//{
			//	_int iPhase = static_cast<CBoss*>(m_pGameObject)->Get_Phase();
			//	if (iPhase != 3)
			//		static_cast<CBoss*>(m_pGameObject)->Set_Phase(iPhase + 1);
			//}
		}
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack1::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack1* CValtan_BT_Attack_Attack1::Create(void* pArg)
{
	CValtan_BT_Attack_Attack1* pInstance = new CValtan_BT_Attack_Attack1;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack1::Free()
{
	__super::Free();
}
