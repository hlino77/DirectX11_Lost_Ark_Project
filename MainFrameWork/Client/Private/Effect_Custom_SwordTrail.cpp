#include "stdafx.h"
#include "Effect_Custom_SwordTrail.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Player_Controller_GN.h"
#include "Player_Gunslinger.h"

CEffect_Custom_SwordTrail::CEffect_Custom_SwordTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Custom_SwordTrail::CEffect_Custom_SwordTrail(const CEffect_Custom_SwordTrail& rhs)
	: Super(rhs)
	, m_iTrailVtxCount(rhs.m_iTrailVtxCount)
{
}

HRESULT CEffect_Custom_SwordTrail::Initialize_Prototype(EFFECTDESC* pDesc)
{
	return S_OK;
}

HRESULT CEffect_Custom_SwordTrail::Initialize(void* pArg)
{
	if (FAILED(Super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pBuffer->Set_VtxCount(m_iTrailVtxCount);

	return S_OK;
}

void CEffect_Custom_SwordTrail::Tick(_float fTimeDelta)
{
	Super::Tick(fTimeDelta);

	if (m_bActive == false)
		return;

	if (m_fStartDelayAcc < m_fStartDelay)
		return;

	Run_Sequence(fTimeDelta);
}

void CEffect_Custom_SwordTrail::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	if (m_bRender)
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this)))
			__debugbreak();
	}
	
	m_matCombined = m_matOffset * m_matPivot;
}

HRESULT CEffect_Custom_SwordTrail::Render()
{
	if (FAILED(Super::Render()))
		return E_FAIL;

	//m_pBuffer->Update_TrailBuffer();

	if (FAILED(m_pShaderCom->Begin(m_strPassName)))
		return E_FAIL;
	if (FAILED(m_pBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Custom_SwordTrail::Reset(CEffect_Manager::EFFECTPIVOTDESC& tEffectDesc)
{
	Super::Reset(tEffectDesc);

	m_pBuffer->Stop_Trail();
}

void CEffect_Custom_SwordTrail::Run_Sequence(const _float& fTimeDelta)
{
	if (m_IsSequence && m_bRender)
	{
		m_fSequenceTimer += fTimeDelta;
		while (m_fSequenceTimer >= m_fSequenceTerm)
		{
			m_fSequenceTimer -= m_fSequenceTerm;
			++m_Variables.vUV_TileIndex.x;

			if (m_Variables.vUV_TileIndex.x >= m_Variables.vUV_TileCount.x)
			{
				m_Variables.vUV_TileIndex.x = 0.0f;
				++m_Variables.vUV_TileIndex.y;
				if (m_Variables.vUV_TileIndex.y >= m_Variables.vUV_TileCount.y)
				{
					if (m_IsLoop)
						m_Variables.vUV_TileIndex.y = 0.0f;
					else
						m_bRender = false;
				}
			}
		}
	}
}

HRESULT CEffect_Custom_SwordTrail::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Buffer */
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"), TEXT("Com_VIBuffer"), (CComponent**)&m_pBuffer)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectTrail"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CEffect_Custom_SwordTrail* CEffect_Custom_SwordTrail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Custom_SwordTrail* pInstance = new CEffect_Custom_SwordTrail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(nullptr)))
	{
		MSG_BOX("Failed To Created : CEffect_Custom_SprialChaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Custom_SwordTrail::Clone(void* pArg)
{
	CEffect_Custom_SwordTrail* pInstance = new CEffect_Custom_SwordTrail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Custom_SprialChaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Custom_SwordTrail::Free()
{
	Super::Free();
}
