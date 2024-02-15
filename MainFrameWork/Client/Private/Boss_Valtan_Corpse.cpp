#include "stdafx.h"
#include "Boss_Valtan_Corpse.h"
#include "BehaviorTree.h"
#include "GameInstance.h"
#include "AsUtils.h"
#include "ColliderSphere.h"
#include "CollisionManager.h"
#include "BehaviorTree.h"
#include <Skill.h>
#include "ColliderOBB.h"
#include "RigidBody.h"
#include "NavigationMgr.h"
#include "PartObject.h"

CBoss_Valtan_Corpse::CBoss_Valtan_Corpse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBoss(pDevice, pContext)
{
}

CBoss_Valtan_Corpse::CBoss_Valtan_Corpse(const CBoss_Valtan_Corpse& rhs)
	: CBoss(rhs)
{
}

HRESULT CBoss_Valtan_Corpse::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Valtan_Corpse::Initialize(void* pArg)
{
	m_iMaxGroggyGauge = 1000;
	m_iGroggyGauge = m_iMaxGroggyGauge;
	m_iMaxHp = 1991561183;
	m_iHp = m_iMaxHp;
	m_bDummy = true;
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_iObjectID = Desc->iObjectID;
	m_iLayer = Desc->iLayer;
	m_iCurrLevel = Desc->iLevel;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pRigidBody->SetMass(2.0f);
	m_pRigidBody->Set_Gravity(false);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Desc->vPos);
	m_vSpawnPosition = Desc->vPos;

	if (FAILED(Ready_Coliders()))
		return E_FAIL;

	m_tCullingSphere.Radius = 2.0f;

	if (FAILED(Ready_BehaviourTree()))
		return E_FAIL;

	if (FAILED(Ready_DissolveTexture()))
		return E_FAIL;

	m_vecAttackRanges.clear();
	m_fMoveSpeed = 4.f;
	m_vecAttackRanges.push_back(2.f);
	m_vecAttackRanges.push_back(3.5f);
	m_IsSuperArmor = true;
	m_fAttackRange = m_vecAttackRanges[0];
	m_fRootTargetDistance = 0.f;
	m_fNoticeRange = 150.f;
	m_iPhase = 1;
	m_fFontScale = 0.55f;
	m_pModelCom->Reserve_NextAnimation(m_pModelCom->Initailize_FindAnimation(TEXT("att_battle_19_06"), 0.1f), 0.f, 0, 0);
	Set_DissolveOut(8.f);
	return S_OK;
}

void CBoss_Valtan_Corpse::Tick(_float fTimeDelta)
{
	m_PlayAnimation = std::async(&CModel::Play_Animation, m_pModelCom, fTimeDelta * m_fAnimationSpeed);
	CNavigationMgr::GetInstance()->SetUp_OnCell(m_iCurrLevel, this);
	m_pRigidBody->Tick(fTimeDelta);

	Update_StatusEffect(fTimeDelta);
	if (m_pWeapon != nullptr)
		m_pWeapon->Tick(fTimeDelta);
}

void CBoss_Valtan_Corpse::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	if (m_pWeapon != nullptr)
		m_pWeapon->LateTick(fTimeDelta);
	Update_Dissolve(fTimeDelta);
}

HRESULT CBoss_Valtan_Corpse::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return S_OK;

	_int	iDissolve =false ;
	if (m_bDissolveIn || m_bDissolveOut)
	{
		_int   bDissolve = true;
		_int  bReverseDissovle = false;
		_float fDissolveDensity = 3.f;
		_float fDissolveValue = 0.1f;
		_float fDissolveColorValue = 0.02f;
		_int  bDissolveEmissive = true;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &bDissolve, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bReverseDissolve", &bReverseDissovle, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveDensity", &fDissolveDensity, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveValue", &fDissolveValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveColorValue", &fDissolveColorValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolveEmissive", &bDissolveEmissive, sizeof(_int))))
			return E_FAIL;

		_float g_fDissolveAmount = m_fDissolvetime / m_fMaxDissolvetime;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveAmount", &g_fDissolveAmount, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_Texture("g_DissolveTexture", m_pDissolveTexture->Get_SRV())))
			return E_FAIL;

	}

	Color vValtanBloom = Color(0.4f, 1.6f, 1.3f, 1.f);
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomColor", &vValtanBloom, sizeof(Color))))
		return E_FAIL;

	m_pModelCom->SetUpAnimation_OnShader(m_pShaderCom);

	_uint		iNumMeshes = m_pModelPartCom[(_uint)PARTS::BODY]->Get_NumMeshes();

	for (_uint j = 0; j < iNumMeshes; ++j)
	{
		if (FAILED(m_pModelPartCom[(_uint)PARTS::BODY]->Render_SingleMesh(m_pShaderCom, j)))
			return E_FAIL;

	}
	if (m_bDissolveIn || m_bDissolveOut)
	{
		_int bDissolve = false;
		_int   bReverseDissovle = false;
		_float fDissolveDensity = 1.f;
		_float fDissolveValue = 0.1f;
		_float fDissolveColorValue = 0.02f;
		_int   bDissolveEmissive = false;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolve", &bDissolve, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bReverseDissolve", &bReverseDissovle, sizeof(_int))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveDensity", &fDissolveDensity, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveValue", &fDissolveValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveColorValue", &fDissolveColorValue, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_bDissolveEmissive", &bDissolveEmissive, sizeof(_int))))
			return E_FAIL;

		Vec4 vDissolveColor = Vec4::One;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomColor", &vDissolveColor, sizeof(Vec4))))
			return E_FAIL;
	}
	return S_OK;
}


HRESULT CBoss_Valtan_Corpse::Ready_Coliders()
{


	return S_OK;
}

void CBoss_Valtan_Corpse::Reserve_WeaponAnimation(wstring strAnimName, _float fChangeTime, _int iStartFrame, _int iChangeFrame, _float fAnimspeed)
{
	if (m_pWeapon->Get_ModelCom()->Initailize_FindAnimation(strAnimName, fAnimspeed) > m_pWeapon->Get_ModelCom()->Get_MaxAnimIndex())
		strAnimName = L"att_battle_8_01_loop";
	m_pWeapon->Get_ModelCom()->Reserve_NextAnimation(m_pWeapon->Get_ModelCom()->Find_AnimIndex(strAnimName), fChangeTime,
		iStartFrame, iChangeFrame);
	m_pWeapon->Get_ModelCom()->Set_Anim_Speed(m_pWeapon->Get_ModelCom()->Find_AnimIndex(strAnimName), fAnimspeed);
}

void CBoss_Valtan_Corpse::Set_Weapon_Render(_bool IsRender)
{
	m_pWeapon->Set_Render(IsRender);
}

HRESULT CBoss_Valtan_Corpse::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBody)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_Boss_Valtan";
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	Safe_Release(pGameInstance);
	strComName = L"Prototype_Component_Model_Boss_Valtan";
	if (FAILED(__super::Add_Component(CGameInstance::GetInstance()->Get_CurrLevelIndex(), strComName, TEXT("Com_Model_Body"), (CComponent**)&m_pModelPartCom[(_uint)PARTS::BODY])))
		return E_FAIL;

	CPartObject::PART_DESC			PartDesc_Weapon;
	PartDesc_Weapon.pOwner = this;
	PartDesc_Weapon.ePart = CPartObject::PARTS::WEAPON_1;
	PartDesc_Weapon.pParentTransform = m_pTransformCom;
	PartDesc_Weapon.pPartenModel = m_pModelCom;
	PartDesc_Weapon.iSocketBoneIndex = m_pModelCom->Find_BoneIndex(TEXT("b_wp_r_01"));
	PartDesc_Weapon.SocketPivotMatrix = m_pModelCom->Get_PivotMatrix();
	m_pWeapon = dynamic_cast<CPartObject*>(CGameInstance::GetInstance()->Clone_GameObject(TEXT("Prototype_GameObject_Weapon_Boss_Valtan"), &PartDesc_Weapon));
	if (nullptr == m_pWeapon)
		return E_FAIL;

	m_vOriginScale.x = 1.f;
	m_vOriginScale.y = 1.f;
	m_vOriginScale.z = 1.f;

	m_pTransformCom->Set_Scale(m_vOriginScale);
	return S_OK;
}

HRESULT CBoss_Valtan_Corpse::Ready_BehaviourTree()
{
	return S_OK;
}

CBoss* CBoss_Valtan_Corpse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Valtan_Corpse* pInstance = new CBoss_Valtan_Corpse(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_Valtan_Corpse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Valtan_Corpse::Clone(void* pArg)
{
	CBoss_Valtan_Corpse* pInstance = new CBoss_Valtan_Corpse(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		cout << "Failed To Cloned : CBoss_Valtan_Corpse" << endl;
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CBoss_Valtan_Corpse::Free()
{
	__super::Free();
	for (auto& Collider : m_Coliders)
		CCollisionManager::GetInstance()->Out_Colider(Collider.second);
}
