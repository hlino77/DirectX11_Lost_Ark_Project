#include "stdafx.h"
#include "Effect_Custom_EarthEaterParticle.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"
#include "Pool.h"

CEffect_Custom_EarthEaterParticle::CEffect_Custom_EarthEaterParticle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Custom_EarthEaterParticle::CEffect_Custom_EarthEaterParticle(const CEffect_Custom_EarthEaterParticle& rhs)
	: Super(rhs)
{
}

HRESULT CEffect_Custom_EarthEaterParticle::Initialize_Prototype(EFFECTDESC* pDesc)
{
	return S_OK;
}

HRESULT CEffect_Custom_EarthEaterParticle::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	Set_Active(false);

	return S_OK;
}

void CEffect_Custom_EarthEaterParticle::Tick(_float fTimeDelta)
{
	if (KEY_TAP(KEY::P))
	{
		Effect_Exit();
	}

	switch (m_eState)
	{
	case EarthEaterState::START:
		Tick_Start(fTimeDelta);
		break;
	case EarthEaterState::TURN:
		Tick_Turn(fTimeDelta);
		break;
	case EarthEaterState::SWING:
		Tick_Swing(fTimeDelta);
		break;
	case EarthEaterState::SHOT:
		Tick_Shot(fTimeDelta);
		break;
	case EarthEaterState::DISAPPEAR:
		Tick_Disappear(fTimeDelta);
		break;

	}
	
}

void CEffect_Custom_EarthEaterParticle::LateTick(_float fTimeDelta)
{
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	}
}

HRESULT CEffect_Custom_EarthEaterParticle::Render()
{
	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	if (FAILED(m_pDissolveTexture->Set_SRV(m_pShaderCom, "g_DissolveTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveAmount", &m_Intensity.fDissolveAmount, sizeof(_float))))
		return E_FAIL;

	for (_uint i = 0; i < m_pModelCom->Get_NumMeshes(); ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			return E_FAIL;

		MaterialFlag tFlag = { Vec4(0.f, 0.f, 0.f, 0.f) };

		if (FAILED(m_pShaderCom->Bind_CBuffer("MaterialFlag", &tFlag, sizeof(MaterialFlag))))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, "PBR_DISSOLVE")))
			return E_FAIL;
	}


	return S_OK;
}

void CEffect_Custom_EarthEaterParticle::Reset(CustomEffectDesc& tDesc)
{
	m_SwingPos.clear();
	m_TurnPos.clear();

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_tDesc = tDesc;

	Vec3 vPos = m_tDesc.vPos;

	vPos.x += pGameInstance->Get_RandomFloat(-1.0f, 1.0f);
	vPos.z += pGameInstance->Get_RandomFloat(-1.0f, 1.0f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	vPos.y += CGameInstance::GetInstance()->Get_RandomFloat(0.5f, 1.0f);
	m_vTargetPos = vPos;

	m_Intensity.fDissolveAmount = 0.0f;

	m_eState = EarthEaterState::START;

	m_pTransformCom->My_Rotation(Vec3(pGameInstance->Get_RandomFloat(0.0f, 360.0f), pGameInstance->Get_RandomFloat(0.0f, 360.0f), pGameInstance->Get_RandomFloat(0.0f, 360.0f)));
	m_vAxisSpeed = Vec3(pGameInstance->Get_RandomFloat(3.0f, 6.0f), pGameInstance->Get_RandomFloat(3.0f, 6.0f), pGameInstance->Get_RandomFloat(3.0f, 6.0f));

	Init_TurnPos();
	Init_SwingPos();

	m_fTurnRatio = 0.0f;
	m_fSwingRatio = 0.0f;


	m_fTimeAcc = 0.0f;
	m_fLifeTime = 1.0f;

	Set_Active(true);

	Safe_Release(pGameInstance);
}



void CEffect_Custom_EarthEaterParticle::Tick_Start(_float fTimeDelta)
{
	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float fDistance = (m_vTargetPos - vPos).Length();

	if (fDistance < 0.01f)
	{
		m_eState = EarthEaterState::TURN;
	}
	else
	{
		vPos = Vec3::Lerp(vPos, m_vTargetPos, 5.0f * fTimeDelta);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}

	Update_Rotation(fTimeDelta);
}

void CEffect_Custom_EarthEaterParticle::Tick_Turn(_float fTimeDelta)
{
	Update_Rotation(fTimeDelta);

	_uint iSize = m_TurnPos.size();

	if (iSize >= 4)
	{
		m_fTurnRatio += m_fTurnSpeed * fTimeDelta;

		if (m_fTurnRatio >= 1.0f)
		{
			if (iSize <= 4)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_TurnPos[2]);
				m_TurnPos.pop_front();
				return;
			}
			else
			{
				m_TurnPos.pop_front();
				m_fTurnRatio -= 1.0f;
			}
		}

		Vec3 vPos = XMVectorCatmullRom(m_TurnPos[0], m_TurnPos[1], m_TurnPos[2], m_TurnPos[3], m_fTurnRatio);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
}

void CEffect_Custom_EarthEaterParticle::Tick_Swing(_float fTimeDelta)
{
	Update_Rotation(fTimeDelta);

	_uint iSize = m_SwingPos.size();

	if (iSize >= 4)
	{
		m_fSwingRatio += m_fSwingSpeed * fTimeDelta;

		if (m_fSwingRatio >= 1.0f)
		{
			if (iSize <= 4)
			{
				m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_SwingPos[2]);
				m_SwingPos.pop_front();
				m_eState = EarthEaterState::SHOT;
				Init_Shot();
				return;
			}
			else
			{
				m_SwingPos.pop_front();
				m_fSwingRatio -= 1.0f;
			}
		}

		Vec3 vPos = XMVectorCatmullRom(m_SwingPos[0], m_SwingPos[1], m_SwingPos[2], m_SwingPos[3], m_fSwingRatio);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
}

void CEffect_Custom_EarthEaterParticle::Tick_Shot(_float fTimeDelta)
{
	Update_Rotation(fTimeDelta);

	Update_RockPos(fTimeDelta);
}

void CEffect_Custom_EarthEaterParticle::Tick_Disappear(_float fTimeDelta)
{
	if (m_fTimeAcc >= m_fLifeTime)
	{
		m_Intensity.fDissolveAmount += fTimeDelta;

		if (m_Intensity.fDissolveAmount > 1.1f)
		{
			Set_Active(false);
			CPool<CEffect_Custom_EarthEaterParticle>::Return_Obj(this);
		}
	}
	else
	{
		m_fTimeAcc += fTimeDelta;
	}
}

void CEffect_Custom_EarthEaterParticle::Set_Swing()
{
	m_eState = EarthEaterState::SWING;

	Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_SwingPos.push_front(vPos);
	
	vPos.y -= 1.0f;
	m_SwingPos.push_front(vPos);
}

void CEffect_Custom_EarthEaterParticle::Effect_Exit()
{
	Set_Active(false);
	CPool<CEffect_Custom_EarthEaterParticle>::Return_Obj(this);
}

void CEffect_Custom_EarthEaterParticle::Update_Rotation(_float fTimeDelta)
{
	Vec3 vRight = m_pTransformCom->Get_State(CTransform::STATE_RIGHT);
	m_pTransformCom->Turn_Axis(vRight, m_vAxisSpeed.x * fTimeDelta);

	Vec3 vUp = m_pTransformCom->Get_State(CTransform::STATE_UP);
	m_pTransformCom->Turn_Axis(vUp, m_vAxisSpeed.y * fTimeDelta);

	Vec3 vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	m_pTransformCom->Turn_Axis(vLook, m_vAxisSpeed.z * fTimeDelta);
}

void CEffect_Custom_EarthEaterParticle::Init_TurnPos()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Vec3 vTargetPos = m_vTargetPos;
	vTargetPos.y += 1.0f;

	m_TurnPos.push_back(vTargetPos);
	m_TurnPos.push_back(m_vTargetPos);

	Vec3 vPlayerPos = m_tDesc.vPlayerPos;
	vPlayerPos += m_tDesc.vLook * 0.5f;
	Vec3 vOffset = m_vTargetPos - m_tDesc.vPlayerPos;
	vOffset.y = 0.0f;
	vOffset.Normalize();


	for (_uint i = 1; i < 10; ++i)
	{
		Vec3 vPos = XMVector3Rotate(vOffset, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(-36.0f * i)));
		vPos *= pGameInstance->Get_RandomFloat(1.5f, 2.0f);
		vPos.y += pGameInstance->Get_RandomFloat(0.6f, 1.0f);

		m_TurnPos.push_back(vPlayerPos + vPos);
	}

	m_fTurnSpeed = pGameInstance->Get_RandomFloat(9.0f, 10.0f);

	/*if (m_tDesc.bSmall == true)
	{
		for (_uint i = 1; i < 10; ++i)
		{
			Vec3 vPos = XMVector3Rotate(vOffset, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(-36.0f * i)));
			vPos *= pGameInstance->Get_RandomFloat(1.5f, 2.0f);
			vPos.y += pGameInstance->Get_RandomFloat(0.6f, 1.0f);

			m_TurnPos.push_back(vPlayerPos + vPos);
		}

		m_fTurnSpeed += 10.0f;
	}*/

	Vec3 vEffectPos = m_tDesc.vPos;

	vEffectPos.x += pGameInstance->Get_RandomFloat(-0.5f, 0.5f);
	vEffectPos.z += pGameInstance->Get_RandomFloat(-0.5f, 0.5f);
	vEffectPos.y += pGameInstance->Get_RandomFloat(1.0f, 1.5f);

	m_TurnPos.push_back(vEffectPos);
	vEffectPos.y += 1.0f;
	m_TurnPos.push_back(vEffectPos);

	Safe_Release(pGameInstance);
}

void CEffect_Custom_EarthEaterParticle::Init_SwingPos()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_fSwingSpeed = 14.0f;

	{
		Vec3 vPos = m_tDesc.vPlayerPos;
		vPos.y += 2.0f;
		vPos.x += pGameInstance->Get_RandomFloat(-0.5f, 0.5f);
		vPos.z += pGameInstance->Get_RandomFloat(-0.5f, 0.5f);
		vPos.y += pGameInstance->Get_RandomFloat(-0.5f, 0.5f);
		m_SwingPos.push_back(vPos);
	}

	{
		Vec3 vPos = m_tDesc.vPlayerPos;
		vPos += m_tDesc.vLook * 1.7f;
		vPos.y += 2.5f;
		vPos.x += pGameInstance->Get_RandomFloat(-0.5f, 0.5f);
		vPos.z += pGameInstance->Get_RandomFloat(-0.5f, 0.5f);
		vPos.y += pGameInstance->Get_RandomFloat(-0.5f, 0.5f);
		m_SwingPos.push_back(vPos);
	}
	
	{
		Vec3 vPos = m_tDesc.vPlayerPos;
		vPos += m_tDesc.vLook * 3.5f;
		vPos.y += 0.1f;
		vPos.x += pGameInstance->Get_RandomFloat(-0.5f, 0.5f);
		vPos.z += pGameInstance->Get_RandomFloat(-0.5f, 0.5f);
		m_SwingPos.push_back(vPos);
	}


	{
		Vec3 vPos = m_tDesc.vPlayerPos;
		vPos += m_tDesc.vLook * 3.5f;
		vPos.y -= 0.5f;
		m_SwingPos.push_back(vPos);
	}

	Safe_Release(pGameInstance);
}

void CEffect_Custom_EarthEaterParticle::Init_Shot()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Vec3 vEndPos = m_tDesc.vPlayerPos;
	vEndPos += m_tDesc.vLook * 3.5f;
	vEndPos.y += 0.1f;

	Vec3 vPos = m_vStartPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	Vec3 vDir = vPos - vEndPos;

	m_vTargetPos = vPos + vDir * 3.0f;
	

	//
	{
		_float fMaxTime = pGameInstance->Get_RandomFloat(0.3f, 0.5f);

		Vec3 vTargetPos = m_vTargetPos;
		_float fEndHeight = vTargetPos.y - m_vStartPos.y;
		_float fHeight = pGameInstance->Get_RandomFloat(1.0f, 1.5f);

		m_fGravity = 2.0f * fHeight / (fMaxTime * fMaxTime);

		m_fVelocityY = sqrtf(2.0f * m_fGravity * fHeight);

		_float a = m_fGravity;
		_float b = -2.0f * m_fVelocityY;
		_float c = 2.0f * fEndHeight;

		m_fEndTime = (-b + sqrtf(b * b - 4.0f * a * c)) / (2.0f * a);

		m_fVelocityX = -(m_vStartPos.x - vTargetPos.x) / m_fEndTime;
		m_fVelocityZ = -(m_vStartPos.z - vTargetPos.z) / m_fEndTime;

		m_fTimeAcc = 0.0f;

	}
	
	Safe_Release(pGameInstance);
}

void CEffect_Custom_EarthEaterParticle::Update_RockPos(_float fTimedelta)
{
	m_fTimeAcc += fTimedelta;

	m_fTimeAcc = min(m_fTimeAcc, m_fEndTime);

	Vec3 vPos;

	vPos.x = m_vStartPos.x + m_fVelocityX * m_fTimeAcc;
	vPos.y = m_vStartPos.y + (m_fVelocityY * m_fTimeAcc) - (0.5f * m_fGravity * m_fTimeAcc * m_fTimeAcc);
	vPos.z = m_vStartPos.z + m_fVelocityZ * m_fTimeAcc;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);


	_float fDistance = (m_vTargetPos - vPos).Length();
	if (fDistance < 0.1f)
	{
		m_eState = EarthEaterState::DISAPPEAR;
	}
}
		

HRESULT CEffect_Custom_EarthEaterParticle::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;
	
	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Dissolve1"),
		TEXT("Com_TextureDissolve"), (CComponent**)&m_pDissolveTexture)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_ModelEffect"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_Model */

	wstring strModelName = L"RockParticle";
	wstring strComName = L"Prototype_Component_Model_";
	
	strModelName += to_wstring(rand() % 3);
	
	strComName += strModelName;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	Vec3 vScale;
	vScale.y = 1.0f;
	vScale.z = 1.0f;
	vScale.x = 1.0f;

	m_pTransformCom->Set_Scale(vScale);

	return S_OK;
}

CEffect_Custom_EarthEaterParticle* CEffect_Custom_EarthEaterParticle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Custom_EarthEaterParticle* pInstance = new CEffect_Custom_EarthEaterParticle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(nullptr)))
	{
		MSG_BOX("Failed To Created : CEffect_Custom_EarthEaterParticle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Custom_EarthEaterParticle::Clone(void* pArg)
{
	CEffect_Custom_EarthEaterParticle* pInstance = new CEffect_Custom_EarthEaterParticle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Custom_EarthEaterParticle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Custom_EarthEaterParticle::Free()
{
	Super::Free();
}
