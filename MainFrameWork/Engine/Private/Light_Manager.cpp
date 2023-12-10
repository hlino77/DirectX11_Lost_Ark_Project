#include "..\Public\Light_Manager.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

const LIGHTDESC * CLight_Manager::Get_LightDesc(_uint iIndex)
{
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

HRESULT CLight_Manager::Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc, CTexture* pTexture)
{
	CLight* pLight = CLight::Create(pDevice, pContext, LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	pLight->Set_StaticShadowMap(pTexture);
	m_Lights.push_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Render(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	for (auto& pLight : m_Lights)
		pLight->Render(pShader, pVIBuffer);

	return S_OK;
}

HRESULT CLight_Manager::Reset_Lights()
{
	Free();
	m_DirectionLightMatrix = XMMatrixIdentity();
	return S_OK;
}

void CLight_Manager::Ready_LightMatrix(Vec3 vOffset, Vec3 vLook)
{
	m_vLightOffset = vOffset;
	m_vLightLook = vLook;
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
