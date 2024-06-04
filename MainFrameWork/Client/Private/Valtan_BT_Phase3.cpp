#include "stdafx.h"
#include "Valtan_BT_Phase3.h"
#include "Monster.h"
#include "Model.h"
#include <Boss.h>
#include <Player.h>
#include "GameInstance.h"
#include <Boss_Valtan.h>
#include "ServerSessionManager.h"
#include "Sound_Manager.h"

CValtan_BT_Phase3::CValtan_BT_Phase3()
{
}

void CValtan_BT_Phase3::OnStart()
{
	__super::OnStart(0);
	static_cast<CBoss*>(m_pGameObject)->Set_Phase(3);
	CServerSessionManager::GetInstance()->Get_Player()->Set_CurValtanPhase(3);
	static_cast<CBoss*>(m_pGameObject)->Set_GroggyLock(true);
	(m_pGameObject)->Set_Render(false);
	static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_Render(false);
	wstring szComponentName = L"Boss_Valtan_Corpse";
	CBoss::MODELDESC Desc;
	Desc.strFileName = L"Boss_Valtan_Corpse";
	Desc.iLayer = (_uint)LAYER_TYPE::LAYER_MONSTER;
	Desc.iObjectID = -1;
	Desc.iLevel = m_pGameObject->Get_CurrLevel();
	Desc.vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	wstring szMonsterName = L"Prototype_GameObject_" + szComponentName;
	CBoss* pCorpse = dynamic_cast<CBoss*>(CGameInstance::GetInstance()->Add_GameObject(Desc.iLevel, Desc.iLayer, szMonsterName, &Desc));
	pCorpse->Get_TransformCom()->LookAt_Dir(m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK));
	Add_Sound(L"Effect", L"Valtan#161 (23475606)");
}

CBT_Node::BT_RETURN CValtan_BT_Phase3::OnUpdate(_float fTimeDelta)
{

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Phase3::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss*>(m_pGameObject)->Set_MaxHp(1588207017);
	static_cast<CBoss*>(m_pGameObject)->Set_Hp(1588207017);
	static_cast<CBoss_Valtan*>(m_pGameObject)->Set_HpUI(40, 1588207017, L"찢겨진 마수의 군주");
	static_cast<CBoss_Valtan*>(m_pGameObject)->Set_HpUIRender(true);
	CSound_Manager::GetInstance()->Stop_Channel_Sound(L"Commander of Beast Valtan");
	CSound_Manager::GetInstance()->PlayBGM_AddChannel(L"Ravaged Tyrant of Beasts", L"BGM", L"Ravaged Tyrant of Beasts.wav", 0.3f);
}

void CValtan_BT_Phase3::On_FirstAnimStart()
{
	static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Render(true);
	static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_Render(true);
	static_cast<CBoss_Valtan*>(m_pGameObject)->Set_HpUIRender(false);
}


CValtan_BT_Phase3* CValtan_BT_Phase3::Create(void* pArg)
{
	CValtan_BT_Phase3* pInstance = new CValtan_BT_Phase3;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Phase3");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Phase3::Free()
{
	__super::Free();
}
