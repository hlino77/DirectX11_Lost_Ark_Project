#include "stdafx.h"
#include "GameInstance.h"
#include "Npc.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "RigidBody.h"
#include "ColliderOBB.h"
#include "CollisionManager.h"
#include "PickingMgr.h"
#include "Struct.pb.h"
#include "NavigationMgr.h"
#include "Skill.h"
#include "Pool.h"

#include "Chat_Manager.h"
#include "BindShaderDesc.h"
#include "UI_Manager.h"
#include "UI_SpeechBubble.h"

CNpc::CNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Npc", OBJ_TYPE::NPC)
{
}

CNpc::CNpc(const CNpc& rhs)
	: CGameObject(rhs)
{
}

HRESULT CNpc::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CNpc::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	{
		NPCDESC* pDesc = static_cast<NPCDESC*>(pArg);

		m_iCurrLevel = pDesc->iCurLevel;

		m_eNpcType = (NPCTYPE)pDesc->iNpcType;
		m_strObjectTag = pDesc->strNpcTag;
		m_strNpcName = pDesc->strNpcName;
		m_matStart = pDesc->matStart;
		memcpy(&m_vStartPos, m_matStart.m[3], sizeof(Vec3));

		m_eNpcShape = (NPCSHAPE)pDesc->iNpcShape;
		m_strNpcMq = pDesc->strNpcMq;
		m_strNpcHead = pDesc->strNpcHead;
		m_strNpcBody = pDesc->strNpcBody;

		m_strIdleAnim = pDesc->strIdleAnim;
		m_strActAnim = pDesc->strActAnim;
		m_fChangeAnimTime = pDesc->fChangeAnimTime;

		m_IsMove = pDesc->IsMove;
		m_fMoveLength = pDesc->fMoveLength;

		m_IsTalk = pDesc->IsTalk;
		m_fTalkStartTime = pDesc->fTalkStartTime;
		m_vecTalks = pDesc->vecTalks;

		m_bUseWeaponPart = pDesc->bUseWeaponPart;
		m_strLeftPart = pDesc->strLeftPart;
		m_vLeftOffset[(_uint)WEAPON_OFFSET::SCALE] = pDesc->vLeftOffset[(_uint)WEAPON_OFFSET::SCALE];
		m_vLeftOffset[(_uint)WEAPON_OFFSET::ROTATION] = pDesc->vLeftOffset[(_uint)WEAPON_OFFSET::ROTATION];
		m_vLeftOffset[(_uint)WEAPON_OFFSET::POSITION] = pDesc->vLeftOffset[(_uint)WEAPON_OFFSET::POSITION];
		m_strRightPart = pDesc->strRightPart;
		m_vRightOffset[(_uint)WEAPON_OFFSET::SCALE] = pDesc->vRightOffset[(_uint)WEAPON_OFFSET::SCALE];
		m_vRightOffset[(_uint)WEAPON_OFFSET::ROTATION] = pDesc->vRightOffset[(_uint)WEAPON_OFFSET::ROTATION];
		m_vRightOffset[(_uint)WEAPON_OFFSET::POSITION] = pDesc->vRightOffset[(_uint)WEAPON_OFFSET::POSITION];

		m_iLeftBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_2"));
		m_iRightBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_1"));
	}
	
	if (true == m_IsMove)
	{
		Vec3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();

		m_vMovePos = m_vStartPos + (vLook * m_fMoveLength);
	}

	//
	m_iLayer = (_uint)LAYER_TYPE::LAYER_NPC;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	if (FAILED(Ready_SpeechBuble()))
		return E_FAIL;

	m_pRigidBody->SetMass(2.0f);
	m_pTransformCom->Set_WorldMatrix(m_matStart);
	m_tCullingSphere.Radius = 2.0f;

	m_iIdleAnimIndex = m_pModelCom->Initailize_FindAnimation(m_strIdleAnim, 1.0f);
	if (m_iIdleAnimIndex == -1)
		return E_FAIL;

	m_iActAnimIndex = m_pModelCom->Initailize_FindAnimation(m_strActAnim, 1.0f);
	if (m_iActAnimIndex == -1)
		return E_FAIL;

	m_pModelCom->Set_CurrAnim(m_iIdleAnimIndex);

	return S_OK;
}

void CNpc::Tick(_float fTimeDelta)
{
	if (m_bNavi)
	{
		CNavigationMgr::GetInstance()->SetUp_OnCell(m_iCurrLevel, this);
	}

	Check_ChangeAnim(fTimeDelta);

	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
}

void CNpc::LateTick(_float fTimeDelta)
{
	if (m_PlayAnimation.valid())
		m_PlayAnimation.get();

	for (auto& pPart : m_pWeaponPart)
	{
		if (nullptr == pPart) continue;

		pPart->Tick(fTimeDelta);
	}

	//m_pRigidBody->Tick(fTimeDelta);

	if (nullptr == m_pRendererCom)
		return;
	{
		READ_LOCK
			for (auto& CollisionStay : m_CollisionList)
				OnCollisionStay(CollisionStay.iColLayer, CollisionStay.pCollider);
	}

	CullingObject();

	for (auto& pPart : m_pWeaponPart)
	{
		if (nullptr == pPart) continue;

		pPart->LateTick(fTimeDelta);
	}

	if (m_bRimLight)
	{
		m_fRimLightTime -= fTimeDelta;
		if (m_fRimLightTime <= 0.0f)
		{
			m_fRimLightTime = 0.0f;
			m_bRimLight = false;
		}
	}
}

HRESULT CNpc::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return S_OK;

	_float fRimLight = (_float)m_bRimLight;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLight, sizeof(_float))))
		return E_FAIL;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	return S_OK;
}

HRESULT CNpc::Render_ShadowDepth()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_ShadowWVP()))
		return S_OK;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	return S_OK;
}

HRESULT CNpc::Render_Debug()
{
	for (auto& Colider : m_Coliders)
	{
		if (Colider.second->IsActive())
		{
			Colider.second->DebugRender();
		}

	}

	return S_OK;
}

void CNpc::Go_Straight(_float fSpeed, _float fTimeDelta)
{
	m_pTransformCom->Go_Straight(fSpeed, fTimeDelta);
}

void CNpc::Move_Dir(Vec3 vDir, _float fSpeed, _float fTimeDelta)
{
	m_pTransformCom->LookAt_Lerp(vDir, 5.0f, fTimeDelta);
	m_pTransformCom->Go_Straight(fSpeed, fTimeDelta);
}

void CNpc::Body_Collision(CGameObject* pObject)
{
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Vec3 vOtherPos = pObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

	Vec3 vDir = vPos - vOtherPos;
	_float fDistance = vDir.Length();

	if (fDistance < 0.5f)
	{
		vDir.Normalize();
		Vec3 vTargetPos = vOtherPos + vDir * 0.5f;
		vPos = Vec3::Lerp(vPos, vTargetPos, 0.2f);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
}

_bool CNpc::Stop_VoiceSound()
{
	if (m_fVoiceSoundDelay == 0.0f)
	{
		CGameInstance::GetInstance()->Find_Stop_Sound(m_VoiceSoundKey);
		return true;
	}

	return false;
}

void CNpc::Set_Several_Weapon_RenderState(WEAPON_PART ePart, _bool Is_Render)
{
	if (nullptr == m_pWeaponPart[(_uint)ePart])
		return;

	m_pWeaponPart[(_uint)ePart]->Set_Render(Is_Render);
}

HRESULT CNpc::Ready_Components()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 3.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_State */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"),
		TEXT("Com_StateMachine"), (CComponent**)&m_pStateMachine)))
		return E_FAIL;

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"),
		TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_" + m_strNpcMq;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (NPCTYPE::FUNCTION == m_eNpcType)
	{
		CCollider::ColliderInfo tColliderInfo;
		tColliderInfo.m_bActive = true;
		tColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY_NPC;
		CSphereCollider* pCollider = nullptr;

		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), TEXT("Com_ColliderBody"), (CComponent**)&pCollider, &tColliderInfo)))
			return E_FAIL;

		if (pCollider)
		{
			{
				CCollider::ColliderInfo tChildColliderInfo;
				tChildColliderInfo.m_bActive = true;
				tChildColliderInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;
				COBBCollider* pChildCollider = nullptr;

				if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"), TEXT("Com_ColliderBodyChild"), (CComponent**)&pChildCollider, &tChildColliderInfo)))
					return E_FAIL;

				pCollider->Set_Child(pChildCollider);
			}

			m_Coliders.emplace((_uint)LAYER_COLLIDER::LAYER_BODY_NPC, pCollider);
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	m_vOriginScale.x = 0.01f;
	m_vOriginScale.y = 0.01f;
	m_vOriginScale.z = 0.01f;

	m_pTransformCom->Set_Scale(m_vOriginScale);

	return S_OK;
}

HRESULT CNpc::Ready_Parts()
{
	if (false == m_bUseWeaponPart)
		return S_OK;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* pParts = nullptr;

	if (TEXT("") != m_strRightPart)
	{
		CPartObject::PART_DESC			PartDesc_Weapon;
		PartDesc_Weapon.pOwner = this;
		PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_1;
		PartDesc_Weapon.pParentTransform = m_pTransformCom;
		PartDesc_Weapon.pPartenModel = m_pModelCom;
		PartDesc_Weapon.iSocketBoneIndex = m_iRightBoneIndex;
		PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
		PartDesc_Weapon.strModel = m_strRightPart;
		PartDesc_Weapon.vPartScale = m_vRightOffset[(_uint)WEAPON_OFFSET::SCALE];
		PartDesc_Weapon.vPartRot = m_vRightOffset[(_uint)WEAPON_OFFSET::ROTATION];
		PartDesc_Weapon.vPartPos = m_vRightOffset[(_uint)WEAPON_OFFSET::POSITION];

		wstring strObject = TEXT("Prototype_GameObject_NpcPart");
		pParts = pGameInstance->Clone_GameObject(strObject, &PartDesc_Weapon);
		if (nullptr == pParts)
			return E_FAIL;
	}

	if (TEXT("") != m_strLeftPart)
	{
		CPartObject::PART_DESC			PartDesc_Weapon;
		PartDesc_Weapon.pOwner = this;
		PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_2;
		PartDesc_Weapon.pParentTransform = m_pTransformCom;
		PartDesc_Weapon.pPartenModel = m_pModelCom;
		PartDesc_Weapon.iSocketBoneIndex = m_iLeftBoneIndex;
		PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
		PartDesc_Weapon.strModel = m_strLeftPart;
		PartDesc_Weapon.vPartScale = m_vLeftOffset[(_uint)WEAPON_OFFSET::SCALE];
		PartDesc_Weapon.vPartRot = m_vLeftOffset[(_uint)WEAPON_OFFSET::ROTATION];
		PartDesc_Weapon.vPartPos = m_vLeftOffset[(_uint)WEAPON_OFFSET::POSITION];

		wstring strObject = TEXT("Prototype_GameObject_NpcPart");
		pParts = pGameInstance->Clone_GameObject(strObject, &PartDesc_Weapon);
		if (nullptr == pParts)
			return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CNpc::Ready_SpeechBuble()
{
	m_pSpeechBuble = CPool<CUI_SpeechBubble>::Get_Obj();

	if (m_pSpeechBuble == nullptr)
		return E_FAIL;

	m_pSpeechBuble->Set_Host(this);

	return S_OK;
}

void CNpc::CullingObject()
{
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_tCullingSphere.Center = vPos;

	const BoundingFrustum& tCamFrustum = CGameInstance::GetInstance()->Get_CamFrustum();

	if (tCamFrustum.Intersects(m_tCullingSphere) == false)
		return;

	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
		m_pRendererCom->Add_DebugObject(this);
	}

}

void CNpc::Check_ChangeAnim(const _float& fTimeDelta)
{
	if (-1 == m_iActAnimIndex)
		return;

	if (m_pModelCom->Get_CurrAnim() == m_iIdleAnimIndex)
	{
		m_fChangeAnimAcc += fTimeDelta;

		if (m_fChangeAnimTime <= m_fChangeAnimAcc)
		{
			Reserve_Animation(m_iActAnimIndex, 0.2f, 0, 0);
		}
	}
	else if (m_pModelCom->Get_CurrAnim() == m_iActAnimIndex)
	{
		if (true == m_pModelCom->Is_AnimationEnd(m_iActAnimIndex))
		{
			m_fChangeAnimAcc = 0.f;
			Reserve_Animation(m_iIdleAnimIndex, 0.2f, 0, 0);
		}
	}
}

void CNpc::Show_SpeechBuble(const wstring& szChat)
{
	m_pSpeechBuble->Active_SpeechBuble(szChat);
}

void CNpc::Set_State(const wstring& szName)
{
	m_pStateMachine->Change_State(szName);
}

void CNpc::Reserve_Animation(_uint iAnimIndex, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fRootDist, _bool bRootRot, _bool bReverse, _bool bUseY)
{
	if (nullptr == m_pModelCom)
		return;

	m_pModelCom->Reserve_NextAnimation(iAnimIndex, fChangeTime, iStartFrame, iChangeFrame, fRootDist, bRootRot, bReverse, bUseY);
}

void CNpc::Free()
{
	__super::Free();

	for (size_t i = 0; i < (_uint)PART::_END; i++)
	{
		Safe_Release(m_pNpcPartCom[i]);
	}

	for (size_t i = 0; i < (_uint)WEAPON_PART::_END; i++)
	{
		Safe_Release(m_pWeaponPart[i]);
	}

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	CPool<CUI_SpeechBubble>::Return_Obj(m_pSpeechBuble);
	m_pSpeechBuble = nullptr;
}
