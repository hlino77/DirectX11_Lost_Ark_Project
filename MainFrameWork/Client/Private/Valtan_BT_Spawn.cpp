#include "stdafx.h"
#include "Valtan_BT_Spawn.h"
#include "Boss_Valtan.h"
#include "Model.h"
CValtan_BT_Spawn::CValtan_BT_Spawn()
{
}

void CValtan_BT_Spawn::OnStart()
{
	__super::OnStart(0);
	m_bShoot = true;
}

CBT_Node::BT_RETURN CValtan_BT_Spawn::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex&& m_pGameObject->Is_Render()==false && m_fLoopTime>2.f)
	{
		m_pGameObject->Set_Render(true);
		static_cast<CBoss*>(m_pGameObject)->Set_HpUIRender(true);
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_Render(true);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 30 && static_cast<CMonster*>(m_pGameObject)->Get_DissolveIn() == false&& m_bShoot)
	{
		m_bShoot = false;
		static_cast<CMonster*>(m_pGameObject)->Set_DissolveIn(4.f);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Spawn::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Spawn(false);
}



CValtan_BT_Spawn* CValtan_BT_Spawn::Create(void* pArg)
{
	CValtan_BT_Spawn* pInstance = new CValtan_BT_Spawn;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Spawn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Spawn::Free()
{
	__super::Free();
}
