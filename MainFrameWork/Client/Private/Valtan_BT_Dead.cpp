#include "stdafx.h"
#include "Valtan_BT_Dead.h"
#include "Monster.h"
#include "Model.h"
#include "GameInstance.h"

CValtan_BT_Dead::CValtan_BT_Dead()
{
}

void CValtan_BT_Dead::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->Deactivate_AllColliders();
	static_cast<CMonster*>(m_pGameObject)->Disable_HpUI();
}

CBT_Node::BT_RETURN CValtan_BT_Dead::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[5].iAnimIndex) >= 34 && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_iCurrAnimation++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
	}

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Dead::OnEnd()
{
	__super::OnEnd();

	static_cast<CMonster*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster*>(m_pGameObject)->Set_Left(false);
	static_cast<CMonster*>(m_pGameObject)->Set_AnimationSpeed(0.f);
	static_cast<CMonster*>(m_pGameObject)->Set_Die(4.f);
}



CValtan_BT_Dead* CValtan_BT_Dead::Create(void* pArg)
{
	CValtan_BT_Dead* pInstance = new CValtan_BT_Dead;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Dead::Free()
{
	__super::Free();
}
