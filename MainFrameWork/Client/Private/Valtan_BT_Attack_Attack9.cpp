#include "stdafx.h"
#include "Valtan_BT_Attack_Attack9.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack9::CValtan_BT_Attack_Attack9()
{
}

void CValtan_BT_Attack_Attack9::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack9::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex)<10 || m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta*2.f);
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 115&& m_iCurrAnimation==0)
	{
		m_iCurrAnimation = 1;

		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);

	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack9::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack9* CValtan_BT_Attack_Attack9::Create(void* pArg)
{
	CValtan_BT_Attack_Attack9* pInstance = new CValtan_BT_Attack_Attack9;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack9::Free()
{
	__super::Free();
}
