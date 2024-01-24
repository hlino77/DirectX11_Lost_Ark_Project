#include "stdafx.h"
#include "Valtan_BT_Attack_Attack21_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include <Boss_Server.h>
#include <AsUtils.h>
#include "GameSessionManager.h"

CValtan_BT_Attack_Attack21_Server::CValtan_BT_Attack_Attack21_Server()
{
}

void CValtan_BT_Attack_Attack21_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
	m_bShoot = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack21_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Move_to_SpawnPosition();
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
	}
	if (m_iCurrAnimation == 5 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex && m_fLoopTime < 3.f)
	{
		static_cast<CBoss_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	}

	if (m_iCurrAnimation == 9 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[9].iAnimIndex && m_fLoopTime < 3.f)
	{
		static_cast<CBoss_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) >= 73 && m_bShoot)
	{
		m_bShoot = false;
		Vec3 vDir = Vec3(1.f, 0.f, 1.f);
		vDir.Normalize();
		Matrix matRotation90 = Matrix::CreateRotationY(XMConvertToRadians(90.f));

		for (size_t i = 0; i < 4; i++)
		{
			Vec3 vPosition = static_cast<CBoss_Server*>(m_pGameObject)->Get_SpawnPosition();
			vDir = Vec3::TransformNormal(vDir, matRotation90);
			vDir.y = 0.f;
			vDir.Normalize();
			vPosition += vDir * 10.f;
			vPosition.y = -60.f;
			Add_Stone(vPosition);
		}
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack21_Server::Add_Stone(Vec3 vPosition)
{
	wstring szComponentName = L"Monster_Crystal";
	CMonster_Server::MODELDESC Desc;
	Desc.strFileName = L"Crystal";
	Desc.iObjectID = g_iObjectID++;
	Desc.iLayer = (_uint)LAYER_TYPE::LAYER_MONSTER;
	Desc.iLevel = m_pGameObject->Get_CurrLevel();

	Desc.vPosition = vPosition;

	wstring szMonsterName = L"Prototype_GameObject_" + szComponentName;
	CMonster_Server* pMonster = dynamic_cast<CMonster_Server*>(CGameInstance::GetInstance()->Add_GameObject(m_pGameObject->Get_CurrLevel(), Desc.iLayer, szMonsterName, &Desc));
	if (pMonster == nullptr)
		return;
	Protocol::S_CREATE_OBJCECT tMonsterPkt;

	tMonsterPkt.set_iobjectid(pMonster->Get_ObjectID());
	tMonsterPkt.set_iobjecttype(pMonster->Get_ObjectType());
	tMonsterPkt.set_strname(CAsUtils::ToString(pMonster->Get_ModelName()));
	tMonsterPkt.set_ilayer(pMonster->Get_ObjectLayer());
	tMonsterPkt.set_ilevel(m_pGameObject->Get_CurrLevel());

	tMonsterPkt.set_bcontroll(true);

	auto vPos = tMonsterPkt.mutable_vpos();
	vPos->Resize(3, 0.0f);
	{
		Vec3 vPosition = pMonster->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		memcpy(vPos->mutable_data(), &vPosition, sizeof(Vec3));
	}
	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(tMonsterPkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);
}
void CValtan_BT_Attack_Attack21_Server::On_FirstAnimStart()
{
	static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(true);
}

void CValtan_BT_Attack_Attack21_Server::On_LastAnimEnd()
{
	static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(false);
}

void CValtan_BT_Attack_Attack21_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_Attack21_Server* CValtan_BT_Attack_Attack21_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack21_Server* pInstance = new CValtan_BT_Attack_Attack21_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack21_Server::Free()
{
	__super::Free();
}
