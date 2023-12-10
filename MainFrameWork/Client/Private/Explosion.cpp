#include "stdafx.h"
#include "Explosion.h"
#include "Client_Defines.h"
#include "Transform.h"
#include "Shader.h"
#include "GameInstance.h"
#include "VIBuffer_Point.h"
#include "RigidBody.h"
#include "Pool.h"

CExplosion::CExplosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect(pDevice, pContext)
{
}

CExplosion::CExplosion(const CExplosion& rhs)
	: CEffect(rhs)
{
}

HRESULT CExplosion::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CExplosion::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	m_szModelName = L"Effect_Explosion";

	m_bActive = false;

	m_vColor = Vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_vBlurColor = Vec4(0.0f, 0.0f, 0.0f, 0.0f);

	m_vUVMaxCount = Vec2(8.0f, 8.0f);
	

	m_fUVTime = 0.01f;
	m_fCurrUVTime = 0.0f;

	m_fDiscardAlpha = 0.4f;

	m_fAlphaWeight = 1.0f;

	m_pRendererCom->Reserve_RenderGroup(CRenderer::RENDER_EFFECT_INSTANCE, this);

	return S_OK;
}

void CExplosion::Tick(_float fTimeDelta)
{
	if (m_bStart)
	{
		m_vScale = Vec2::Lerp(m_vScale, m_vTargetScale, 5.0f * fTimeDelta);

		Update_UV(fTimeDelta);

		if (m_bEnd)
		{
			Set_Active(false);
			CPool<CExplosion>::Return_Obj(this);

			/*m_fAlphaWeight += 1.0f * fTimeDelta;
			if (m_fAlphaWeight >= 1.0f)
			{
				m_fAlphaWeight = 1.0f;

				Set_Active(false);
				CPool<CExplosion>::Return_Obj(this);
			}*/
		}

	}
	else
	{
		m_fStartTime -= fTimeDelta;
		if (m_fStartTime <= 0.0f)
			m_bStart = true;
	}



}

void CExplosion::LateTick(_float fTimeDelta)
{
	if(m_bStart)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT_INSTANCE, this);
}

HRESULT CExplosion::Render()
{
	return S_OK;
}

HRESULT CExplosion::Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &Vec3(pGameInstance->Get_CamPosition()), sizeof(Vec3))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDiscardAlpha", &m_fDiscardAlpha, sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pTextureCom->Set_SRV(m_pShaderCom, "g_MaskTexture")))
		return E_FAIL;


	if (FAILED(m_pMaskTexture->Set_SRV(m_pShaderCom, "g_MaskTexture2")))
		return E_FAIL;

	if (FAILED(m_pDisolveTexture->Set_SRV(m_pShaderCom, "g_DisolveTexture")))
		return E_FAIL;



	m_pShaderCom->Begin(3);

	m_pVIBufferCom->Render_Instance(pInstanceBuffer, iSize, sizeof(VTXINSTANCE_POINTEFFECT));


	Safe_Release(pGameInstance);
	return S_OK;
}

void CExplosion::Add_InstanceData(vector<Vec4>& BufferData)
{
	Vec4 vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	Vec4 vScale = m_vScale;


	m_vBlurColor.x = m_fAlphaWeight;

	BufferData.push_back(vPosition);
	BufferData.push_back(Vec4(m_vUVIndex.x, m_vUVIndex.y, m_vUVMaxCount.x, m_vUVMaxCount.y));
	BufferData.push_back(vScale);
	BufferData.push_back(m_vColor);
	BufferData.push_back(m_vBlurColor);
}

void CExplosion::Appear(Vec3 vPos)
{
	m_bActive = true;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	//m_vUVIndex = Vec2(rand() % 4 * 1.0f, rand() % 3 * 1.0f);

	m_vUVIndex = Vec2(0.0f, 0.0f);

	m_vColor.w = 1.0f;


	_float fScale = (rand() % 40 + 30) * 0.01f;

	m_vScale.x = fScale;
	m_vScale.y = fScale;

	m_bEnd = false;
}

void CExplosion::Appear(_float fScale, Vec3 vPos, _float fLength, _float fTime)
{
	Vec3 vEffectPos(0.0f, 0.0f, 0.0f);

	vEffectPos.x += (rand() % 100 - 50) * 1.0f;
	vEffectPos.y += (rand() % 100 - 50) * 1.0f;
	vEffectPos.z += (rand() % 100 - 50) * 1.0f;

	vEffectPos.Normalize();
	vEffectPos *= fLength;

	vEffectPos += vPos;

	m_fStartTime = fTime;

	m_bActive = true;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEffectPos);
	m_vUVIndex = Vec2(0.0f, 0.0f);
	m_vColor.w = 1.0f;

	_float fTargetScale = (rand() % 40 + 30) * fScale;


	m_fAlphaWeight = 0.0f;

	m_bEnd = false;
	m_bStart = false;

	m_vScale = Vec2(fScale, fScale);
	m_vTargetScale = Vec2(fTargetScale, fTargetScale);
}

void CExplosion::Appear_Up(_float fScale, Vec3 vPos, _float fLength, _float fTime)
{
	Vec3 vEffectPos(0.0f, 0.0f, 0.0f);

	vEffectPos.x += (rand() % 100 - 50) * 1.0f;
	vEffectPos.y += (rand() % 100 - 50) * 1.0f;
	if (vEffectPos.y < 0.0f)
		vEffectPos.y *= -1.0f;

	vEffectPos.z += (rand() % 100 - 50) * 1.0f;

	vEffectPos.Normalize();
	vEffectPos *= fLength;

	vEffectPos += vPos;

	m_fStartTime = fTime;

	m_bActive = true;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vEffectPos);
	m_vUVIndex = Vec2(0.0f, 0.0f);
	m_vColor.w = 1.0f;

	_float fTargetScale = (rand() % 40 + 30) * fScale;


	m_fAlphaWeight = 0.0f;

	m_bEnd = false;
	m_bStart = false;

	m_vScale = Vec2(fScale, fScale);
	m_vTargetScale = Vec2(fTargetScale, fTargetScale);
}


HRESULT CExplosion::Ready_Components()
{
	__super::Ready_Components();

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect_PointToTex_Instance"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;


	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Explosion_BC"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Explosion_M"),
		TEXT("Com_MaskTexture"), (CComponent**)&m_pMaskTexture)))
		return E_FAIL;

	/*Com_Texture*/
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_ExplosionDisolve"),
			TEXT("Com_DisolveTexture"), (CComponent**)&m_pDisolveTexture)))
			return E_FAIL;

	return S_OK;
}

void CExplosion::Update_UV(_float fTimeDelta)
{
	m_fCurrUVTime += fTimeDelta;

	if (m_fCurrUVTime >= m_fUVTime)
	{
		m_vUVIndex.x += 1.0f;

		if (m_vUVIndex.x >= m_vUVMaxCount.x)
		{
			m_vUVIndex.x = 0.0f;

			m_vUVIndex.y += 1.0f;

			/*if (m_vUVIndex.y >= 3.0f)
				m_bEnd = true;*/

			if (m_vUVIndex.y >= m_vUVMaxCount.y)
			{
				m_vUVIndex.y = 0.0f;
				m_bEnd = true;
			}
		}

		m_fCurrUVTime = 0.0f;
	}
}


CGameObject* CExplosion::Clone(void* pArg)
{
	CExplosion* pInstance = new CExplosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CExplosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CExplosion* CExplosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CExplosion* pInstance = new CExplosion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CExplosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CExplosion::Free()
{
	__super::Free();
}
