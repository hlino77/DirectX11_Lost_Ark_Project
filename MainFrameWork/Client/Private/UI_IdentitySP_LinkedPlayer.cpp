#include "stdafx.h"
#include "UI_IdentitySP_LinkedPlayer.h"
#include "Player.h"
#include "GameInstance.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"

CUI_IdentitySP_LinkedPlayer::CUI_IdentitySP_LinkedPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_IdentitySP_LinkedPlayer::CUI_IdentitySP_LinkedPlayer(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_IdentitySP_LinkedPlayer::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_IdentitySP_LinkedPlayer");

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = 850.f;

	//m_pTransform_BubbleGaugeL
	m_pTransform_BubbleGaugeL->Set_Scale(Vec3(52.f, 52.f, 1.f));
	m_pTransform_BubbleGaugeL->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_BubbleGaugeC
	m_pTransform_BubbleGaugeC->Set_Scale(Vec3(52.f, 52.f, 1.f));
	m_pTransform_BubbleGaugeC->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_BubbleGaugeR
	m_pTransform_BubbleGaugeR->Set_Scale(Vec3(52.f, 52.f, 1.f));
	m_pTransform_BubbleGaugeR->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_SkillL
	m_pTransform_SkillL->Set_Scale(Vec3(52.f, 52.f, 1.f));
	m_pTransform_SkillL->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 50.f) - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransform_SkillR
	m_pTransform_SkillR->Set_Scale(Vec3(52.f, 52.f, 1.f));
	m_pTransform_SkillR->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 50.f) - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransfrom_SkillFrameL
	m_pTransfrom_SkillFrameL->Set_Scale(Vec3(56.f, 56.f, 1.f));
	m_pTransfrom_SkillFrameL->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 50.f) - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
	//m_pTransfrom_SkillFrameR
	m_pTransfrom_SkillFrameR->Set_Scale(Vec3(56.f, 56.f, 1.f));
	m_pTransfrom_SkillFrameR->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 50.f) - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    return S_OK;
}

void CUI_IdentitySP_LinkedPlayer::Tick(_float fTimeDelta)
{
}

void CUI_IdentitySP_LinkedPlayer::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_IdentitySP_LinkedPlayer::Render()
{
    return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	//m_pTexture_BubbleGauge
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_GaugeEmpty"),
		TEXT("Com_Texture_BubbleGauge"), (CComponent**)&m_pTexture_BubbleGauge)))
		return E_FAIL;
	//m_pTexture_GaugeCut
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_GaugeEmpty"),
		TEXT("Com_Texture_GaugeCut"), (CComponent**)&m_pTexture_GaugeCut)))
		return E_FAIL;
	//m_pTexture_MoonFall
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_GaugeEmpty"),
		TEXT("Com_Texture_MoonFall"), (CComponent**)&m_pTexture_MoonFall)))
		return E_FAIL;
	//m_pTexture_SunRise
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_GaugeEmpty"),
		TEXT("Com_Texture_SunRise"), (CComponent**)&m_pTexture_SunRise)))
		return E_FAIL;
	//m_pSkill_Frame
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_GaugeEmpty"),
		TEXT("Com_Texture_SkillFrame"), (CComponent**)&m_pTexture_SkillFrame)))
		return E_FAIL;


	//m_pTransform_BubbleGaugeL
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BubbleGaugeL"), (CComponent**)&m_pTransform_BubbleGaugeL)))
		return E_FAIL;
	//m_pTransform_BubbleGaugeC
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BubbleGaugeC"), (CComponent**)&m_pTransform_BubbleGaugeC)))
		return E_FAIL;
	//m_pTransform_BubbleGaugeR
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BubbleGaugeR"), (CComponent**)&m_pTransform_BubbleGaugeR)))
		return E_FAIL;
	//m_pTransform_SkillL
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_SkillL"), (CComponent**)&m_pTransform_SkillL)))
		return E_FAIL;
	//m_pTransform_SkillR
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_SkillR"), (CComponent**)&m_pTransform_SkillR)))
		return E_FAIL;
	//m_pTransfrom_SkillFrameL
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_SkillFrameL"), (CComponent**)&m_pTransfrom_SkillFrameL)))
		return E_FAIL;
	//m_pTransfrom_SkillFrameR
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_SkillFrameR"), (CComponent**)&m_pTransfrom_SkillFrameR))) 
		return E_FAIL;
    return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources()
{
    return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_BubbleGaugeL()
{
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_BubbleGaugeC()
{
	return E_NOTIMPL;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_BubbleGaugeR()
{
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_SkillL()
{
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_SkillR()
{
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_SkillFrameL()
{
	return S_OK;
}

HRESULT CUI_IdentitySP_LinkedPlayer::Bind_ShaderResources_SkillFrameR()
{
	return S_OK;
}

CUI_IdentitySP_LinkedPlayer* CUI_IdentitySP_LinkedPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentitySP_LinkedPlayer* pInstance = new CUI_IdentitySP_LinkedPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentitySP_LinkedPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentitySP_LinkedPlayer::Clone(void* pArg)
{
	CUI_IdentitySP_LinkedPlayer* pInstance = new CUI_IdentitySP_LinkedPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentitySP_LinkedPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentitySP_LinkedPlayer::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTexture_BubbleGauge);
	Safe_Release(m_pTexture_GaugeCut);
	Safe_Release(m_pTexture_MoonFall);
	Safe_Release(m_pTexture_SunRise);
	Safe_Release(m_pTexture_SkillFrame);
	Safe_Release(m_pTexture_None);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTransform_BubbleGaugeL);
	Safe_Release(m_pTransform_BubbleGaugeC);
	Safe_Release(m_pTransform_BubbleGaugeR);
	Safe_Release(m_pTransform_SkillL);
	Safe_Release(m_pTransform_SkillR);
	Safe_Release(m_pTransfrom_SkillFrameL);
	Safe_Release(m_pTransfrom_SkillFrameR);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
