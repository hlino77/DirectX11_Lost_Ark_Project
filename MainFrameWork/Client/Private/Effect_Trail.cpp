#include "stdafx.h"
#include "Effect_Trail.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "VIBuffer_Point.h"
#include "PartObject.h"

CEffect_Trail::CEffect_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Trail::CEffect_Trail(const CEffect_Trail& rhs)
	: Super(rhs)
	, m_iTrailVtxCount(rhs.m_iTrailVtxCount)
{
}

HRESULT CEffect_Trail::Initialize_Prototype(EFFECTDESC* pDesc)
{
	if (FAILED(Super::Initialize_Prototype(pDesc)))
		return E_FAIL;

	m_iTrailVtxCount = pDesc->iTrailVtxCount;

	return S_OK;
}

HRESULT CEffect_Trail::Initialize(void* pArg)
{
	if (FAILED(Super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pBuffer->Set_VtxCount(m_iTrailVtxCount);

	return S_OK;
}

void CEffect_Trail::Tick(_float fTimeDelta)
{
	Super::Tick(fTimeDelta);

	if (m_bActive == false)
		return;

	if (m_fWaitingAcc < m_fWaitingTime)
		return;

	CB_UpdatePivot(m_matPivot);

	Run_Sequence(fTimeDelta);
}

void CEffect_Trail::LateTick(_float fTimeDelta)
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

HRESULT CEffect_Trail::Render()
{
	if (FAILED(Super::Render()))
		return E_FAIL;

	m_pBuffer->Update_TrailBuffer(m_matCombined);

	if (FAILED(m_pShaderCom->Begin(m_strPassName)))
		return E_FAIL;
	if (FAILED(m_pBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Trail::Reset()
{
	m_fSequenceTimer = 0.0f;
	m_Variables.vUV_TileIndex = Vec2(0.0f, 0.0f);
	m_fTimeAcc = 0.0f;
	m_bRender = true;

	if (m_fWaitingTime > 0.0f)
	{
		m_fWaitingAcc = 0.0f;
		m_bRender = false;
	}

	m_pBuffer->Stop_Trail();
}

HRESULT CEffect_Trail::Ready_Components()
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

void CEffect_Trail::Run_Sequence(const _float& fTimeDelta)
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

CEffect_Trail* CEffect_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, EFFECTDESC* pDesc)
{
	CEffect_Trail* pInstance = new CEffect_Trail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pDesc)))
	{
		MSG_BOX("Failed To Created : CEffect_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Trail::Clone(void* pArg)
{
	CEffect_Trail* pInstance = new CEffect_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Trail::Free()
{
	Super::Free();
	Safe_Release(m_pBuffer);
}
