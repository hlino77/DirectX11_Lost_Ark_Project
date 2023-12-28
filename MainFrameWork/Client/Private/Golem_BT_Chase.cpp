#include "stdafx.h"
#include "Golem_BT_Chase.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"

CGolem_BT_Chase::CGolem_BT_Chase()
{
}

void CGolem_BT_Chase::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CGolem_BT_Chase::OnUpdate(const _float& fTimeDelta)
{

	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[0].iAnimIndex))
		return BT_FAIL;

	if (static_cast<CMonster*>(m_pGameObject)->Get_Target_Distance() <0.7f)
	{
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
		if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimDesc[1].iAnimIndex &&m_pGameObject->Get_ModelCom()->Get_NextAnim() != m_vecAnimDesc[1].iAnimIndex)
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[1].iAnimIndex, m_vecAnimDesc[1].fChangeTime,
			m_vecAnimDesc[1].iStartFrame, m_vecAnimDesc[1].iChangeFrame);
		return BT_RUNNING;
	}
	else if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimDesc[0].iAnimIndex&& m_pGameObject->Get_ModelCom()->Get_NextAnim()	!= m_vecAnimDesc[0].iAnimIndex)
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[0].iAnimIndex, m_vecAnimDesc[0].fChangeTime,
			m_vecAnimDesc[0].iStartFrame, m_vecAnimDesc[0].iChangeFrame);
	_float fSpeed = 1.5f;
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == 34)
	{
		_int Animframe = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(34); 
		if (30 > Animframe % 30 && Animframe % 30 > 20)
		{
			fSpeed = Vec2::Lerp(Vec2(0.f, 0.f), Vec2(0.5f, 0.f), (Animframe % 30 - 20) / 10).x;
		}
	}
	static_cast<CMonster*>(m_pGameObject)->Move_Dir(static_cast<CMonster*>(m_pGameObject)->Get_Target_Direction(), fSpeed, fTimeDelta);

	return BT_RUNNING;
}

void CGolem_BT_Chase::OnEnd()
{
	__super::OnEnd();
}



CGolem_BT_Chase* CGolem_BT_Chase::Create(void* pArg)
{
	CGolem_BT_Chase* pInstance = new CGolem_BT_Chase;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Chase::Free()
{
	__super::Free();
}
