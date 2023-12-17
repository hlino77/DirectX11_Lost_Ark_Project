#include "stdafx.h"
#include "Golem_BT_Attack_Dash.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
CGolem_BT_Attack_Dash::CGolem_BT_Attack_Dash()
{
}

void CGolem_BT_Attack_Dash::OnStart()
{
	cout << endl << "Å¬¶ó °ñ·½" << "	|	" << m_pGameObject->Get_ModelCom()->Get_CurrAnim() << "	|	" << m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_pGameObject->Get_ModelCom()->Get_CurrAnim()) << endl;
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CGolem_BT_Attack_Dash::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_SUCCESS;
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[0].first.iAnimIndex)<10)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction(fTimeDelta);
	return BT_RUNNING;
}

void CGolem_BT_Attack_Dash::OnEnd()
{
	__super::OnEnd();
	cout << endl << "Å¬¶ó °ñ·½" << "	|	" << m_pGameObject->Get_ModelCom()->Get_CurrAnim() << "	|	" << m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_pGameObject->Get_ModelCom()->Get_CurrAnim()) << endl;
	static_cast<CMonster*>(m_pGameObject)->Set_AttackRange(0);

}



CGolem_BT_Attack_Dash* CGolem_BT_Attack_Dash::Create(void* pArg)
{
	CGolem_BT_Attack_Dash* pInstance = new CGolem_BT_Attack_Dash;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Dash::Free()
{
	__super::Free();
}
