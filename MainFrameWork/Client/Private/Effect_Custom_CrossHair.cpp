#include "stdafx.h"
#include "Effect_Custom_CrossHair.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"
#include "AsUtils.h"

CEffect_Custom_CrossHair::CEffect_Custom_CrossHair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Custom_CrossHair::CEffect_Custom_CrossHair(const CEffect_Custom_CrossHair& rhs)
	: Super(rhs)
{
}

HRESULT CEffect_Custom_CrossHair::Initialize_Prototype(EFFECTDESC* pDesc)
{
	return S_OK;
}

HRESULT CEffect_Custom_CrossHair::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strObjectTag = L"GN_CrossHair";

	POINT MousePos;

	GetCursorPos(&MousePos);
	ScreenToClient(g_hWnd, &MousePos);

	m_fX = MousePos.x;
	m_fY = MousePos.y;
	m_fDefaultSize = 250.0f;
	m_fStartSize = m_fDefaultSize * 2.0f;
	m_fAlpha = 0.0f;
	m_vColor = Vec4(0.0f, 1.0f, 0.0f, 1.0f);

	m_pTransformCom->Set_Scale(Vec3(m_fStartSize, m_fStartSize, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.8f));


	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_eState = CrossHairState::START;

	m_fShakeAcc = 0.0f;
	m_fShakeTime = 0.3f;

	return S_OK;
}

void CEffect_Custom_CrossHair::Tick(_float fTimeDelta)
{
	switch (m_eState)
	{
	case CrossHairState::START:
		Tick_Start(fTimeDelta);
		break;
	case CrossHairState::IDLE:
		Tick_Idle(fTimeDelta);
		break;
	case CrossHairState::SHAKE:
		Tick_Shake(fTimeDelta);
		break;
	case CrossHairState::EFFECTEND:
		Tick_EffectEnd(fTimeDelta);
		break;
	}
}

void CEffect_Custom_CrossHair::LateTick(_float fTimeDelta)
{
	if (m_bRender)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_WORLDUI, this);
	}
}

HRESULT CEffect_Custom_CrossHair::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	if (FAILED(m_pTextureCrossHair->Set_SRV(m_pShaderCom, "g_MaskTexture")))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	//OutCircle
	_float fAlpha = m_fAlpha;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	Vec4 vColor = Vec4(0.0f, 0.0f, 0.0f, 2.0f);
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &vColor, sizeof(Vec4))))
		return E_FAIL;

	_float fSize = m_pTransformCom->Get_Scale().x;
	m_pTransformCom->Set_Scale(Vec3(fSize * 1.6f, fSize * 1.6f, 1.0f));

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;

	if (FAILED(m_pTextureOutCircle->Set_SRV(m_pShaderCom, "g_MaskTexture")))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(Vec3(fSize, fSize, 1.0f));

	return S_OK;
}

void CEffect_Custom_CrossHair::EffectEnd()
{
	m_fDefaultSize = 1600.0f;
	m_fStartSize = m_pTransformCom->Get_Scale().x;
	m_eState = CrossHairState::EFFECTEND;
}

void CEffect_Custom_CrossHair::EffectShot()
{
	m_fShakeAcc = 0.0f;
	m_eState = CrossHairState::SHAKE;

	m_fVelShake = 90.0f;
}

void CEffect_Custom_CrossHair::Tick_Start(_float fTimeDelta)
{
	Update_Scale_Start(fTimeDelta);

	Update_Pos();
}

void CEffect_Custom_CrossHair::Tick_Idle(_float fTimeDelta)
{
	Update_Pos();
}

void CEffect_Custom_CrossHair::Tick_Shake(_float fTimeDelta)
{
	Update_Pos();

	m_fShakeAcc += fTimeDelta;
	if (m_fShakeAcc >= m_fShakeTime)
	{
		m_eState = CrossHairState::IDLE;

		m_pTransformCom->Set_Scale(Vec3(m_fDefaultSize, m_fDefaultSize, 1.0f));
		return;
	}

	_float fOffset = m_pTransformCom->Get_Scale().x - m_fDefaultSize;

	m_fVelShake += -fOffset * 60.f * fTimeDelta;

	//m_fVelShake *= 1.0f - (0.4f * fTimeDelta);

	fOffset += m_fVelShake;
	fOffset *= 1.0f - (14.0f * fTimeDelta);
	fOffset += m_fDefaultSize;

	m_pTransformCom->Set_Scale(Vec3(fOffset, fOffset, 1.0f));
}

void CEffect_Custom_CrossHair::Tick_EffectEnd(_float fTimeDelta)
{
	Update_Scale_End(fTimeDelta);
}


HRESULT CEffect_Custom_CrossHair::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_CrossHair"),
		TEXT("Com_TextureCrossHair"), (CComponent**)&m_pTextureCrossHair)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_CrossHairOutCircle"),
		TEXT("Com_TextureOutCircle"), (CComponent**)&m_pTextureOutCircle)))
		return E_FAIL;


	return S_OK;
}

void CEffect_Custom_CrossHair::Update_Pos()
{
	POINT MousePos;

	GetCursorPos(&MousePos);
	ScreenToClient(g_hWnd, &MousePos);

	m_fX = MousePos.x;
	m_fY = MousePos.y;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.8f));
}

void CEffect_Custom_CrossHair::Update_Scale_Start(_float fTimeDelta)
{
	Vec3 vScale = m_pTransformCom->Get_Scale();

	if (vScale.x != m_fDefaultSize)
	{
		vScale.y = vScale.x = CAsUtils::Lerpf(vScale.x, m_fDefaultSize, 3.0f * fTimeDelta);

		if (fabs(vScale.x - m_fDefaultSize) < 0.001f)
		{
			vScale.x = m_fDefaultSize;

			m_fAlpha = 1.0f;
			m_eState = CrossHairState::IDLE;
		}

		m_pTransformCom->Set_Scale(vScale);


		if (m_fAlpha < 1.0f)
		{
			m_fAlpha += 2.0f * fTimeDelta;

			if (m_fAlpha > 1.0f)
				m_fAlpha = 1.0f;
		}

	}
}

void CEffect_Custom_CrossHair::Update_Scale_End(_float fTimeDelta)
{
	Vec3 vScale = m_pTransformCom->Get_Scale();

	if (vScale.x != m_fDefaultSize)
	{
		vScale.y = vScale.x = CAsUtils::Lerpf(vScale.x, m_fDefaultSize, 7.0f * fTimeDelta);

		if (fabs(vScale.x - m_fDefaultSize) < 0.001f)
		{
			vScale.x = m_fDefaultSize;

			Set_Active(false);
			Set_Dead(true);
		}

		m_pTransformCom->Set_Scale(vScale);


		if (m_fAlpha > 0.0f)
		{
			m_fAlpha -= 5.0f * fTimeDelta;

			if (m_fAlpha < 0.0f)
				m_fAlpha = 0.0f;
		}
	}
}


CEffect_Custom_CrossHair* CEffect_Custom_CrossHair::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Custom_CrossHair* pInstance = new CEffect_Custom_CrossHair(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(nullptr)))
	{
		MSG_BOX("Failed To Created : CEffect_Custom_CrossHair");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Custom_CrossHair::Clone(void* pArg)
{
	CEffect_Custom_CrossHair* pInstance = new CEffect_Custom_CrossHair(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Custom_CrossHair");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Custom_CrossHair::Free()
{
	Super::Free();
}
