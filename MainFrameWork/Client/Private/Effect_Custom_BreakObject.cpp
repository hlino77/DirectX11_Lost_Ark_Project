#include "stdafx.h"
#include "Effect_Custom_BreakObject.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#include "Client_Defines.h"
#include "Boss_Valtan.h"
#include "StaticModel.h"
#include "AnimModel.h"
#include "GameObject.h"

CEffect_Custom_BreakObject::CEffect_Custom_BreakObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Custom_BreakObject::CEffect_Custom_BreakObject(const CEffect_Custom_BreakObject& rhs)
	: Super(rhs)
{
}

HRESULT CEffect_Custom_BreakObject::Initialize_Prototype(EFFECTDESC* pDesc)
{
	return S_OK;
}

HRESULT CEffect_Custom_BreakObject::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	BreakObjectDesc* pDesc = static_cast<BreakObjectDesc*>(pArg);
	
	m_WorldMatrix = pDesc->WorldMatrix;
	

	m_strModelName = pDesc->strModelName;

	m_pTransformCom->Set_WorldMatrix(m_WorldMatrix);
	m_vStartPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_bBreakStart = false;

	m_fLifeTime = 0.f;
	m_fTimeAcc = 0.0f;

	if (pDesc->pOwner)
	{
		m_pOwner = pDesc->pOwner;
	}
	 
	m_pTarget = CGameInstance::GetInstance()->Find_GameObject(LEVEL_VALTANMAIN, _uint(LAYER_TYPE::LAYER_BOSS), TEXT("Boss_Valtan"));

	
	m_AttackStyle = 0;


#pragma region ITR_02315_Cell 

	if (m_strModelName == TEXT("Itr_02315_Cell_000") || m_strModelName == TEXT("Itr_02315_Cell_022") || m_strModelName == TEXT("Itr_02315_Cell_063") ||
		m_strModelName == TEXT("Itr_02315_Cell_102") || m_strModelName == TEXT("Itr_02315_Cell_108") || m_strModelName == TEXT("Itr_02315_Cell_160") ||
		m_strModelName == TEXT("Itr_02315_Cell_162") || m_strModelName == TEXT("Itr_02315_Cell_201") || m_strModelName == TEXT("Itr_02315_Cell_240") ||
		m_strModelName == TEXT("Itr_02315_Cell_327") || m_strModelName == TEXT("Itr_02315_Cell_451")
		)

	{
		m_AttackStyle = 1;
	}

#pragma endregion

#pragma region ITR_02316_Cell

	if (m_strModelName == TEXT("Itr_02316_Cell_000") || m_strModelName == TEXT("Itr_02316_Cell_247") || m_strModelName == TEXT("Itr_02316_Cell_444") ||
		m_strModelName == TEXT("Itr_02316_Cell_508") || m_strModelName == TEXT("Itr_02316_Cell_565") || m_strModelName == TEXT("Itr_02316_Cell_619") ||
		m_strModelName == TEXT("Itr_02316_Cell_821") || m_strModelName == TEXT("Itr_02316_Cell_871") || m_strModelName == TEXT("Itr_02316_Cell_876") ||
		m_strModelName == TEXT("Itr_02316_Cell_915")
		)
	{
		m_AttackStyle = 1;
	}

#pragma endregion


	return S_OK;
}

void CEffect_Custom_BreakObject::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc < 0.3f)
	{
		m_bRimLight = true;
	}

	if (m_fTimeAcc < 0.2f)  // Latency Break
	{
		return;
	}

	m_bRimLight = false;

	CTransform* pTargetTransform = m_pTarget->Get_TransformCom();
	Vec3 TargetPos = pTargetTransform->Get_State(CTransform::STATE_POSITION);


	// Spread Random -> Dir
	if (m_AttackStyle == 0)
	{
		Spread_Random_Dir(fTimeDelta);
	}

	// Target Pos -> Dir
	if (m_AttackStyle == 1)
	{
		Target_Random_Dir(TargetPos, fTimeDelta);
	}

	if (m_fTimeAcc >= 1.f)
	{
		Set_Active(false);
		Set_Dead(true);
	}


}

void CEffect_Custom_BreakObject::LateTick(_float fTimeDelta)
{
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	}
}

HRESULT CEffect_Custom_BreakObject::Render()
{

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	if (true == m_bRimLight)
	{
		_float fRimLightColor = (_float)m_bRimLight;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
			return E_FAIL;
	}
	else
	{
		_float fRimLightColor = 0.f;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
			return E_FAIL;
	}

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;


	_float fRimLightColor = 0.f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRimLight", &fRimLightColor, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

HRESULT CEffect_Custom_BreakObject::Ready_Components()
{
	return S_OK;
}

HRESULT CEffect_Custom_BreakObject::Ready_Components(void* pArg)
{
	BreakObjectDesc* pDesc = static_cast<BreakObjectDesc*>(pArg);

	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	wstring strComName = pDesc->strModelName;
	if (FAILED(__super::Add_Component(LEVEL_VALTANMAIN, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}



CEffect_Custom_BreakObject* CEffect_Custom_BreakObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Custom_BreakObject* pInstance = new CEffect_Custom_BreakObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(nullptr)))
	{
		MSG_BOX("Failed To Created : CEffect_Custom_BreakObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CEffect_Custom_BreakObject::Spread_Random_Dir(_float fTimeDelta)
{
	_float iInitalSpeed = 10.0f;
	_float fGravity = -9.8f * 0.1; // Power Custom
	_float airResistance = 0.1f;


	if (false == m_bSetDir)
	{
		_float fSpeed = iInitalSpeed * exp(-airResistance * (m_fTimeAcc - 0.1f));

		// Random Vector Dir
		_float fRandomAngle = static_cast<float>((rand() % 360) * (3.141592 / 180.0));
		Vec3 fRandomDirection = Vec3(cos(fRandomAngle), 0.0f, sin(fRandomAngle));

		// Move Dir Create
		m_RandomMoveDirection = Vec3(fRandomDirection.x * fSpeed, m_vStartPos.y, fRandomDirection.z * fSpeed);

		m_bSetDir = true;
	}

	// fGravity
	float timeSinceStart = m_fTimeAcc - 0.1f;
	float verticalDisplacement = 0.5f * fGravity * pow(timeSinceStart, 2); // s = ut + 0.5at^2 

	//Position Move
	m_vStartPos += m_RandomMoveDirection * fTimeDelta; // x, z
	m_vStartPos.y += verticalDisplacement;	   // fGravity -> y	

	// Bind World Position
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vStartPos);

}


void CEffect_Custom_BreakObject::Target_Random_Dir(Vec3 TargetPosition, float fTimeDelta)
{
	_float iInitalSpeed = 10.0f;  // Power Cutom Dir 
	_float fGravity = -9.8f * 0.1; // Power Custom Gravity
	_float airResistance = 0.1f;

	Vec3 DirectionToTarget = TargetPosition - m_vStartPos; // Dir Vector
	DirectionToTarget.Normalize();

	Vec3 vUp = Vec3(0.0f, 1.0f, 0.0f); // Standard -> Y Axis

	Vec3 PerpendicularVector = DirectionToTarget.Cross(vUp);
	PerpendicularVector.Normalize();

	Vec3 AnotherPerpendicularVector = DirectionToTarget.Cross(PerpendicularVector);
	AnotherPerpendicularVector.Normalize();

	if (false == m_bSetDir)
	{
		// Random Angle Rotation
		_float fRandomAngle = static_cast<float>((rand() % 360) * (3.141592 / 180.0));
		Vec3 RandomDirection = DirectionToTarget * cos(fRandomAngle) + PerpendicularVector * sin(fRandomAngle) + AnotherPerpendicularVector * sin(fRandomAngle);

		m_RandomMoveDirection = RandomDirection * iInitalSpeed;

		m_bSetDir = true;
	}

	// fGravity
	float timeSinceStart = m_fTimeAcc - 0.1f;
	float verticalDisplacement = 0.5f * fGravity * pow(timeSinceStart, 2); // s = ut + 0.5at^2 

	//Position Move
	m_vStartPos += m_RandomMoveDirection * fTimeDelta; // x, z
	m_vStartPos.y += verticalDisplacement;	   // fGravity -> y	

	// Bind World Position
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vStartPos);

}



CGameObject* CEffect_Custom_BreakObject::Clone(void* pArg)
{
	CEffect_Custom_BreakObject* pInstance = new CEffect_Custom_BreakObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Custom_BreakObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Custom_BreakObject::Free()
{
	Super::Free();
}


