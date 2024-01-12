#include "stdafx.h"
#include "Effect_Custom_SpiralChaser.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"

CEffect_Custom_SpiralChaser::CEffect_Custom_SpiralChaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Custom_SpiralChaser::CEffect_Custom_SpiralChaser(const CEffect_Custom_SpiralChaser& rhs)
	: Super(rhs)
{
}

HRESULT CEffect_Custom_SpiralChaser::Initialize_Prototype(EFFECTDESC* pDesc)
{
	return S_OK;
}

HRESULT CEffect_Custom_SpiralChaser::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Turn_Axis(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(90.0f));
	m_pTransformCom->Turn_Axis(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(-90.0f));

	m_fBulletTime = 0.05f;
	m_fBulletAcc = 0.0f;

	m_vOffsetLook = Vec3(CGameInstance::GetInstance()->Get_RandomFloat(-1.0f, 1.0f), 0.0f, CGameInstance::GetInstance()->Get_RandomFloat(-1.0f, 1.0f));
	m_vOffsetLook.Normalize();

	SpiralChaserDesc* pDesc = static_cast<SpiralChaserDesc*>(pArg);
	m_pTransformCom->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, pDesc->vPos);
	m_vTargetPos = pDesc->vTargetPos;

	m_bAttackStart = false;

	m_fLifeTime = 2.0f;
	m_fTimeAcc = 0.0f;

	if (pDesc->pPlayer)
	{
		m_pPlayer = pDesc->pPlayer;
		m_iSkillKey = pDesc->iSkillKey;
	}

	return S_OK;
}

void CEffect_Custom_SpiralChaser::Tick(_float fTimeDelta)
{
	m_pTransformCom->Turn_Speed(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), -30.0f, fTimeDelta);


	if (m_bAttackStart)
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc >= m_fLifeTime)
		{
			Set_Active(false);
			Set_Dead(true);
			return;
		}


		m_fBulletAcc += fTimeDelta;
		if (m_fBulletAcc >= m_fBulletTime)
		{
			m_fBulletAcc = 0.0f;
			Effect_Bullet();
		}

		Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos = Vec3::Lerp(vPos, m_vTargetPos, fTimeDelta * 10.0f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}
	else
	{
		Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos = Vec3::Lerp(vPos, m_vTargetPos, fTimeDelta * 10.0f);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		_float fLength = (vPos - m_vTargetPos).Length();
		if (fLength < 0.3f)
		{
			m_bAttackStart = true;
			if(m_pPlayer && m_pPlayer->Is_Control())
				m_pPlayer->Get_GN_Controller()->Get_SkillAttackMessage((CPlayer_Controller::SKILL_KEY)m_iSkillKey, m_vTargetPos);
		}
			
	}
	
}

void CEffect_Custom_SpiralChaser::LateTick(_float fTimeDelta)
{
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	}
}

HRESULT CEffect_Custom_SpiralChaser::Render()
{
	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Custom_SpiralChaser::Effect_Bullet()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	{
		Vec3 vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		Vec3 vOriginLook = m_vOffsetLook = XMVector3Rotate(m_vOffsetLook, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(pGameInstance->Get_RandomFloat(23.0f, 99.0f))));

		Vec3 vOriginUp(0.0f, 1.0f, 0.0f);

		Vec3 vOriginRight = vOriginUp.Cross(vOriginLook);
		vOriginRight.Normalize();

		vPos += vOriginLook * 0.2f;
		Matrix matWorld = Matrix::CreateWorld(vPos, -vOriginLook, vOriginUp);

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matWorld;
		EFFECT_START(TEXT("DeathFireBullet"), &desc)

		_uint iCount = rand() % 2 + 1;
		for (_uint i = 0; i < iCount; ++i)
		{
			_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);
			_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);


			Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

			Matrix matEffectWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

			CEffect_Manager::EFFECTPIVOTDESC desc;
			desc.pPivotMatrix = &matEffectWorld;

			EFFECT_START(TEXT("DeathFireBulletParticle"), &desc)
		}
	}

	Safe_Release(pGameInstance);
}

HRESULT CEffect_Custom_SpiralChaser::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_Effect_Custom_SpiralChaser";
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName,
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	Vec3 vScale;
	vScale.y = 1.3f;
	vScale.z = 1.3f;
	vScale.x = 1.3f;

	m_pTransformCom->Set_Scale(vScale);

	return S_OK;
}

CEffect_Custom_SpiralChaser* CEffect_Custom_SpiralChaser::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Custom_SpiralChaser* pInstance = new CEffect_Custom_SpiralChaser(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(nullptr)))
	{
		MSG_BOX("Failed To Created : CEffect_Custom_SprialChaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Custom_SpiralChaser::Clone(void* pArg)
{
	CEffect_Custom_SpiralChaser* pInstance = new CEffect_Custom_SpiralChaser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Custom_SprialChaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Custom_SpiralChaser::Free()
{
	Super::Free();
}
