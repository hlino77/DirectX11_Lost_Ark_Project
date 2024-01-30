#include "stdafx.h"
#include "Valtan_BT_Phase2.h"
#include "Monster.h"
#include "Model.h"
#include <Boss_Valtan.h>
#include "Transform.h"
#include <AnimModel.h>
#include "GameInstance.h"
#include "Player.h"
CValtan_BT_Phase2::CValtan_BT_Phase2()
{
}

void CValtan_BT_Phase2::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CValtan_BT_Phase2::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) > 41 && m_iCurrAnimation == 3)
		for (auto iter : CGameInstance::GetInstance()->GetInstance()->Find_GameObjects(m_pGameObject->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_BACKGROUND))
		{
			if (iter->Get_ModelCom()->Get_ModelType() == CModel::TYPE::TYPE_ANIM &&
				iter->Get_ModelName() == TEXT("Wall01") ||
				iter->Get_ModelName() == TEXT("Wall02") ||
				iter->Get_ModelName() == TEXT("Wall03") ||
				iter->Get_ModelName() == TEXT("Wall04"))
				static_cast<CAnimModel*>(iter)->Set_PlayAnim(true);

		}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) > 77 && m_iCurrAnimation == 3)
	{
		static_cast<CBoss*>(m_pGameObject)->Set_Invincible(false);

		m_iCurrAnimation++;

		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
		m_pGameObject->Get_ModelCom()->Set_Anim_Speed(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fAnimSpeed);

	}
	if (m_iCurrAnimation == 3)
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_Render(false);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_iCurrAnimation == 4)
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_Render(true);


	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Phase2::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss*>(m_pGameObject)->Set_Phase(2);
	static_cast<CPlayer*>(CGameInstance::GetInstance()->Find_CtrlPlayer(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_PLAYER))->Set_CurValtanPhase(2);
}



CValtan_BT_Phase2* CValtan_BT_Phase2::Create(void* pArg)
{
	CValtan_BT_Phase2* pInstance = new CValtan_BT_Phase2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Phase2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Phase2::Free()
{
	__super::Free();
}
