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

	Vec3 CenterPos = { 100.f, 0.f, 100.f };


	// Target Pos -> Dir
	if (m_AttackStyle == 0)
	{
		Target_Random_Dir(TargetPos, fTimeDelta);
	}

	// Spread Random -> Dir
	if (m_AttackStyle == 1)
	{
		Spread_Random_Dir(fTimeDelta);
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
	_float fMinSpeed = 10.f;
	_float fMaxSpeed = 15.f;


	_float fGravity = -9.8f * 0.05; // Power Custom
	_float airResistance = 0.1f;


	if (false == m_bSetDir)
	{
		 
		_float iInitalSpeed = fMinSpeed + static_cast<_float>(rand()) / RAND_MAX * (fMaxSpeed - fMinSpeed);

		_float fSpeed = iInitalSpeed * exp(-airResistance * (m_fTimeAcc - 0.1f));

		// Random Vector Dir
		_float fRandomAngle = static_cast<_float>((rand() % 360) * (3.141592 / 180.0));
		Vec3 fRandomDirection = Vec3(cos(fRandomAngle), 0.0f, sin(fRandomAngle));

		// Move Dir Create
		m_RandomMoveDirection = Vec3(fRandomDirection.x * fSpeed, m_vStartPos.y, fRandomDirection.z * fSpeed);

		m_iRight = static_cast<_int>(rand() % 2);
		m_iFront = static_cast<_int>(rand() % 2);

		// Z
		if (m_iRight == 0)
		{
			m_iRight = -1;
		}
		else
		{
			m_iRight = 1;
		}
		// X
		if (m_iFront == 0)
		{
			m_iFront = -1;
		}
		else
		{
			m_iFront = 1;
		}

	}

	// fGravity
	_float timeSinceStart = m_fTimeAcc - 0.1f;
	_float verticalDisplacement = 0.5f * fGravity * pow(timeSinceStart, 2); // s = ut + 0.5at^2 

	//Position Move
	m_vStartPos += m_RandomMoveDirection * fTimeDelta; // x, z
	m_vStartPos.y += verticalDisplacement;	   // fGravity -> y	

	// Bind World Position
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vStartPos);

	// Bind World Roatation
	Vec3  AxisX = Vec3(1.f, 0.f, 0.f);
	Vec3  AxisZ = Vec3(0.f, 0.f, 1.f);
	m_pTransformCom->Turn_Rotation_CurrentState(AxisX, fTimeDelta * m_iFront);
	m_pTransformCom->Turn_Rotation_CurrentState(AxisZ, fTimeDelta * m_iRight);

}


void CEffect_Custom_BreakObject::Target_Random_Dir(Vec3 TargetPosition, _float fTimeDelta)
{
	_float fMinSpeed = 10.f;
	_float fMaxSpeed = 15.f;

	_float fGravity = -9.8f * 0.05; // Power Custom Gravity
	_float airResistance = 0.1f; //

	if (false == m_bSetDir)
	{
		Vec3 DirectionToTarget = (TargetPosition - m_vStartPos); 
		DirectionToTarget.Normalize();

		// Random Angle    -> + 135  - 135
		_float fRandomAngleRadians = static_cast<_float>((rand() % 270 - 135)) * (3.141592 / 180.0f);

		Vec3 rotationAxis = Vec3(0.0f, 1.0f, 0.0f);
		Vec3 RotationAxisCross = rotationAxis.Cross(DirectionToTarget);
		RotationAxisCross.Normalize();

	
		Vec3 finalDirection = DirectionToTarget * cos(fRandomAngleRadians) + RotationAxisCross * sin(fRandomAngleRadians);
		finalDirection.Normalize();

		_float iInitialSpeed = fMinSpeed + static_cast<_float>(rand()) / RAND_MAX * (fMaxSpeed - fMinSpeed);


		m_RandomMoveDirection = finalDirection * iInitialSpeed;


		m_iRight = static_cast<_int>(rand() % 2);
		m_iFront = static_cast<_int>(rand() % 2);

		// Z
		if (m_iRight == 0)
		{
			m_iRight = -1;
		}
		else
		{
			m_iRight = 1;
		}
		// X
		if (m_iFront == 0)
		{
			m_iFront = -1;
		}
		else
		{
			m_iFront = 1;
		}

		m_bSetDir = true;
	}

	// Gravity
	_float timeSinceStart = m_fTimeAcc - 0.1f;
	_float verticalDisplacement = 0.5f * fGravity * pow(timeSinceStart, 2);

	// Position Move
	m_vStartPos += m_RandomMoveDirection * fTimeDelta; // x, z
	m_vStartPos.y += verticalDisplacement; // Gravity -> y

	// Bind World Position
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vStartPos);


	// Bind World Roatation
	Vec3  AxisX = Vec3(1.f, 0.f, 0.f);
	Vec3  AxisZ = Vec3(0.f, 0.f, 1.f);
	m_pTransformCom->Turn_Rotation_CurrentState(AxisX, fTimeDelta * m_iFront);
	m_pTransformCom->Turn_Rotation_CurrentState(AxisZ, fTimeDelta * m_iRight);

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


