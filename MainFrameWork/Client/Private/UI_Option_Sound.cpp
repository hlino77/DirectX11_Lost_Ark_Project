#include "stdafx.h"
#include "UI_Option_Sound.h"
#include "GameInstance.h"
#include "TextBox.h"

CUI_Option_Sound::CUI_Option_Sound(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}
CUI_Option_Sound::CUI_Option_Sound(const CUI& rhs)
	: CUI(rhs)
{
}


HRESULT CUI_Option_Sound::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Option_Sound::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 235.f;
	m_fSizeY = 29.f;
	m_fX = 130.f;
	m_fY = 300.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_Option_Sound::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Option_Sound::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Option_Sound::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return S_OK;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Option_Sound::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	//m_pTexture_OptionCheckBox
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_CheckBox"),
		TEXT("Com_Texture_OptionCheckBox"), (CComponent**)&m_pTexture_OptionCheckBox)))
		return E_FAIL;
	//m_pTexture_OptionDetailButton
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_DetailButton"),
		TEXT("Com_Texture_OptionDetailButton"), (CComponent**)&m_pTexture_OptionDetailButton)))
		return E_FAIL;
	//m_pTexture_Line
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_Line"),
		TEXT("Com_Texture_Line"), (CComponent**)&m_pTexture_Line)))
		return E_FAIL;
	//m_pTexture_OptionDragLine
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_DragLine"),
		TEXT("Com_Texture_OptionDragLine"), (CComponent**)&m_pTexture_OptionDragLine)))
		return E_FAIL;
	//m_pTexture_ValueTextWnd
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_ValueTextWnd"),
		TEXT("Com_Texture_ValueTextWnd"), (CComponent**)&m_pTexture_ValueTextWnd)))
		return E_FAIL;
	//m_pTexture_DragBar
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_DragBar"),
		TEXT("Com_Texture_DragBar"), (CComponent**)&m_pTexture_DragBar)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Option_Sound::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

CUI_Option_Sound* CUI_Option_Sound::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Option_Sound* pInstance = new CUI_Option_Sound(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Option_Sound");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Option_Sound::Clone(void* pArg)
{
	CUI_Option_Sound* pInstance = new CUI_Option_Sound(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Option_Sound");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Option_Sound::Free()
{
	__super::Free();

	Safe_Release(m_pTexture_OptionCheckBox);
	Safe_Release(m_pTexture_OptionDetailButton);
	Safe_Release(m_pTexture_Line);
	Safe_Release(m_pTexture_OptionDragLine);
	Safe_Release(m_pTexture_ValueTextWnd);
	Safe_Release(m_pTexture_DragBar);

	for (size_t i = 0; i < 5; i++)
	{
		Safe_Release(m_pTransform_CheckBox[i]);
	}

	if (nullptr != m_pTextBox)
		m_pTextBox->Set_Dead(true);
}
