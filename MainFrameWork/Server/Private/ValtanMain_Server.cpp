#include "stdafx.h"
#include "GameInstance.h"
#include "ValtanMain_Server.h"
#include "Player_Server.h"
#include "Boss_Server.h"
#include "Monster_Server.h"
#include "GameSessionManager.h"
#include "AsUtils.h"
#include "NavigationMgr.h"
#include "GameSession.h"
#include <filesystem>
#include "BreakAbleObject_Server.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"

CValtanMain_Server::CValtanMain_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"VALTANMAIN", OBJ_TYPE::DUNGEAN)
{
}

CValtanMain_Server::CValtanMain_Server(const CValtanMain_Server& rhs)
	: CGameObject(rhs)
{
}

HRESULT CValtanMain_Server::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CValtanMain_Server::Initialize(void* pArg)
{
	DUNGEANDESC* tDesc = static_cast<DUNGEANDESC*>(pArg);
	m_Players = tDesc->Players;
	Ready_Dungean();

	
	Send_OpenLevel();


	//if (FAILED(Load_BossMapData(LEVEL_VALTANMAIN, TEXT("../Bin/Resources/MapData/Test0121.data"))))
	//{
	//	return E_FAIL;
	//}



	m_bEnd = false;
	m_fEndDelay = 5.0f;


    return S_OK;
}

void CValtanMain_Server::Tick(_float fTimeDelta)
{
	if (m_fStartDelay > 0.0f)
	{
		m_fStartDelay -= fTimeDelta;
		if (m_fStartDelay < 0.0f)
		{
			m_fStartDelay = 0.0f;
			Broadcast_Boss(Vec3(100.0f, 0.19f, 100.0f), L"Valtan");
		}
			
		return;
	}

	if (m_iBossCount == 0 && m_Bosses.size() == 0)
	{
		/*m_bEnd = true;

		if (m_bEnd)
		{
			m_fEndDelay -= fTimeDelta;
			if (m_fEndDelay <= 0.0f)
			{

				Set_Active(false);
				Set_Dead(true);
			}
		}*/
	}
}

void CValtanMain_Server::LateTick(_float fTimeDelta)
{
	for (auto iterBoss = m_Bosses.begin(); iterBoss != m_Bosses.end();)
	{
		if ((*iterBoss)->Is_Dead())
			iterBoss = m_Bosses.erase(iterBoss);
		else
			++iterBoss;
	}

	m_fBroadcastTime += fTimeDelta;
	if (m_fBroadcastTime >= 0.05f)
	{
		Broadcast_PlayerInfo();
		m_fBroadcastTime = 0.0f;
	}
}

HRESULT CValtanMain_Server::Render()
{
    return S_OK;
}

HRESULT CValtanMain_Server::Ready_Components()
{
    return S_OK;
}

HRESULT CValtanMain_Server::Ready_Dungean()
{
	m_iCurrLevel = LEVELID::LEVEL_VALTANMAIN;
	m_fStartDelay = 15.0f;
	m_iBossCount = 1;
	return S_OK;
}


void CValtanMain_Server::Broadcast_Boss(Vec3 vPos, wstring ModelName)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	{

		wstring szComponentName = L"Boss_" + ModelName;
		CBoss_Server::MODELDESC Desc;
		Desc.strFileName = ModelName;
		Desc.iObjectID = g_iObjectID++;
		Desc.iLayer = (_uint)LAYER_TYPE::LAYER_BOSS;
		Desc.iLevel = m_iCurrLevel;
		Desc.vPosition = vPos;
		wstring szMonsterName = L"Prototype_GameObject_" + szComponentName;
		CBoss_Server* pBoss = dynamic_cast<CBoss_Server*>(pGameInstance->Add_GameObject(m_iCurrLevel, Desc.iLayer, szMonsterName, &Desc));
		if (pBoss == nullptr)
			return;

		CNavigationMgr::GetInstance()->Find_FirstCell(pBoss->Get_CurrLevel(), pBoss);
		m_Bosses.push_back(pBoss);
		--m_iBossCount;

		Protocol::S_CREATE_OBJCECT tMonsterPkt;

		tMonsterPkt.set_iobjectid(pBoss->Get_ObjectID());
		tMonsterPkt.set_iobjecttype(pBoss->Get_ObjectType());
		tMonsterPkt.set_strname(CAsUtils::ToString(pBoss->Get_ModelName()));
		tMonsterPkt.set_ilayer(pBoss->Get_ObjectLayer());
		tMonsterPkt.set_ilevel(pBoss->Get_CurrLevel());

		tMonsterPkt.set_bcontroll(true);

		auto vPos = tMonsterPkt.mutable_vpos();
		vPos->Resize(3, 0.0f);
		Vec3 vPosition = pBoss->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		memcpy(vPos->mutable_data(), &vPosition, sizeof(Vec3));

		SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(tMonsterPkt);
		CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);
		pBoss->Send_Monster_Action();
	}


	Safe_Release(pGameInstance);
}

void CValtanMain_Server::Send_OpenLevel()
{
	Protocol::S_OPEN_LEVEL pkt;
	pkt.set_ilevelid(m_iCurrLevel);

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	
	for (auto& Player : m_Players)
		Player->Get_GameSession()->Send(pSendBuffer);
}

void CValtanMain_Server::Exit_Dungean()
{
	
}

HRESULT CValtanMain_Server::Broadcast_PlayerInfo()
{
	Protocol::S_OBJECTINFO pkt;

	for (auto& Object : m_Players)
	{
		if (Object == nullptr)
			continue;

		auto tObject = pkt.add_tobject();
		tObject->set_iobjectid(Object->Get_ObjectID());
		tObject->set_ilevel(m_iCurrLevel);
		tObject->set_ilayer((_uint)LAYER_TYPE::LAYER_PLAYER);


		auto vTargetPos = tObject->mutable_vtargetpos();
		vTargetPos->Resize(3, 0.0f);
		Vec3 vPlayerTargetPos = Object->Get_TargetPos();
		memcpy(vTargetPos->mutable_data(), &vPlayerTargetPos, sizeof(Vec3));


		auto matWorld = tObject->mutable_matworld();
		matWorld->Resize(16, 0.0f);
		Matrix matPlayerWorld = Object->Get_TransformCom()->Get_WorldMatrix();
		memcpy(matWorld->mutable_data(), &matPlayerWorld, sizeof(Matrix));
	}


	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);

	for (auto& Player : m_Players)
		dynamic_cast<CPlayer_Server*>(Player)->Get_GameSession()->Send(pSendBuffer);

	return S_OK;
}

HRESULT	CValtanMain_Server::Load_BossMapData(LEVELID eLevel, const wstring& szFullPath)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFullPath, FileMode::Read);

	Vec3	QuadTreePosition = {};
	Vec3	QuadTreeScale = {};
	_uint	QuadTreeMaxDepth = {};

	file->Read<Vec3>(QuadTreePosition);
	file->Read<Vec3>(QuadTreeScale);
	file->Read<_uint>(QuadTreeMaxDepth);


	_uint iSize = file->Read<_uint>(); // Object List Size 


	bool fileFound = false;

	for (_uint i = 0; i < iSize; ++i)
	{
		string strFileName = file->Read<string>(); // Object Model FileName
		wstring selectedPath = {};

		// Object World Matrix
		Matrix	matWorld = file->Read<Matrix>();

		// Object ModelType
		_uint ModelType = file->Read<_uint>();

		// Instancing Check 
		_bool bInstance = file->Read<_bool>();


		// Clone GameObject
		CGameObject* pObject = nullptr;

		if (0 == ModelType) // NonAnim
		{
			CBreakAbleObject_Server::MODELDESC Desc;
			Desc.strFileName = CAsUtils::ToWString(strFileName);
			Desc.iLayer = (_uint)LAYER_TYPE::LAYER_BACKGROUND;
			


			// Under Navi Cell Info
			_uint vecNavicellsize = file->Read<_uint>(); // Read Vector Size


			// Not Creat
			if (vecNavicellsize <= 0)
			{
				for (size_t i = 0; i < vecNavicellsize; i++)
				{
					_uint CellIndex = file->Read<_uint>();


				}
				_uint iColliderCount = file->Read<_uint>();

				for (_uint i = 0; i < iColliderCount; ++i)
				{

					Vec3 vOffset = file->Read<Vec3>();
					_float fRadius = file->Read<_float>();

					_bool bChild = file->Read<_bool>();

					if (bChild)
					{

						Vec3 vOffset = file->Read<Vec3>();
						Vec3 vScale = file->Read<Vec3>();

						Quaternion vQuat = file->Read<Quaternion>();
					}
				}
			}
			else // Creat BreakAbleObjcet
			{
				Desc.iObjectID = g_iObjectID++;

				pObject = pGameInstance->Add_GameObject(eLevel, Desc.iLayer, TEXT("Prototype_GameObject_BreakAbleObject"), &Desc);
				pObject->Get_TransformCom()->Set_WorldMatrix(matWorld);

				for (size_t i = 0; i < vecNavicellsize; i++)
				{
					_uint CellIndex = file->Read<_uint>();

					dynamic_cast<CBreakAbleObject_Server*>(pObject)->Add_NaviCellIndex(CellIndex);
					CNavigationMgr::GetInstance()->Set_NaviCell_Active(LEVEL_VALTANMAIN, CellIndex, false);
				}

				_uint iColliderCount = file->Read<_uint>();

				for (_uint i = 0; i < iColliderCount; ++i)
				{

					dynamic_cast<CBreakAbleObject_Server*>(pObject)->Add_Collider();

					CSphereCollider* pCollider = dynamic_cast<CBreakAbleObject_Server*>(pObject)->Get_StaticCollider(i);

					{
						Vec3 vOffset = file->Read<Vec3>();
						pCollider->Set_Offset(vOffset);


						_float fRadius = file->Read<_float>();
						pCollider->Set_Radius(fRadius);


						pCollider->Set_Center();
					}

					_bool bChild = file->Read<_bool>();

					if (bChild)
					{
						dynamic_cast<CBreakAbleObject_Server*>(pObject)->Add_ChildCollider(i);

						COBBCollider* pChild = dynamic_cast<COBBCollider*>(pCollider->Get_Child());


						Vec3 vOffset = file->Read<Vec3>();
						pChild->Set_Offset(vOffset);

						Vec3 vScale = file->Read<Vec3>();
						pChild->Set_Scale(vScale);

						Quaternion vQuat = file->Read<Quaternion>();
						pChild->Set_Orientation(vQuat);

						pChild->Set_StaticBoundingBox();
					}
				}
				dynamic_cast<CBreakAbleObject_Server*>(pObject)->Add_CollidersToManager();


			}

		}
		else if (1 == ModelType) // Anim
		{
		  // Not Create

		}

		_uint			QuadTreeSize = {};


		file->Read<_uint>(QuadTreeSize);

		for (size_t i = 0; i < QuadTreeSize; i++)
		{
			_uint Index = {};
			file->Read<_uint>(Index);

			//pObject->Add_QuadTreeIndex(Index);
			//CQuadTreeMgr::GetInstance()->Add_Object(pObject, Index);
		}

	}

	Safe_Release(pGameInstance);
	return S_OK;

}

CValtanMain_Server* CValtanMain_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CValtanMain_Server* pInstance = new CValtanMain_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CValtanMain_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CValtanMain_Server::Clone(void* pArg)
{
	CValtanMain_Server* pInstance = new CValtanMain_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CValtanMain_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtanMain_Server::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	for (auto& Boss : m_Bosses)
		Boss->Set_Dead(true);
}
