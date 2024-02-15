#include "..\Public\Light_Manager.h"
#include "Light.h"
#include "Texture.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

const LIGHTDESC * CLight_Manager::Get_LightDesc(_uint iIndex)
{
	if (m_Lights.empty())
		return nullptr;

	auto	iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_LightDesc();	
}

HRESULT CLight_Manager::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc)
{
	CLight*		pLight = CLight::Create(pDevice, pContext, LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Bind_LightDescription(CShader* pShader)
{
	if (!m_Lights.empty())
	{
		const Color& vLightDiffuse = m_Lights.front()->Get_LightDesc()->vDiffuse;
		if (FAILED(pShader->Bind_RawValue("g_vLightDiffuse", &vLightDiffuse, sizeof(Color))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLight_Manager::Bind_LightShadowTexture(CShader* pShader)
{
	_int iShadow = true;
	if (m_pStaticShadowMap)
	{
		pShader->Bind_RawValue("g_bShadow", &iShadow, sizeof(_int));
		if (FAILED(pShader->Bind_Texture("g_StaticShadowDepthTarget", m_pStaticShadowMap->Get_SRV())))
			return E_FAIL;
	}
	else
	{
		iShadow = false;
		pShader->Bind_RawValue("g_bShadow", &iShadow, sizeof(_int));
	}

	return S_OK;
}

HRESULT CLight_Manager::Clear_LightShadowTexture()
{
	if (m_pStaticShadowMap)
		Safe_Release(m_pStaticShadowMap);

	return S_OK;
}

HRESULT CLight_Manager::Set_LightShadowTexture(CTexture* pTexture)
{
	if (nullptr == pTexture || nullptr != m_pStaticShadowMap)
		return E_FAIL;

	m_pStaticShadowMap = pTexture;

	return S_OK;
}

HRESULT CLight_Manager::Reset_Lights()
{
	WRITE_LOCK

	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();

	m_DirectionLightMatrix = XMMatrixIdentity();
	return S_OK;
}

void CLight_Manager::Ready_LightMatrix(Vec3 vOffset, Vec3 vLook)
{
	m_vLightOffset = vOffset;
	m_vLightLook = vLook;
	m_vLightLook.Normalize();
	Vec3 vPos(0.0f, 0.0f, 0.0f);

	m_DirectionLightMatrix = XMMatrixLookAtLH(vPos, vPos + vLook, Vec3(0.0f, 1.0f, 0.0f));
}

void CLight_Manager::Ready_StaticLightMatrix(Vec3 vPos, Vec3 vLook)
{
	m_StaticLightMatrix = XMMatrixLookAtLH(vPos, vPos + vLook, Vec3(0.0f, 1.0f, 0.0f));
}


void CLight_Manager::Update_LightMatrix(Vec3 vPos)
{
	//vPos.y = 0.0f;
	Vec3 vLightPos = vPos + m_vLightOffset;
	m_DirectionLightMatrix = XMMatrixLookAtLH(vLightPos, vLightPos + m_vLightLook, Vec3(0.0f, 1.0f, 0.0f));
}

void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}
